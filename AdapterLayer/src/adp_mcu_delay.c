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
#include "adp_mcu_system.h"
#include "adp_mcu_delay.h"
#include "string.h"
#include "stdio.h"



/*
		单片机的软件延时各类单片机的延时写法不同 下面是stm32f10x单片机的72MHZ的延时函数
*/
//#if OS_EN < 1	

//static u8  fac_us=0;
//static u16 fac_ms=0;
//static u16 i=0,j=0,k=0;

///**
//  * @brief  This function 
//  * @param  None
//  * @retval : None
//  */
//void delayMs(u16 nms)
//{	 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
//	SysTick->VAL =0x00;           //清空计数器
//	SysTick->CTRL=0x01 ;          //开始倒数  
//	do
//	{
//		temp=SysTick->CTRL;
//	}
// 	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
//	SysTick->CTRL=0x00;       //关闭计数器
//	SysTick->VAL =0X00;       //清空计数器	  	    
//} 
//#endif

/**
  * @brief  ms
  * @param  t (0x00-0xFFFFFFFF)
  * @retval : None
**/
void ADP_delay_ms(u32 nms)
{
#if 0	
	if(nms>500){  
		k = nms/500;
		j = nms%500;
		for(i=0;i<k;i++)
		{
				delayMs(500);
		}	
		if(j>0)
		delayMs(j);
	}
	else{
			delayMs(nms);
	}
#else
	 u16 i = 0;
	if(nms == 0x00)
		return;
	while(nms--)
	{
	#if SYSTEM_CLK_72MHZ >0
		i=12000;
	#elif SYSTEM_CLK_48MHZ >0
		i=9500;
	#endif
		while(i--);
	}
#endif
}
/**
  * @brief  us
  * @param  t (0x00-0xFFFFFFFF)
  * @retval : None
**/
void ADP_delay_us(u32 nus)
{
	#if 0	
		u32 temp;	    	 
		SysTick->LOAD =nus*fac_us; //时间加载
		SysTick->VAL  =0x00;        //清空计数器
		SysTick->CTRL =0x01 ;      //开始倒数
		do
		{
			temp=SysTick->CTRL;
		}
		while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
		SysTick->CTRL=0x00;       //关闭计数器
		SysTick->VAL =0X00;       //清空计数器
	#else
		 u16 i =0;
		if(nus == 0x00)
			return;
		while(nus--)
		{
	#if SYSTEM_CLK_72MHZ >0
		i=12;
	#elif SYSTEM_CLK_48MHZ >0
		i=9;
	#endif
			
			while(i--);
		}
	#endif
}

/**
  * @brief  void ADP_delay_init(void)
  * @param  None
  * @retval : None
**/
void ADP_delay_init(void)
{
//#if OS_EN < 1	
//	SysTick->CTRL&=0xfffffffb;
//	#if SYSTEM_CLK_72MHZ >0
//		fac_us=72/8;
//	#elif SYSTEM_CLK_48MHZ >0
//		fac_us=48/8;
//	#endif
//	
//	fac_ms=(u16)fac_us*1000;
//#endif
}

