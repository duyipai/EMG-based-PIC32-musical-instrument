#include <p32xxxx.h>
#include <plib.h>
#include "dsp.h"
#include "speaker.h"

/*
In this file, components are used to fit FRCDIV (4MHz)
*/

/* Global variables */
static unsigned int flags = 0;
//static unsigned int flag_T2 = 0;

static unsigned int c3 = 3816;
static unsigned int d3 = 3400;
static unsigned int e3 = 3029;
static unsigned int f3 = 2864;
static unsigned int g3 = 2550;
static unsigned int a3 = 2272;
static unsigned int b3 = 2023;
static unsigned int c4 = 1911;
static unsigned int tmp = 1911;



/* Timer2 ISR - handling OC-PWM module operations */
#pragma interrupt PWM_ISR ipl3 vector 8
static void PWM_ISR (void) {
	OC1RS = PR2 * 0.12; //update duty cycle register
	IFS0CLR = 0x0100; //clear Timer 2 interrupt flag
}

/* Configure interrupt globally */
void initIntGlobal(void) {
	INTCONbits.MVEC = 1; // Enable multiple vector interrupt
	asm("ei"); // Enable all interrupts
}



/* Initialize OC module and timer base - Timer 2 */
void initPWM(void){
	OC1CON = 0x0000; //stop OC1 module
	OC1RS = 0; //initialize duty cycle register
	OC1R = 0; //initialize OC1R register for the first time
	OC1CON = 0x0006; //OC1 16-bit, Timer 2, in PWM mode w/o FP
	
	/*set frequency*/
	PR2 = tmp;

	IFS0CLR = 0x00000100; //clear Timer 2 interrupt	
	IEC0SET = 0x00000100; //enable Timer 2 interrupt
	IPC2SET = 0x0000000F; //Timer 2 interrupt priority 3, subpriority 3
	T2CONbits.TCKPS = 0b000; 	// prescale = 1:64
	T2CONSET = 0x8000; //start Timer 2
	OC1CONCLR = 0x8000; //enable OC1 module for PWM generation
}


static void note(unsigned int n){
	PR2 = n;
	OC1CONSET = 0x8000;
}


static void TestPWMFrequency(void){
	note(d3);
}

void play(struct status signal){
	if (signal.statusA == 0 && signal.statusB == 0 && signal.statusC == 0){
		OC1CONCLR = 0x8000;
	}
	else if(signal.statusA == 0 && signal.statusB == 0 && signal.statusC == 1){
		note(c3);
	}
	else if(signal.statusA == 0 && signal.statusB == 1 && signal.statusC == 0){
		note(d3);
	}
	else if(signal.statusA == 0 && signal.statusB == 1 && signal.statusC == 1){
		note(e3);
	}
	else if(signal.statusA == 1 && signal.statusB == 0 && signal.statusC == 0){
		note(f3);
	}
	else if(signal.statusA == 1 && signal.statusB == 0 && signal.statusC == 1){
		note(g3);
	}
	else if(signal.statusA == 1 && signal.statusB == 1 && signal.statusC == 0){
		note(a3);
	}
	else if(signal.statusA == 1 && signal.statusB == 1 && signal.statusC == 1){
		note(b3);
	}

}

