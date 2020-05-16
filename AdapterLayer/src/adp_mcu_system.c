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
#include "adp_mcu_system.h"

/**
  * @brief  中断配置
  * @param  None
  * @retval None
*/
void ADP_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 优先级组 说明了抢占优先级所用的位数，和子优先级所用的位数*/   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
#if SERIAL1_SUPPORT > 0	
	/*USART1*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			     	    //设置串口1中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SERIAL1_NVIC_IRQ_PP;	     	//抢占优先级 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SERIAL1_NVIC_IRQ_SP;						  //子优先级为
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								  //使能
	NVIC_Init(&NVIC_InitStructure);
#endif	
#if SERIAL2_SUPPORT > 0	
	/*USART2*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			     	    //设置串口2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SERIAL2_NVIC_IRQ_PP;	     	//抢占优先级 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SERIAL2_NVIC_IRQ_SP;						  //子优先级为
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								  //使能
	NVIC_Init(&NVIC_InitStructure);
#endif	
#if SERIAL3_SUPPORT > 0	
	/*USART3*/
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			     	    //设置串口3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SERIAL3_NVIC_IRQ_PP;	     	//抢占优先级 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = SERIAL3_NVIC_IRQ_SP;						  //子优先级为
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								  //使能
	NVIC_Init(&NVIC_InitStructure);
#endif	
#if TIME2_SUPPORT > 0
 	/*TIMER2*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	    						//TIM2 RX0中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIME2_NVIC_IRQ_PP;		    //抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIME2_NVIC_IRQ_SP;			        //子优先级为
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
#if TIME3_SUPPORT > 0
 	/*TIMER3*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	    						//TIM3  RX0中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIME3_NVIC_IRQ_PP;		   //抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIME3_NVIC_IRQ_SP;			       //子优先级为
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
#if TIME4_SUPPORT > 0
	/*TIMER4*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	    						//TIM4 RX0中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIME4_NVIC_IRQ_PP;		    //抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIME4_NVIC_IRQ_SP;			        //子优先级为
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
#if USB_SUPPORT > 0
	/*USB相关中断的配置*/
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USB_RX0_NVIC_IRQ_PP;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = USB_RX0_NVIC_IRQ_SP;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USB_TX_NVIC_IRQ_PP;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = USB_TX_NVIC_IRQ_SP;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif	
}

/**
  * @brief  GPIO 配置
  * @param  None
  * @retval None
*/
#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)
void ADP_GPIO_Configuration(void)
{

	/*JTAG IO ½ûÖ¹*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
}

/**
  * @brief  单片机初始化
  * @param  None
  * @retval : None
**/
void ADP_MCU_Init(void)
{	
	/*系统初始化*/
	SystemInit();
	
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
		
	/*中断优先级配置*/
	ADP_NVIC_Configuration();
	
	/*GPIP 初始化*/
	ADP_GPIO_Configuration();

}
/**
  * @brief 获取单片机UUID
  * @param
  * @retval: None
*/
u8* ADP_MCU_GetUUID(void)
{
	static u8 ID[12];
	static u32 UID_ADDR = 0x1FFFF7E8;
	for(u8 i=0;i<12;i++) {
		 ID[i] = (*(__IO u32 *)(UID_ADDR+(i/4)*4) >>((i%4)*8))&0x000000ff;
	}
	return ID;
}
/**
  * @brief  单片机复位
  * @param  None
  * @retval : None
**/
void ADP_MCU_Reset(void)
{
	
}

void MemManage_Handler(void)
{
	if (CoreDebug->DHCSR & 1) {  //check C_DEBUGEN == 1 -> Debugger Connected  
      __breakpoint(0);  // halt program execution here         
  }  
	while(1);
}

void HardFault_Handler(void)
{
	 if (CoreDebug->DHCSR & 1) {  //check C_DEBUGEN == 1 -> Debugger Connected  
      __breakpoint(0);  // halt program execution here         
  }  
	while(1);
}
