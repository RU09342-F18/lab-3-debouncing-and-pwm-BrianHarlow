#include <msp430.h> 

// Author: Brian Harlow
// Class: Introduction to Embedded Systems
// Section: 3
// Date: 5 October 2018
// Microcontroller: MSP430FR2311

// Program Description: This program is designed to change the brightness of an LED at the push of a button. This is done using a Pulse
//                      Width Modulation Duty Cycle. The length of the Duty Cycle is incremented by 10% of the total pulse period for each
//                      button press.

/**
 * main.c
 */

void GPIOsetup(void){
    // Pin Setup
    P1SEL0 &= ~BIT7;            // Sets P1.7 to timer output
    P1SEL1 |= BIT7;             // Sets P1.7 to timer output
    P2SEL0 &= ~BIT0;            // Sets P2.0 to GPIO
    P2SEL1 &= ~BIT0;            // Sets P2.0 to GPIO

    P1DIR |= BIT7;              // Sets P1.7 to output
    P2DIR |= BIT0;              // Sets P2.0 to output

    P2OUT &= ~BIT0;             // Sets LED at P2.0 off

    // Pull-up Resistor
    P1REN |= BIT1;              // Enable pull-up/pull-down resistor
    P1OUT |= BIT1;              // Pull-Up/Pull-Down resistor set to pull-up

    // Interrupt Setup
    P1IE |= BIT1;               // Enable interrupt from P1.1
    P1IES |= BIT1;              // Sets interrupt to trigger on falling edge
    P1IFG &= ~BIT1;             // Clears the interrupt flag for P1.1
}

void TB0setup(void){
    TB0CTL = TBSSEL_2 + MC_1 + ID_2 + TBCLR; // Enable SMCLK in UP Mode, divided by 4, clears the timer, enables CCR0 interrupt
    TB0CCR0 = 255;              // Sets CCR0 to 255
    TB0CCR2 = 125;              // Sets CCR2 to 125

    TB0CCTL2 |= OUTMOD_7;

}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // Allows changes to port registers

    GPIOsetup();                // Runs GPIOsetup function
    TB0setup();                 // Runs TB0setup function

    _BIS_SR(LPM0_bits + GIE);   // Forces microcontroller into LPM0 and enables global interrupts
    return 0;
}

// Button Interrupt Service Routine
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
    P2OUT ^= BIT0;              // Toggles status of LED at P2.0
    if(P1IES & BIT1)            // Check condition for button press
    {
        TB0CCR2 += 25;          // Increases PWM Duty Cycle by 10%
        if(TB0CCR2 > 255)       // Check condition for CCR2 wrap-around
        {
            TB0CCR2 = 0;        // Resets CCR2 if wrap around is detected
        }
    }
    P1IES ^= BIT1;              // Toggle the edge select for the P1.1 interrupt
    P1IFG &= ~BIT1;             // Clears the interrupt flag for P1.1
}

