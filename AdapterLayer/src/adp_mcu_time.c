/**
  ******************************************************************************
  * @file    al_time.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    24-1-2019
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

/* Includes ------------------------------------------------------------------*/

#include "adp_mcu_system.h"
#include "adp_mcu_time.h"
#include "string.h"


/*
	定时器中断回调函数定义
*/
typedef struct
{
	void (*timer1CbSet)(void);
	void (*timer2CbSet)(void);
	void (*timer3CbSet)(void);
	void (*timer4CbSet)(void);
	void (*timer5CbSet)(void);
	void (*timer6CbSet)(void);
	void (*timer7CbSet)(void);
	void (*timer8CbSet)(void);
	void (*timer9CbSet)(void);
	void (*timer10CbSet)(void);
	void (*timer11CbSet)(void);
	void (*timer12CbSet)(void);
	
}timerIrqCb_t;

/* Values ------------------------------------------------------------------*/
timerIrqCb_t timerIrqCb;

/**
  * @brief  
  * @param  None
  * @retval : None
**/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)	//检测是否发生溢出更新事件
	{
		TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);	//清除TIM2的中断待处理位
		if(timerIrqCb.timer4CbSet != NULL)
			timerIrqCb.timer4CbSet();
	}
}
/**
  * @brief  
  * @param  None
  * @retval : None
**/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	//检测是否发生溢出更新事件
	{
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);	//清除TIM2的中断待处理位	
		if(timerIrqCb.timer2CbSet != NULL)
			timerIrqCb.timer2CbSet();
	}
}

/**
  * @brief  
  * @param  None
  * @retval : None
**/
void ADP_TIMER_Enable_TIMER2(void)
{
	TIM_Cmd(TIM2,ENABLE);
}
/**
  * @brief  
  * @param  None
  * @retval : None
**/
void ADP_TIMER_Enable_TIMER4(void)
{
	TIM_Cmd(TIM4,ENABLE);
}
/**
  * @brief  
  * @param  None
  * @retval : None
**/
void ADP_TIMER_DisEnable_TIMER2(void)
{
	TIM_Cmd(TIM2,DISABLE);
}
/**
  * @brief  
  * @param  None
  * @retval : None
**/
void ADP_TIMER_DisEnable_TIMER4(void)
{
	TIM_Cmd(TIM4,DISABLE);
}
/**
  * @brief
  * @param  None
  * @retval : None
**/
void ADP_TIMER_Config_TIMER2(u32 t,void (*ptr)(void))
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	timerIrqCb.timer2CbSet = ptr;

	/*开启定时器的时钟*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	/*定时器2*/
	TIM_DeInit(TIM2);
	TIM_InternalClockConfig(TIM2);
	#if SYSTEM_CLK_72MHZ >0
		TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1; //预分频系数为24000-1，这样计数器时钟为48MHz/24000 = 2kHz
	#elif SYSTEM_CLK_48MHZ>0
		TIM_TimeBaseStructure.TIM_Prescaler = 24000 - 1; //预分频系数为24000-1，这样计数器时钟为48MHz/24000 = 2kHz
	#endif
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 2 - 1;//设置计数溢出大小，每计2000个数就产生一个更新事件1ms
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM2, DISABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
}

/**
  * @brief  打开定时器4
  * @param  None
  * @retval : None
**/
void ADP_TIMER_Config_TIMER4(u32 t,void (*ptr)(void))
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	timerIrqCb.timer4CbSet = ptr;
	
	/*开启定时器的时钟*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	/*定时器2*/
	TIM_DeInit(TIM4);
	TIM_InternalClockConfig(TIM4);
	#if SYSTEM_CLK_72MHZ >0
		TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1; //预分频系数为24000-1，这样计数器时钟为48MHz/24000 = 2kHz
	#elif SYSTEM_CLK_48MHZ >0
		TIM_TimeBaseStructure.TIM_Prescaler = 24000 - 1; //预分频系数为24000-1，这样计数器时钟为48MHz/24000 = 2kHz
	#endif
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 2 - 1;//设置计数溢出大小，每计2000个数就产生一个更新事件1ms
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM4,DISABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
}
