#include <msp430.h> 

// Author: Brian Harlow
// Class: Introduction to Embedded Systems
// Section: 3
// Date: 3 October 2018
// Microcontroller: MSP430F5529

// Program Description: This program is designed to nullify the effects of button bouncing, or debounce, a button on an MSP430
//                      development board. At the push of the button, the status of the desired LED will toggle.

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	UCSCTL4 = SELA_1;           // Sets ACLK to VLO at 10-kHz
	
	// GPIO Setup
	P1SEL &= ~(BIT0 + BIT1);    // Sets P1.0 and P1.1 to GPIO
	P1DIR |= BIT0;              // Sets P1.0 to output
	P1DIR &= ~BIT1;             // Sets P1.1 to input

	// Pull-up Resistor
	P1REN |= BIT1;              // Enable pull-up/pull-down resistor
	P1OUT |= BIT1;              // Pull-Up/Pull-Down resistor set to pull-up

	// Interrupt Setup
    P1IE |= BIT1;               // Enable interrupt from P1.1
    P1IES |= BIT1;              // Sets interrupt to trigger on falling edge
    P1IFG &= ~BIT1;             // Clears the interrupt flag for P1.1

	// Capture/Compare Register Setup
    TA0CCR0 = 40;               // Sets CCR0 register to 40

	// Low Power Mode and Global Interrupts
	_BIS_SR(LPM0_bits + GIE);   // Enters microcontroller into LPM0 and enables global interrupts
	return 0;
}

// Button Interrupt Service Routine
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
    TA0CTL = TASSEL_1 + MC_1 + TACLR + TAIE;   // Sets timer to ACLK, UP mode, and clears the clock
    P1IES ^= BIT1;                      // Toggle the edge select for the P1.3 interrupt
    P1IFG &= ~BIT1;                     // Clears the interrupt flag for P1.3
}

// Timer Interrupt Service Routine
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A0(void) {
    if(P1IN & BIT1)     // Check condition for is button is pushed
    {
        P1OUT ^= BIT0;  // Toggles status of D1
    }
    TA0CTL = MC_0;      // Sets timer to STOP Mode
}

