#ifndef ACQUISITION_H
#define ACQUISITION_H
#include <p32xxxx.h>

void ADCcSonfig(void);

#pragma interrupt ADC_interrupt ipl2 vector 27
void ADC_interrupt(void);

void DMAconfig();

#pragma interrupt DMA0_ISR ipl1 vector 36
void DMA0_ISR ();

#pragma interrupt DMA1_ISR ipl1 vector 37
void DMA1_ISR ();

#pragma interrupt DMA2_ISR ipl1 vector 38
void DMA2_ISR ();
#endif