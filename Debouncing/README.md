# Brian Harlow's Software Debouncing

## Program Purpose
This program was designed to nullify the effects of bouncing on a switch using software programming. Bouncing has the potential to toggle an input more times than desired which could yield incorrect results in a program. Debouncing stops the input from toggling more than desired so that the result is the proper output.

## Functionality
A 32 kHz timer is used to time several milliseconds after the button is pressed. At that point in time the status of the button is pressed to decide whether the LED should be toggled or not. By waiting the several milliseconds to check the button status, any bouncing is completely avoided.

## Microcontrollers Used
* MSP430F5529 - The LED used is at P1.0 and the button is at P1.1. The clock used is the ACLK at 10 kHz.
* MSP430FR2311 - The LED used is at P1.0 and the button is at P1.1. The clock used is the ACLK set to 32 kHz.
