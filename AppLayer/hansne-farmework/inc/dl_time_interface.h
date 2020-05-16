/**
  ******************************************************************************
  * @file    dl_time.h
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
#ifndef __DL_INTERFACE_H_
#define __DL_INTERFACE_H_

#include "adapter.h"

/*定义定时器操作必要的抽象函数或者参数，
实现需要用户在用户层实现*/
typedef struct
{
	void (*Init)(u8 timeIndex,u8 BaseTimeCounter);       /*! Initialize the timer */
	void (*Timer_CMD)(u8 timeIndex,u8 cmd);              /*! Timer CMD */
	void (*RegisterTimerIrq)(void (*ptr)(u8 hal_time_id));
	u8 Hal_time_num;
}TIMER_Hal_Interface_t;

extern TIMER_Hal_Interface_t Implement_Timer_HAL_Interface;

#endif
