#ifndef PWM_H
#define PWM_H
#include "dsp.h"

void LEDSignal(void);
void T3Con(void);
void initIntGlobal(void);
void initPWM(void);
void play(struct status signal);

#endif
