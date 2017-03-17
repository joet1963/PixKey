/*
 * code.h
 *
 *  Created on: Apr 22, 2016
 *      Author: Magu
 */

#ifndef CODE_H_
#define CODE_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "key.h"


#define AR  0x03
#define VA  0x04
#define BT  0x16
#define BK  0x10
#define KN  0x17
#define AS  0x1B
#define AA  0x08


#define _ar  0b01010100
#define _va  0b00010110
#define _bt  0b10001100
#define _bk  0b10001011  // DLE 0x10
#define _kn  0b10110100  //
#define _as  0b01000100  //
#define _aa  0b01011000  // LF 0x0A


typedef enum {
	
	initCHAR = 0,
	outCHAR,
	nextCHAR,
	doneMESS	
	
}States;

typedef struct {
	
	volatile byte shiftchar;
	volatile word MARKtimer;
	volatile word SPACEtimer;
	enum States State;
	byte CHbuffer[2];
	const unsigned char *codemesspoint;
	bool key_out;
	bool kinterrupt;
} coder ;


extern const byte npattern[];

byte sendMess(const unsigned char *mess, coder *codedata);
byte sendChar(const unsigned char c, coder *codedata);
void CODEprocess(coder *codedata, byte speed);



#endif /* CODE_H_ */
