/*
 * prog.h
 *
 *  Created on: May 4, 2016
 *      Author: Magu
 */

#ifndef PROG_H_
#define PROG_H_

#define VERSION "VERSION 1.0"

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"

#include "Cpu.h"
#include "key.h"
#include "code.h"


typedef enum{
	
	param = 0,	
	speed,
	speed0,
	message,
	time,
	freq
		
}progstates;

typedef struct{	
		
	bool progmode;
	bool progentry;
	byte btime;
	byte messageselect;
	byte newspeed;
	byte freq;
	enum progstates progstate;
	
}prog;

void Prog(keyer *keydata, prog *progdata, coder *codedata);

#endif /* PROG_H_ */
