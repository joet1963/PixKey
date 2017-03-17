/* ###################################################################
**     Filename    : Events.c
**     Project     : PixKey
**     Processor   : MC9S08QG8CPB
**     Component   : Events
**     Version     : Driver 01.02
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2017-02-28, 14:53, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.02
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

#include "code.h"
#include "key.h"
#include "prog.h"
#include "tim.h"

extern keyer internalkeyer;
extern timer mytimers;
extern coder mycodeprocess;
extern prog myprogprocess;
extern const byte npattern[];


/*
** ===================================================================
**     Event       :  KB1_OnInterrupt (module Events)
**
**     Component   :  KB1 [KBI]
**     Description :
**         This event is called when the active signal edge/level
**         occurs. This event is enabled only if <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void KB1_OnInterrupt(void)
{
  /* Write your code here ... */
	
	mycodeprocess.kinterrupt = 1;
	//myprogprocess.beacon = 0; // stop beacon	
}

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void)
{
  /* Write your code here ... */
	
	//internalkeyer.pshbtimer++;
	timerProcess(&mytimers);
	CODEprocess(&mycodeprocess, internalkeyer.speed);
	keysprocess(&internalkeyer, npattern, 47);	// was 47

}

/* END Events */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
