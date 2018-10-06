#include <msp430.h> 

// Author: Brian Harlow
// Class: Introduction to Embedded Systems
// Section: 3
// Date: 5 October 2018
// Microcontroller: MSP430F5529

// Program Description: This program is designed to change the brightness of an LED at the push of a button. This is done using a Pulse
//                      Width Modulation Duty Cycle. The length of the Duty Cycle is incremented by 10% of the total pulse period for each
//                      button press.

/**
 * main.c
 */

void GPIOsetup(void){
    // Pin Setup
    P1SEL |= BIT2;              // Sets P1.2 to timer output
    P4SEL &= ~BIT7;             // Sets P4.7 to GPIO

    P1DIR |= BIT2;              // Sets P1.2 to output
    P1DIR &= ~BIT1;             // Sets P1.1 to input
    P4DIR |= BIT7;              // Sets P4.7 to output

    P4OUT &= ~BIT7;             // Sets LED at P4.7 off

    // Pull-up Resistor
    P1REN |= BIT1;              // Enable pull-up/pull-down resistor
    P1OUT |= BIT1;              // Pull-Up/Pull-Down resistor set to pull-up

    // Interrupt Setup
    P1IE |= BIT1;               // Enable interrupt from P1.1
    P1IES |= BIT1;              // Sets interrupt to trigger on falling edge
    P1IFG &= ~BIT1;             // Clears the interrupt flag for P1.1
}

void TA0setup(void){
    TA0CTL = TASSEL_2 + MC_1 + ID_2 + TACLR; // Enable SMCLK in UP Mode, divided by 4, clears the timer, enables CCR0 interrupt
    TA0CCR0 = 255;              // Sets CCR0 to 255
    TA0CCR1 = 125;              // Sets CCR1 to 125

    TA0CCTL1 |= OUTMOD_7;

}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    GPIOsetup();                // Runs GPIOsetup function
    TA0setup();                 // Runs TA0setup function

    _BIS_SR(LPM0_bits + GIE);   // Forces microcontroller into LPM0 and enables global interrupts
    return 0;
}

// Button Interrupt Service Routine
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
    P4OUT ^= BIT7;              // Toggles status of LED at P4.7
    if(P1IES & BIT1)            // Check condition for button press
    {
        TA0CCR1 += 25;          // Increases PWM Duty Cycle by 10%
        if(TA0CCR1 > 255)       // Check condition for CCR1 wrap-around
        {
            TA0CCR1 = 0;        // Resets CCR1 if wrap around is detected
        }
    }
    P1IES ^= BIT1;              // Toggle the edge select for the P1.1 interrupt
    P1IFG &= ~BIT1;             // Clears the interrupt flag for P1.1
}

