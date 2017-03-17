/* ###################################################################
**     Filename    : main.c
**     Project     : PixKey
**     Processor   : MC9S08QG8CPB
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2017-02-28, 14:53, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Bit1.h"
#include "PWM1.h"
#include "KB1.h"
#include "AD1.h"
#include "TI1.h"
#include "PWM2.h"
#include "Bit2.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */


#include "key.h"
#include "code.h"
#include "mess.h"
#include "prog.h"
#include "tim.h"


timer mytimers;
keyer internalkeyer;
coder mycodeprocess;
coder locodeprocess;
prog  myprogprocess;



void main(void)
{
  /* Write your local variable definition here */	
	

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  
   
  
  	  myprogprocess.newspeed = 0;
	  myprogprocess.messageselect = 1;	   
	  myprogprocess.progstate = param;
	  myprogprocess.progmode = 0;
	  
	  mytimers.msTimer = 0;
	  mytimers.acTimer = 0;
	  mytimers.secTimer = 0;
	  mytimers.minTimer = 0;	  
	  
	  mycodeprocess.MARKtimer = 0;
	  mycodeprocess.SPACEtimer = 0;
	  mycodeprocess.State = doneMESS;  	
	  mycodeprocess.key_out = 0;
	  mycodeprocess.kinterrupt = 0;  
	  
	  internalkeyer.dit_flag = 0;
	  internalkeyer.key_out = 0;
	  internalkeyer.new_char = 0;
	  internalkeyer.local = 1;  
	  internalkeyer.chartimer = 0;
	  internalkeyer.keytimer = 0;	    		
	  internalkeyer.keystate = idle;
	  internalkeyer.push = noPsh;
	  internalkeyer.mkey = 0; // keyer by default
	  internalkeyer.speed = 20; // send startup messges fast
	  internalkeyer.local = 1;	  
	  
	  if(Cpu_GetLowVoltageWarningFlag())
	  {		  
		  while(sendMess("B", &mycodeprocess) == ERR_BUSY);		  
		  Cpu_ClearLowVoltageWarningFlag();
	  }
	  else while(sendMess("R", &mycodeprocess) == ERR_BUSY);  
	  
	  if(KB1_GetVal() & 0x04)
	  {
		  internalkeyer.mkey = 1; // manual 
		  while(sendMess("M", &mycodeprocess) == ERR_BUSY);
	  }
	  
	  internalkeyer.speed = 15; // restore to default speed
	  internalkeyer.local = 0;
	    
	  for(;;)
	  {			
		keyiohandler(&internalkeyer);
		txiohandler(&internalkeyer, &mycodeprocess);	
		Prog(&internalkeyer, &myprogprocess, &mycodeprocess);
		Mem(&internalkeyer, &myprogprocess, &mycodeprocess );		
		  
	  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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
