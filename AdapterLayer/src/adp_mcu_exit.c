/**
  ******************************************************************************
  * @file    xxx.c
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
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "adp_mcu_exit.h"
#include "adp_mcu_gpio.h"
#include "misc.h"
#include "string.h"

/*最大支持的外部中断个数*/
#define MAX_SUPPORT_NUM     32


/*中断回调函数*/
typedef struct 
{
	/*回调函数*/
	void (*irqCall)(Exit_Event_t*);
}exitSetIrqCb_t;

exitSetIrqCb_t exitSetIrqCb[MAX_SUPPORT_NUM];
Exit_Event_t Exit_Event;

/*硬件外部中断回调函数*/
void xEXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
		
		/*PB15*/
		#define PB15_TRIGGER_CHANNEL  31
		if(ADP_GPIO_ReadInputDataBit(PB15_TRIGGER_CHANNEL)){
			Exit_Event.Event = Trigger_Rising;
		}
		else{
			Exit_Event.Event = Trigger_Falling;
		}
		if(exitSetIrqCb[PB15_TRIGGER_CHANNEL].irqCall != NULL){
			Exit_Event.TriggerChannel = PB15_TRIGGER_CHANNEL;
			exitSetIrqCb[PB15_TRIGGER_CHANNEL].irqCall(&Exit_Event);
		}
	}
}

/**
  * @brief  初始化
  * @param  None
  * @retval : None
  */
void ADP_McuGpioExitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/*中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	
	/* EXTI line gpio config(XX) 	 °´¼ü¼ì²â*/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line(XX) mode config  ÉÏÉýÏÂ½µÑØ*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}
/**
  * @brief  这是对应用层提供的API操作函数 支持中断的回调函数
						IO口的电平触发类型通过配置统一设置好 根据G_Map
						查看中断
  * @param  None
  * @retval : None
  */
uint8_t ADP_attachInterrupt(u8 Pin,void (*ptr)(Exit_Event_t*))
{
	if(Pin < MAX_SUPPORT_NUM)
	{
		/*获取函数指针*/
		if(exitSetIrqCb[Pin].irqCall == NULL){
			exitSetIrqCb[Pin].irqCall = ptr;
		}
		return 1;
	}
	return 0;
}

/**
  * @brief  外部中断初始化
  * @param  None
  * @retval : None
  */
void ADP_ExitInitializes(void)
{
	/*首先对需要配置的IO引脚进行配置*/
	ADP_McuGpioExitConfig();
}
