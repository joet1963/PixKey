/*
 * mess.c
 *
 *  Created on: May 5, 2016
 *      Author: Magu
 */

#include "mess.h"




void Mem(keyer *keydata, prog *progdata, coder *codedata )
{
	if(keydata->push != Psh1)return;  // exit no message to send
	
	keydata->push = noPsh;	// clear for next push
	keydata->local = 0; // not local
	
	switch(progdata->messageselect)
	{
	
		case 1:
			while(sendMess("CQ CQ CQ DE VA3VCO VA3VCO AR K", codedata) == ERR_BUSY);			
			break;
			
		case 2:
			while(sendMess("TEST VA3VCO VA3VCO TEST", codedata) == ERR_BUSY);			
			break;
			
		case 3:
			while(sendMess("CQ CQ CQ DE VA3VCO VA3VCO CQ CQ CQ DE VA3VCO AR K", codedata) == ERR_BUSY);			
			break;
			
		case 4:
			while(sendMess("CQ CQ CQ DE VA3VCO VA3VCO/QRP AR K", codedata) == ERR_BUSY);			
			break;	
					
		default:
			while(sendMess("???", codedata) == ERR_BUSY);			
			break;	
	
	}	
}


