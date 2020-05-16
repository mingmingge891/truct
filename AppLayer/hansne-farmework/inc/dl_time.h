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
#ifndef __DL_TIMER_H_
#define __DL_TIMER_H_
#include "adapter.h"

//硬件定时器的个数  2个
#define timerNew(x)              AlltimerNew(1,x)
#define sys_timerNew(x)          AlltimerNew(2,x)

typedef struct
{
	void (*t_callBack)(void*);//回调函数
	uint32_t time_out;//定时器溢出
	uint32_t time_count;//定时器的计数
	void *   prg;//定时器参数传递
}timerEx_callback_t;

//定时器实例
typedef struct
{
	timerEx_callback_t *timerEx_call;
	u8 timerEx_Cnt;
}timerEx_Instance_t;
//API接口
typedef struct
{
	uint8_t (*start)(timerEx_Instance_t* instance,void* p_arg,void (*ptr)(void*),uint32_t args,...);
	void (*stop)(void *args,...);
	timerEx_Instance_t* instance;
}timer_t;

timer_t *AlltimerNew(u8 HalId,u8 time_cnt);
void init_timerEX(void);

#endif
