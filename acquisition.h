#ifndef ACQUISITION_H
#define ACQUISITION_H
#include <p32xxxx.h>

void ADCcSonfig(void);

void ADC_interrupt(void);

void DMAconfig();


void DMA0_ISR ();

void DMA1_ISR ();

void DMA2_ISR ();
#endif