

#include "xc.h"
#include "IOinit.h"
#include "TimeDelay.h"
#include "UART2.h"
#include "Interrupts.h"

#include<stdio.h>
extern uint8_t cn0Flag; // Flag to indicate that the CN0 interrupt has occurred
extern uint8_t cn1Flag; // Flag to indicate that the CN1 interrupt has occurred
extern uint8_t cn30Flag; // Flag to indicate that the CN30 interrupt has occurred 
extern uint8_t min; 	 // Global variable to store the minutes
extern uint8_t sec ; 	// Global variable to store the seconds
uint8_t state = 0; // State variable
uint8_t press = 0; // Press variable
char time[25];

void IOinit()
{
    AD1PCFG = 0xFFFF;     // Set all pins to digital
    TRISBbits.TRISB8 = 0; // Make GPIO RB8 as a digital output
    LATBbits.LATB8 = 0;   // Make GPIO RB8 as a digital output

    TRISAbits.TRISA4 = 1; // Makes GPIO RA4 as a digital input
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN0IE = 1;  // enable CN on CN0

    TRISBbits.TRISB4 = 1; // Makes GPIO RB4 as a digital input
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN1IE = 1;  // enable CN on CN1

    TRISAbits.TRISA2 = 1;  // Makes GPIO RA2 as a digital input
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN2bits.CN30IE = 1;  // enable CN on CN30

    IPC4bits.CNIP = 6; // 7 is highest priority, 1 is lowest, 0 is disabled interrupt
    IFS1bits.CNIF = 0; // Clear interrupt flag
    IEC1bits.CNIE = 1; // Enable CN interrupts

    InitUART2(); // Initialize UART2 connection for 300 baud rate and 32 kHz clock
}

void IOcheck()
{
    IEC1bits.CNIE = 0;                                               // disable CN interrupts to avoid debounces
    delay_ms(200, 1);                                                // 200 msec delay to filter out debounces
    IEC1bits.CNIE = 1;                                               // Enable CN interrupts to detect pb release
    while (cn30Flag == 1 ) // if only Ra2 is pressed
    {
	    if (min < 59 && min >= 0){min++;} //minute boundary check and increment
        display();        // display message
        
    }
    while (cn0Flag == 1) // if only ra4 is pressed
    {
	    if(sec <59 && sec >= 0){ //second boundary check and increment
		    sec++;
	    }
        display(); // display message
        
    }
    while (cn1Flag == 1) // if only rb4 is presed
    {
	    press = 0;	     //reset press
	    delay_ms(3000,1); //3 second delay
	    if (cn1Flag == 1){ //if rb4 is still pressed after 3 seconds (long press)
		    min = 0;    //reset minute
		    sec = 0;   //reset second
			sprintf(time, "\r%02dm: %02ds                       ", min, sec); //print time
			Disp2String(time); //display time
			LATBbits.LATB8 = 0; //turn off LED
	    }else{display();} //if rb4 is not pressed after 3 seconds (short press)
	    
    }
    
}

void display()
{
    if (cn30Flag == 1) // if only pb1 is pressed
    {
	sprintf(time, "\r%02dm: %02ds", min, sec); //print time

	   Disp2String(time); //display time
	   Disp2String("                       ");
    }
    else if (cn0Flag == 1) // if only pb2 is pressed
    {
	sprintf(time, "\r%02dm: %02ds ", min, sec); //print time
	   Disp2String(time); //display time
	   Disp2String("                       ");

    }
    else if (cn1Flag == 0) // if only pb3 is pressed
    {
	    state = state ^ 1;	     //toggle state 
	    Count(state);		 //countdown
	    
    }

}
int check (){//fucntion to check for pb3 press and toggle 
	if (PORTBbits.RB4 == 0){ 
		return 1;
	}
	return 0;
	
}
void Count(int state){
	int totalsecs = sec + (min * 60); //total seconds
	if (state == 1){ //if state is 1
		for (int i = 0; i < totalsecs ; i++ ){ //countdown each iteration is one second
			int c = check(); //check for pb3 press
			if (c == 1) {break;}; //if pb3 is pressed and paused break out of loop for pause
			if (sec > 0 && sec <= 59){  //second boundary check and decrement
			sec--;}
			if (min == 0 && sec == 0){ //if time is up
				Disp2String("\r00m: 00s --ALARM"); //display alarm
				LATBbits.LATB8 = 1; //turn on LED
				break; //break out of loop
			}
			if (sec <= 0){ //if seconds is 0
				if (min >0){ //if minutes is greater than 0
					min--;	//decrement minutes
				}
				sec = 59; //reset seconds 
			}
			sprintf(time, "\r%02dm: %02ds                       ", min, sec); //print time
			Disp2String(time); //display time
			LATBbits.LATB8 = LATBbits.LATB8 ^ 1; //toggle LED
			delay_ms(1000,1); //delay for 1 second
		}
		
	}
	
}