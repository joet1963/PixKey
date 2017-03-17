/*
 * key.h
 *
 *  Created on: Apr 22, 2016
 *      Author: Magu
 */

#ifndef KEY_H_
#define KEY_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "AD1.h"
#include "Events.h"
#include "code.h"


typedef struct
{
	bool dit_sw;    // paddle
	bool dah_sw;    // paddle
	bool dit_flag;  // iambic flag
	bool key_out;   // keying line
	bool new_char;  // new char in keyer input buffer
	bool local;     // local, no keying just sidetone 
	bool prog_sw;	// state of program switch
	bool T;         // T 1 = TX or T 0 = RX ?
	bool mkey;      // program switch is manual key 	
	word chartimer; // char out after n time
	word keytimer;  // element timing
	word pshbtimer; // push duration timer 
	word txdelay;   // transmit delay	
	byte key_char;  // keyer char buffer for parse method
	byte speed;     // code speed WPM
	byte keybuff;	// keyer char buff
	enum keystates keystate;   // keyer state machine "states"
	enum swFunctions function; // headset functions
	enum pshFunctions push;    // prog switch
	
}keyer;

typedef enum 
{
	idle = 0,
	dit,
	dah,
	send_dit,
	send_dah,
	wait_dit,
	wait_dah,
	pause,
	char_out,
	iambic
	
}keystates;

typedef enum
{
	noFunc = 0,
	FuncA,
	FuncB,
	FuncC,
	FuncD
	
}swFunctions;

typedef enum
{
	noPsh = 0,
	Psh1,
	Psh2,
	Psh3
	
}pshFunctions;

// prototypes

void keyiohandler(keyer *keydata);

void initcmdbuff(void);
void keysprocess(keyer *keydata, const byte lookuptable[], byte tablesize);

#endif /* KEY_H_ */
