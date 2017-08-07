#include "dsp.h"
#include <plib.h>
#include <sys/kmem.h>

static int flags=0;
static int buff = 0;
static unsigned char valueA,valueB,valueC,nonidea;
static unsigned char arrA1[20];

static unsigned char arrA2[20];
static unsigned char arrB1[20];

static unsigned char arrB2[20];
static unsigned char arrC1[20];

static unsigned char arrC2[20];

static int count=0;

void ADCcSonfig(void){
	asm ("ei"); 				// Enable all interrupts
	IPC6SET = 0x08000000; 		// Interrupt priority level 2, Subpriority level 0
	IFS1CLR = 0x00000002; 		// Clear timer interrupt flag
	IEC1SET = 0x00000002; 		// Enable ADC interrupt

	TRISB = 0xffff;		//a. pin input
	AD1PCFG = 0xffc7;      //PORTB = Digital; RB3,4,5 = analog

//	AD1CHSbits.CH0SA = 0b0011;	//b.  AN3 as pos input
 // AD1CHSbits.CH0SB = 0b0011;	//b.  AN3 as pos input
	//AD1CON1bits = 0x000;		//c. set output format 000 (unsigned, frac, 16-bit)	
    AD1CON1bits.FORM=0b100;	
	AD1CON1SET = 0xE0;		//d. SSRC
	AD1CON2bits.VCFG = 0b011;	//e. VCFG 
	//AD1CON2SET = 0x400;		//f. CSCNA field of AD1CON2, MUX A
	//g. SMPI 0010
	AD1CON2bits.SMPI = 0b0010;
    //scan mode enable
    AD1CON2bits.CSCNA=0b1;
    //select scan pins,an3,an4,an5
    AD1CSSLSET=0x38;
	//h. BUFM
	AD1CON2CLR = 0x01;
	//i. ALTS (which MUX)
//	AD1CON2CLR = 0x0;
 	//j. ADRC		Tad > 65ns
	AD1CON3CLR = 0x8000;
	//k. SAMC auto-sample time bits 20
	AD1CON3bits.SAMC = 0b10100;
	//l. ADCS Tad = 500 Tpb -> ADCS = 249
	AD1CON3SET = 0xF9;

	//m. turn on
	AD1CON1SET = 0x8000;
	AD1CON1bits.ASAM = 1;
}

#pragma interrupt ADC_interrupt ipl2 vector 27
void ADC_interrupt(void){
	//AD1CON1SET = 0x1;

	// deliver data to LCD
    if(count>=19)
    {
     	AD1CON1bits.SAMP = 0;
    }

    if(ADC1BUF0>=0b1100000000)
    {
     valueA=0b11111010;
     nonidea=ADC1BUF0;
     }
	else
    {
valueA=(330*ADC1BUF0)/1024;
nonidea=ADC1BUF0;
}
    
    if(ADC1BUF1>=0b1100000000)
    {
     valueB=0b11111010;
nonidea=ADC1BUF1;
     }
	else
    {valueB=(330*ADC1BUF1)/1024;
nonidea=ADC1BUF1;} 
 
    if(ADC1BUF2>=0b1100000000)
    {
     valueC=0b11111010;
nonidea=ADC1BUF2;
     }
	else
    {valueC=(330*ADC1BUF2)/1024;
nonidea=ADC1BUF2;}   

    arrA1[count]=valueA;
    arrB1[count]=valueB;
    arrC1[count]=valueC;
    count++;

	if(count>=20)
     {
		int i1;
		for(i1=0;i1<=19;i1++)
		{
		arrA2[i1]=arrA1[i1];
		}
		for(i1=0;i1<=19;i1++)
		{
		arrB2[i1]=arrB1[i1];
		}
		for(i1=0;i1<=19;i1++)
		{
		arrC2[i1]=arrC1[i1];
		}
		count=0;
		DCH0ECONSET=0x00000080;//SET CFORCE to be 1 to start dma transfer
		DCH1ECONSET=0x00000080;//SET CFORCE to be 1 to start dma transfer
		DCH2ECONSET=0x00000080;//SET CFORCE to be 1 to start dma transfer
		AD1CON1bits.SAMP = 1;
    }
	
	IFS1CLR = 0x2; 		// Clear ADC interrupt flag
}

