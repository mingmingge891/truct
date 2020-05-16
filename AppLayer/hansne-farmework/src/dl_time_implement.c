/**
  ******************************************************************************
  * @file    dl_time_implement.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    24-1-2019
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.twc.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2019 HansenTech</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dl_time_interface.h"
#include "string.h"

typedef void (*HalTimeCallbackTyp)(u8 hal_time_id);

HalTimeCallbackTyp HalTimeCallback;
/**
  * @brief 具体的实现
  * @param  None
  * @retval : None
**/
static void timer1CallBack(void)
{
	if(HalTimeCallback != NULL)
	{
		HalTimeCallback(1);
	}
}
static void timer2CallBack(void)
{
	if(HalTimeCallback != NULL)
	{
		HalTimeCallback(2);
	}
}

static void HalTimerInitialize(u8 timeIndex,u8 BaseTimeCounter)
{
	if(timeIndex == 1)
	{
		ADP_TIMER_Config_TIMER2(BaseTimeCounter,timer1CallBack);
	}
	if(timeIndex == 2)
	{
		//ADP_TIMER_Config_TIMER4(BaseTimeCounter,timer2CallBack);
	}
}

static void HalTimer_CMD(u8 index,u8 state)
{
	switch(index)
	{
		case 1:
			if(state == 1)
				ADP_TIMER_Enable_TIMER2();
			if(state == 0)
				ADP_TIMER_DisEnable_TIMER2();
			break;
		case 2:
			if(state == 1)
				ADP_TIMER_Enable_TIMER4();
			if(state == 0)
				ADP_TIMER_DisEnable_TIMER4();
			break;
	}
}

static void Hal_RegisterTimerIrq(void (*ptr)(u8 hal_time_id))
{
	HalTimeCallback = ptr;
}

/*实现*/
TIMER_Hal_Interface_t Implement_Timer_HAL_Interface = {
	.Init = HalTimerInitialize,
	.Timer_CMD = HalTimer_CMD,
	.RegisterTimerIrq = Hal_RegisterTimerIrq,
	.Hal_time_num = 2,
};
