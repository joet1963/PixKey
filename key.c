/*
 * key.c
 *
 *  Created on: Apr 22, 2016
 *      Author: Magu
 */


#include "key.h"


uint8_t cmdbuff[16] = {0};
uint8_t cbindex = 0;



void keyiohandler(keyer *keydata)
{	
	uint8_t adresult;
	byte io = KB1_GetVal();
	
	if(io & 0x01) keydata->dit_sw = 0;	
	else keydata->dit_sw = 1;
		
	if(io & 0x02) keydata->dah_sw = 0;	
	else keydata->dah_sw = 1;
	
	if(keydata->mkey == 0)  // if not manual mode
	{
		if(io & 0x04) keydata->prog_sw = 1;	
		else
		{
			keydata->prog_sw = 0;
			if(keydata->pshbtimer > 2500 ) keydata->push = Psh3;
			else if(keydata->pshbtimer > 1000 ) keydata->push = Psh2;
			else if(keydata->pshbtimer > 125 ) keydata->push = Psh1;    // send message
			keydata->pshbtimer = 0;				
		}
	}
	
	(void)AD1_MeasureChan(1,0); // wait for result chan 0		
	(void)AD1_GetChanValue(0,&adresult); // read switch
		
	if(adresult <= 6)  keydata->function = FuncA;   // Function A                         0
	else if(adresult > 6 && adresult <= 14) keydata->function = FuncD;  // Function D     11
	else if(adresult > 14 && adresult <= 25) keydata->function = FuncB; // Function B     18  
	else if(adresult > 25 && adresult <= 40) keydata->function = FuncC;  // Function C    33 		
	
}



void initcmdbuff(void)
{
	cmdbuff[0] = '\0';
	cbindex = 0;	
}


void keysprocess(keyer *keydata, const byte lookuptable[], byte tablesize)
{
	byte i = 0;	
	word k = 1200 / keydata->speed;	
	keydata->keytimer++;
	keydata->txdelay++;
	keydata->pshbtimer++;
	
	if(keydata->mkey == 1)
	{
		if(KB1_GetVal() & 0x04) keydata->key_out = 0;
		else keydata->key_out = 1;
		return;
	}
	
	switch (keydata->keystate)
	{
		case idle:
			if (keydata->dit_sw == 1) keydata->keystate = send_dit;
			else if (keydata->dah_sw == 1) keydata->keystate = send_dah;
			else keydata->keystate = idle;
			if(keydata->chartimer++ > (k * 2))keydata->keystate = char_out;
			break;
			
		case dit: 
			if (keydata->dah_sw == 1) keydata->keystate = iambic;
			else keydata->keystate = send_dit;
			break;
			
		case send_dit:			
			keydata->keytimer = 0;
			keydata->key_out = 1;
			keydata->dit_flag = 1;
			keydata->keystate = wait_dit;
			keydata->key_char >>= 1; 
			keydata->key_char = keydata->key_char & 0x7F;			
			break;
			
		case dah:
			if (keydata->dit_sw == 1) keydata->keystate = iambic;
			else keydata->keystate = send_dah;
			break;
			
		case send_dah:
			keydata->keytimer = 0;
			keydata->key_out = 1;
			keydata->dit_flag = 0;
			keydata->keystate = wait_dah;
			keydata->key_char >>= 1; 
			keydata->key_char = keydata->key_char | 0x80;			
			break;
			
		case wait_dit:
			
			if(keydata->keytimer > k)
			{
				keydata->key_out = 0;
				keydata->keytimer = 0;
				keydata->keystate = pause;				
			}			
			break;
			
		case wait_dah:
			
			if(keydata->keytimer > (k * 3))
			{
				keydata->key_out = 0;
				keydata->keytimer = 0;
				keydata->keystate = pause;				
			}			
			break;
			
		case pause:			
			if(keydata->keytimer > k) keydata->keystate = idle;   // inter element space
			keydata->chartimer = 0;  // reset timer to complete char
			break;
			
		case char_out:
			keydata->chartimer = 0;  // reset timer
			if(keydata->key_char != 0x80)
			{
				while(keydata->key_char != lookuptable[i] && (i < tablesize )) i++;	
						
				if( keydata->key_char == lookuptable[i] ) 		
				{
					keydata->keybuff = i + 44; // adjust to ASCII
					keydata->new_char = 1; // flag					
					cmdbuff[cbindex++] = i + 44; // adjust to ASCII
					cmdbuff[cbindex] = '\0';
					if(cbindex >= 15) initcmdbuff();
				}
					
				keydata->key_char = 0x80;  // restore
			}
			keydata->keystate = idle;
			break;
			
		case iambic:			
			if(keydata->dit_flag == 1)keydata->keystate = send_dah;
			else keydata->keystate = send_dit;
			break;					
	
	}	
}
