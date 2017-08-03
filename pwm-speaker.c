#include <p32xxxx.h>
#include <plib.h>
#include "dsp.h"

/* Global variables */
static unsigned int flags = 0;
//static unsigned int flag_T2 = 0;

static unsigned int c3 = 11416;
static unsigned int d3 = 10201;
static unsigned int e3 = 9089;
static unsigned int f3 = 8595;
static unsigned int g3 = 7651;
static unsigned int a3 = 6816;
static unsigned int b3 = 6071;
static unsigned int c4 = 5735;
static unsigned int tmp = 5735;
static struct status signal;

/* Function prototypes */
static void initIntGlobal (void);
static void initTimer (void);
static void initPWM (void);

#pragma interrupt T3_ISR ipl4 vector 12
//#pragma interrupt T2_ISR ipl5 vector 8


static void GenMsec(void) {
	PR3 = 19999; // Load PR3
	TMR3 = 0x0; // Clear contents of TMR3
	T3CONSET = 0x8000; // Start Timer 3
	while(1){
		if(flags==1){
			flags=0;
			break;
		}
	}
	T3CONCLR = 0x8000; // Close Timer 3
}

static void DelayMsec(int num) {
	int i;
	for (i=0; i<num; i++) {
		GenMsec();
	}
}
/* timer interrupt handler */
static void T3_ISR (void) 
{
	flags=1;
	IFS0CLR = (1<<12); // Clear timer 3 interrupt flag
}
/*
static void T2_ISR (void){
	flag_T2 = 1;
	IFS0CLR = (1<<8); // Clear timer 2 interrupt flag
	
}
*/

static void LEDCon(void){
	TRISD = 0x1;
}

static void give0(void){
		PORTDbits.RD1 = 1;
		asm("NOP");
		PORTDbits.RD1 = 0;
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
	
}
static void LEDSignal(void){
	PORTDbits.RD1 = 1;
	PORTDbits.RD1 = 0;
	PORTDbits.RD1 = 1;
	PORTDbits.RD1 = 0;
}


static void T3Con(void){

	IPC3SET = 0b010001; // Interrupt priority level 4, Subpriority level 1
	IFS0CLR = 0x1000; // Clear timer interrupt flag
	IEC0SET = 0x1000; // Enable Timer3 interrupt
	T3CON = 0x0; // Stop any 16/32-bit Timer2 operation
	T3CONbits.TCKPS =  0b01; // Enable 16-bit mode, prescaler 1:8,
                    // internal clock
	TMR3 = 0x0; // Clear contents of TMR2
}

/* Timer2 ISR - handling OC-PWM module operations */
#pragma interrupt PWM_ISR ipl3 vector 8
static void PWM_ISR (void) {
	OC1RS = PR2 * 0.05; //update duty cycle register
	IFS0CLR = 0x0100; //clear Timer 2 interrupt flag
}

/* Configure interrupt globally */
static void initIntGlobal() {
	INTCONbits.MVEC = 1; // Enable multiple vector interrupt
	asm("ei"); // Enable all interrupts
}



/* Initialize OC module and timer base - Timer 2 */
static void initPWM() {
	OC1CON = 0x0000; //stop OC1 module
	OC1RS = 0; //initialize duty cycle register
	OC1R = 0; //initialize OC1R register for the first time
	OC1CON = 0x0006; //OC1 16-bit, Timer 2, in PWM mode w/o FP
	
	/*set frequency*/
	PR2 = tmp;

	IFS0CLR = 0x00000100; //clear Timer 2 interrupt	
	IEC0SET = 0x00000100; //enable Timer 2 interrupt
	IPC2SET = 0x0000000F; //Timer 2 interrupt priority 3, subpriority 3
	T2CONbits.TCKPS = 0b101; 	// prescale = 1:64
	T2CONSET = 0x8000; //start Timer 2
	OC1CONCLR = 0x8000; //enable OC1 module for PWM generation
}

static void playnote(unsigned int n){
	PR2 = n;
	OC1CONSET = 0x8000;
	DelayMsec(700);
	OC1CONCLR = 0x8000;
	DelayMsec(300);
	
}

static void note(unsigned int n){
	PR2 = n;
	OC1CONSET = 0x8000;
}

static void StarsSim(void){
	playnote(c3);
	playnote(c3);
	playnote(g3);
	playnote(g3);
	playnote(a3);
	playnote(a3);
	playnote(g3);
	DelayMsec(1000);
	playnote(f3);
	playnote(f3);
	playnote(e3);
	playnote(e3);
	playnote(d3);
	playnote(d3);
	playnote(c3);
	DelayMsec(1000);
	playnote(g3);
	playnote(g3);
	playnote(f3);
	playnote(f3);
	playnote(e3);
	playnote(e3);
	playnote(d3);
	DelayMsec(1000);
	playnote(g3);
	playnote(g3);
	playnote(f3);
	playnote(f3);
	playnote(e3);
	playnote(e3);
	playnote(d3);
	DelayMsec(1000);
	playnote(c3);
	playnote(g3);
	playnote(g3);
	playnote(a3);
	playnote(a3);
	playnote(g3);
	DelayMsec(1000);
	playnote(f3);
	playnote(f3);
	playnote(e3);
	playnote(e3);
	playnote(d3);
	playnote(d3);
	playnote(c3);
	DelayMsec(1000);

}
static void TestPWMFrequency(void){
	note(d3);
}

static void play(){
	OC1CONCLR = 0x8000;
	
	signal = getNote();
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

/* main function */
int main() {
	initIntGlobal();
	T3Con();
	LEDCon();
	initPWM();
	while (1){
		//StarsSim();
		//TestPWMFrequency();
		//LEDSignal();
	}
}