/*
 * tim.h
 *
 *  Created on: May 6, 2016
 *      Author: Magu
 */

#ifndef TIM_H_
#define TIM_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"

#include "prog.h"
#include "key.h"
#include "code.h"
#include "Events.h"


typedef struct {
	
	word msTimer;
	word acTimer;
	byte secTimer;
	byte minTimer;
	//byte swTimer;
	
}timer;

void timerProcess(timer *timedata);

void txiohandler(keyer *keydata, coder *codedata);


#endif /* TIM_H_ */


