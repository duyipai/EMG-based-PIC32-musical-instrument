#include "acquisition.h"
#include "dsp.h"
//#include "speaker.h"

int main()
{
    INTCONbits.MVEC = 1; // Enable multiple vector interrupt
    //initIntGlobal();
    ADCcSonfig();
    DMAconfig();
    
	//T3Con();
	//LEDCon();
	//initPWM();
    AD1CON1bits.SAMP = 1;
	while (1)
    {
        //play(getNote());
        //struct status tmp= getNote();
	}
}