/*
 * tim.c
 *
 *  Created on: May 6, 2016
 *      Author: Magu
 */

#include "tim.h"

#define TXDELAY 1000  // 1 second

/*
 * 			     RX        TX
 *           F1 0.0  kHz   .7  kHz   
 *           F2  .35 kHz  1.05 kHz
 *           F3  .70 kHz  1.4  kHz
 *           F4 1.05 kHZ   .35 kHz 
 *           F5 1.4  kHz   .7  kHz
 *           F6  .7  kHz  0.0  kHz
 * 
 */

const int8_t TXoffset[16] = {127,127,127,127,127,127,127,127,-128,-128,-128,-128,-128,-128,-128,-128};

const uint8_t RXF[6] = {0,27,54,81,109,54};

const uint8_t TXF[6] = {54,81,109,27,54,0};


void GENfreq(uint8_t ch, bool T)
{
	if(T)
	PWM2_SetRatio8(TXF[ch]);
	else
	PWM2_SetRatio8(RXF[ch]);	
}




void txiohandler(keyer *keydata, coder *codedata)
{
	uint8_t adresult;
	
	if(keydata->txdelay > TXDELAY){ keydata->txdelay = 0; keydata->T = 0;}	 // returns back to rx after delay
	
	if(keydata->key_out || codedata->key_out)
	{
		if(!keydata->local){ Bit1_SetVal(); keydata->txdelay = 0; keydata->T = 1; } // goes to tx
		(void)PWM1_Enable();		
	}	
	else
	{
		Bit1_ClrVal();
		(void)PWM1_Disable();
	}	
	
	(void)AD1_MeasureChan(1,1); // wait for result chan 1	
	(void)AD1_GetChanValue(1,&adresult); // read potentiometer
	if(keydata->T)(void)PWM2_SetRatio8((byte)adresult + TXoffset[adresult/16]); // generate a TX shift	
	else (void)PWM2_SetRatio8(adresult); // pass value through
	
	(void)AD1_MeasureChan(1,2); // wait for result chan 2		
	(void)AD1_GetChanValue(2,&adresult); //  read rf detector	
	if(adresult > 70)Bit2_SetVal(); // red LED on
	else Bit2_ClrVal(); // red LED off		
	
}




void timerProcess(timer *timedata)
{
	if(++timedata->msTimer >= 1000) // 1000 mS
		{
			timedata->msTimer = 0;
			if(++timedata->secTimer > 59)
			{
				timedata->secTimer = 0;
				timedata->minTimer++;
			}
		}
						
		timedata->acTimer++;	
}

/* 
 * 
 * 
 * 
 * 
 * 
 * 
 *                  _____________________
 *                 |                     |
 *          RESET -| 1  PTA5    PTA0  16 |- Side
 *          BKGD  -| 2  PTA4    PTA1  15 |- Mic/switch
 *          +3.3V -| 3  VDD     PTA2  14 |- Dit    
 *           GND  -| 4  VSS     PTA3  13 |- Dah      
 *                -| 5  PTB7    PTB0  12 |- Pot
 *                -| 6  PTB6    PTB1  11 |- Pwr
 *          Shift -| 7  PTB5    PTB2  10 |- LED
 *          Key   -| 8  PTB4    PTB3  9  |- PSHSW1
 *                 |_____________________|
 * 
 * 
 *
 *
 * Ear speaker impedance	16 ohms or higher	Recommend 32 - 300 ohms
 * Mic DC resistance	1000 ohms or higher	Mic characteristics must be compliant with section 5.4 Audio Recording of the current Android CDD
 * Control Function Equivalent impedance*
 *   0 ohm	                    [Function A] Play/Pause/Hook   < 6
 * 240 ohm +/- 1% resistance	[Function B] 216 mV  18    > 14 < 25        
 * 470 ohm +/- 1% resistance	[Function C] 387 mV  33    > 25  < 40
 * 135 ohm +/- 1% resistance	[Function D] 127 mV  11    > 6 < 14
 * Total impedance from positive mic terminal to GND when button is pressed with 2.2 V mic bias applied through 2.2 kOhm resistor
 *
 * In the following diagrams, Button A maps to Function A, Button B to Function B, and so on.
 *
 *
 * R when entering command mode
 * If the battery voltage is approaching the minimum operating range, it
 * will respond with an L instead.
 * The actual supply
 * voltage can be read out in Morse by using the V command
 * use pot for speed and shift
 *
 */





