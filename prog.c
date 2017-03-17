/*
 * prog.c
 *
 *  Created on: May 4, 2016
 *      Author: Magu
 */

#include "prog.h"


extern coder mycodeprocess;


void Prog(keyer *keydata, prog *progdata, coder *codedata)
{	
	if(progdata->progentry == 1) // prog mode intialization
	{
		progdata->progmode = 1;
		progdata->progentry = 0;
		initcmdbuff();
		while(sendMess("P", codedata) == ERR_BUSY);
	}
	
	if(progdata->progmode == 0 || keydata->new_char == 0)return;	
	
		keydata->new_char = 0;
		
		switch(progdata->progstate)
		{
			case param:
			
				switch(keydata->keybuff)
				{
					case 'A':
						keydata->local = 0;
						while(sendMess("DE", codedata) == ERR_BUSY);
						progdata->progstate = param;
						progdata->progmode = 0; // complete				
						break;
						
					case 'B':
						//progdata->beacon = 1;
						progdata->progstate = param;
						keydata->local = 1;
						while(sendMess("E", codedata) == ERR_BUSY);						
						progdata->progmode = 0; // complete
						break;
						
					case 'F':
						progdata->progstate = freq;
						keydata->local = 1;
						while(sendMess("F", codedata) == ERR_BUSY);						
						break;
						
					case 'K':
						keydata->key_out = 1;
						progdata->progstate = param;
						keydata->local = 1;
						while(sendMess("E", codedata) == ERR_BUSY);						
						progdata->progmode = 0; // complete
						break;
					
					case 'M':
						progdata->progstate = message;
						keydata->local = 1;
						while(sendMess("E", codedata) == ERR_BUSY); // enter digit						
						break;
						
					case 'R':						
						progdata->progstate = param;
						keydata->local = 1;
						while(sendMess("RESET", codedata) == ERR_BUSY);	
						while(sendMess("E", codedata) == ERR_BUSY); // this makes sure previous message completes
						progdata->progmode = 0; // complete
						_EntryPoint();
						break;	
			
					case 'S':
						progdata->progstate = speed;
						progdata->newspeed = 0; // remove old speed
						keydata->local = 1;
						while(sendMess("E", codedata) == ERR_BUSY); // enter digit						
						break;
						
					case 'T':
						progdata->progstate = time;
						keydata->local = 1;
						while(sendMess("E", codedata) == ERR_BUSY); // enter digit						
						break;
						
					case 'U':
						keydata->key_out = 0;						
						keydata->local = 1;
						while(sendMess("E", codedata) == ERR_BUSY);						
						progdata->progstate = param;						
						progdata->progmode = 0; // complete
						break;
						
					case 'V':						
						progdata->progstate = param;
						keydata->local = 1;
						while(sendMess(VERSION, codedata) == ERR_BUSY);						
						progdata->progmode = 0; // complete						
						break;		
						
					case 'W':
						keydata->local = 1;
						while(sendChar(keydata->speed/10 + 48, codedata) == ERR_BUSY);
						while(sendChar(keydata->speed%10 + 48, codedata) == ERR_BUSY);				
						progdata->progstate = param;
						progdata->progmode = 0; // complete
						break;
						
					default:
						progdata->progstate = param;
						keydata->local = 1;
						while(sendMess("?", codedata) == ERR_BUSY);						
						progdata->progmode = 0; // complete
					
				}
				break;
			
				case speed:
			
					if(keydata->keybuff > '0' && keydata->keybuff <= '6') //keybuff speed in ASCII
					{
						progdata->newspeed = (keydata->keybuff - 48) * 10 ;
						progdata->progstate = speed0; // next digit
					}
					else
					{
						keydata->local = 1;
						while(sendMess("?", codedata) == ERR_BUSY); // error digit						
						progdata->progstate = param; // start over						
					}
					break;
			
				case speed0:
			
					if(keydata->keybuff  >= '0' && keydata->keybuff  <= '9')
					{
						progdata->newspeed += (keydata->keybuff - 48);
						keydata->speed = progdata->newspeed; // load new speed
						keydata->local = 1;
						while(sendMess("R", codedata) == ERR_BUSY);											
					}
					else
					{
						keydata->local = 1;
						while(sendMess("?", codedata) == ERR_BUSY); // error digit												
					}
					progdata->progstate = param; // next digit
					progdata->progmode = 0; // complet
					break;
			
				case message:
			
					if(keydata->keybuff >= '0' && keydata->keybuff <= '3')
					{
						keydata->local = 1;
						progdata->messageselect = keydata->keybuff - 48;
						keydata->local = 1;
						while(sendMess("M", codedata) == ERR_BUSY);
						while(sendChar(keydata->keybuff, codedata) == ERR_BUSY);
					}
					else
					{
						keydata->local = 1;
						while(sendMess("?", codedata) == ERR_BUSY); // error digit											
					}
					progdata->progstate = param; // next digit					
					break;
					
				case time:
					
					if(keydata->keybuff > '0' && keydata->keybuff <= '9')
					{
						progdata->btime = keydata->keybuff - 48;
						keydata->local = 1;
						while(sendMess("R", codedata) == ERR_BUSY);											
					}
					else
					{
						keydata->local = 1;
						while(sendMess("?", codedata) == ERR_BUSY); // error digit											
					}
					progdata->progstate = param; // next digit
					progdata->progmode = 0; // complet
					break;
					
				case freq:
					
					if(keydata->keybuff > '0' && keydata->keybuff <= '6')
					{
						progdata->freq = keydata->keybuff - 48;
						keydata->local = 1;
						while(sendMess("R", codedata) == ERR_BUSY);											
					}
					else
					{
						keydata->local = 1;
						while(sendMess("?", codedata) == ERR_BUSY); // error digit											
					}
					progdata->progstate = param; 
					progdata->progmode = 0; // complet					
					break;
		
		}		
	
}
