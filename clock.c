/*
 * File:   clock.c
 * Author: issamakhtar
 *
 * Created on October 27, 2022, 6:26 PM
 */

#include "xc.h"
#include "clock.h"

// For the clock frequency, use the following for clkval:
// 8 for 8 MHz
// 500 for 500 KHz
// 32 for 32 KHz
// Code given for clock switching
void newClk(unsigned int clkval)
{
    char COSCNOSC;   // Clock Oscillator Selection
    if (clkval == 8) // 8  Mhz
    {
        COSCNOSC = 0x00;
    }
    else if (clkval == 500) // 500 kHz
    {
        COSCNOSC = 0x66;
    }
    else if (clkval == 32) // 32 Khz
    {
        COSCNOSC = 0x55;
    }
    else
    {
        COSCNOSC = 0x55;
    }

    SRbits.IPL = 7;                    // Disable interrupts
    CLKDIVbits.RCDIV = 0;              // Set RCDIV to 0
    __builtin_write_OSCCONH(COSCNOSC); // Set clock oscillator selection
    __builtin_write_OSCCONL(0x01);     // Initiate clock switch
    OSCCONbits.OSWEN = 1;              // Enable clock switch
    while (OSCCONbits.OSWEN == 1)
    {
    } // Wait for clock switch to complete
    {
    }
    SRbits.IPL = 0; // Enable interrupts
}