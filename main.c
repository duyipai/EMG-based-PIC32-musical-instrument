#include "acquisition.h"
#include "pwm-speaker.h"

int main()
{
    initIntGlobal();
    ADCcSonfig();
    DMAconfig();
	T3Con();
	LEDCon();
	initPWM();
	while (1)
    {
        //play(getNote());
		StarsSim();
	}
}