/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
#ifndef __ADP_STM32F10X_EXIT_H_
#define __ADP_STM32F10X_EXIT_H_ 	

/** 
  * @brief  EXTI Trigger enumeration  
  */
typedef enum
{
	Trigger_Rising = 0,
  Trigger_Falling,  
  Trigger_Rising_Falling
}EXITTrigger_Mode;
/** 
  * @brief  EXTI Trigger struct  
  */
typedef struct
{
	EXITTrigger_Mode Event;
	u8 TriggerChannel;
}Exit_Event_t;

uint8_t ADP_attachInterrupt(u8 Pin,void (*ptr)(Exit_Event_t*));

void ADP_ExitInitializes(void);

#endif
