/**
  ******************************************************************************
  * @file    timerEX.h
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    24-September-2018
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.wisdudu.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2016 WIS-DUDU</center></h2>
  ******************************************************************************
  */
#ifndef __ADP_STM32F10X_TIME_H_
#define __ADP_STM32F10X_TIME_H_


void ADP_TIMER_Config_TIMER2(u32 t,void (*ptr)(void));
void ADP_TIMER_Config_TIMER4(u32 t,void (*ptr)(void));
void ADP_TIMER_Enable_TIMER2(void);
void ADP_TIMER_Enable_TIMER4(void);
void ADP_TIMER_DisEnable_TIMER2(void);
void ADP_TIMER_DisEnable_TIMER4(void);

#endif