void DMAconfig()
{
IFS1CLR=0x00010000; // clear existing DMA channel 0 interrupt flag
IFS1CLR=0x00020000; // clear existing DMA channel 1 interrupt flag
IFS1CLR=0x00040000; // clear existing DMA channel 2 interrupt flag
DMACONSET=0x00008000; // enable the DMA controller
DCH0CON=0x3; // channel 0 disabled, priority 3, no chaining
DCH1CON=0x3; // channel 1 disabled, priority 3, no chaining
DCH2CON=0x3; // channel 2 disabled, priority 3, no chaining
DCH0ECON=0; // no start or stop IRQ, no pattern match
DCH1ECON=0; // no start or stop IRQ, no pattern match
DCH2ECON=0; // no start or stop IRQ, no pattern match
/********* program the transfer *********/
DCH0SSA=KVA_TO_PA(&arrA2[0]); // transfer source physical address
DCH0DSA=KVA_TO_PA(&receiver.arrA[0]); // transfer destination physical address
DCH0SSIZ=20; // source size 30 bytes
DCH0DSIZ=20; // destination size 30 bytes
DCH0CSIZ=20; // 30 bytes transferred per event
DCH1SSA=KVA_TO_PA(&arrB2[0]); // transfer source physical address
DCH1DSA=KVA_TO_PA(&receiver.arrB[0]); // transfer destination physical address
DCH1SSIZ=20; // source size 30 bytes
DCH1DSIZ=20; // destination size 30 bytes
DCH1CSIZ=20; // 30 bytes transferred per event
DCH2SSA=KVA_TO_PA(&arrC2[0]); // transfer source physical address
DCH2DSA=KVA_TO_PA(&receiver.arrC[0]); // transfer destination physical address
DCH2SSIZ=20; // source size 30 bytes
DCH2DSIZ=20; // destination size 30 bytes
DCH2CSIZ=20; // 30 bytes transferred per event
DCH0INTCLR=0x00ff00ff; // clear existing events, disable all interrupts
DCH0INTbits.CHBCIE=1;//enable the destination done interrupt
DCH1INTCLR=0x00ff00ff; // clear existing events, disable all interrupts
DCH1INTbits.CHBCIE=1;//enable the destination done interrupt
DCH2INTCLR=0x00ff00ff; // clear existing events, disable all interrupts
DCH2INTbits.CHBCIE=1;//enable the destination done interrupt
IPC9CLR=0x0000001f; // clear the DMA channel 0 priority and sub-priority
IPC9SET=0x00000006; // set IPL 1, sub-priority 2
IPC9CLR=0x00001F00; // clear the DMA channel 1 priority and sub-priority
IPC9SET=0x00000600; // set IPL 1, sub-priority 2
IPC9CLR=0x001F0000; // clear the DMA channel 2 priority and sub-priority
IPC9SET=0x00060000; // set IPL 1, sub-priority 2
IEC1SET=0x00010000; // enable DMA channel 0 interrupt
IEC1SET=0x00020000; // enable DMA channel 1 interrupt
IEC1SET=0x00040000; // enable DMA channel 2 interrupt
DCH0CONSET=0x80; // turn channel on
DCH1CONSET=0x80; // turn channel on
DCH2CONSET=0x80; // turn channel on
}

#pragma interrupt DMA0_ISR ipl1 vector 36
void DMA0_ISR ()
{
IFS1CLR=0x00010000; // clear existing DMA channel 0 interrupt flag
DCH0ECONbits.CABORT=0b1;
pushStatus();
}

#pragma interrupt DMA1_ISR ipl1 vector 37
void DMA1_ISR ()
{
IFS1CLR=0x00020000; // clear existing DMA channel 1 interrupt flag
DCH1ECONbits.CABORT=0b1;
pushStatus();
}

#pragma interrupt DMA2_ISR ipl1 vector 38
void DMA2_ISR ()
{
IFS1CLR=0x00040000; // clear existing DMA channel 2 interrupt flag
DCH2ECONbits.CABORT=0b1;
pushStatus();
}