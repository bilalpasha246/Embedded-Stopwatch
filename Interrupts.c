/*
 * File:   T2interrupt.c
 * Author: issamakhtar
 *
 * Created on October 27, 2022, 6:27 PM
 */

#include "xc.h"
#include "Interrupts.h"
#include "IOinit.h"
#include "TImeDelay.h"
extern uint8_t t2Flag;
extern uint8_t cn0Flag;
extern uint8_t cn1Flag;
extern uint8_t cn30Flag;
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
	IFS0bits.T2IF = 0; // Clear interrupt flag
	T2CONbits.TON = 0; // Disable timer
	TMR2 = 0;		   // Reset timer
}
// Change of pin Interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
	// use if statements not else if so every flag is checked
	if (PORTAbits.RA2 == 0) // if pb1 is pressed
	{
		cn30Flag = 1; // set flag
	}
	if (PORTAbits.RA4 == 0) // if pb2 is pressed
	{
		cn0Flag = 1; // set flag
	}
	if (PORTBbits.RB4 == 0) // if pb3 is pressed
	{
		cn1Flag = 1; // set flag
	}
	if (PORTAbits.RA2 == 1) // if pb1 is let go
	{
		cn30Flag = 0; // clear flag
	}
	if (PORTAbits.RA4 == 1) // if pb2 is let go
	{
		cn0Flag = 0; // clear flag
	}
	if (PORTBbits.RB4 == 1) // if pb3 is let go
	{
		cn1Flag = 0; // clear flag
	}
	IFS1bits.CNIF = 0; // clear IF flag
	Nop();

	return;
}