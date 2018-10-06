# Brian Harlow's Software PWM

## Program Purpose
This program was designed to use a button to increment the brightness of an LED on an MSP430 development board.

## Functionality
A clock is set to up mode to trigger a overflow interrupt flag when the timer reaches a value set in CCR0. Another capture/compare register, CCRx, is set to another value at a certain percentage of CCR0 to determine the pulse width. The smaller the pulse width, the dimmer the LED will be. When the CCRx value is hit, another interrupt flag toggles the LED as does the CCR0 overflow. The button press changes the value in CCRx to change the pulse width, and thus the brightness of the LED.

## Microcontrollers Used
* MSP430F5529 - The LEDs used is at P1.0 and P2.0 and the button is at P1.1. The clock used is the ACLK at 10 kHz.
* MSP430FR2311 - The LEDs used are at P1.0 and P4.7 and the button is at P1.1. The clock used is the ACLK set to 32 kHz.
