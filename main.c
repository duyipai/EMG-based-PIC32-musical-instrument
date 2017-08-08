#include "acquisition.h"
#include "dsp.h"
#include "speaker.h"

int main()
{
    INTCONbits.MVEC = 1; // Enable multiple vector interrupt
    initIntGlobal();
    ADCcSonfig(); 
	initPWM();
    AD1CON1bits.SAMP = 1;
	tr4config();
	while (1)
    {
        struct status tmp= getNote();
        play(tmp);
	}
}