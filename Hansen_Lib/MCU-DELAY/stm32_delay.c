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
/* Includes ------------------------------------------------------------------*/
#include "stm32_delay.h"

#if USE_SYS_TICK_EN > 0

static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数	
static u16 i=0,j=0,k=0;

#endif

/**
  * @brief  This 延时初始化 
  * @param  None
  * @retval : None
  */
void delay_init(u8 SYSCLK)
{
#if USE_SYS_TICK_EN > 0
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8 1011
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
#else
	SYSCLK = SYSCLK;
#endif
}

#if STM32F407EN  >  0

/**
  * @brief  ms delay
  * @param  None
  * @retval : None
**/
void delay_ms(u32 time)
{
#if USE_SYS_TICK_EN > 0
	
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
  u16 i;
	while(time--)
	{
#if STM32F_180MHZ_EN > 0
		i=12000;
#endif 
		while(i--);
	}
#endif
}

/**
  * @brief  us delay
  * @param  None
  * @retval : None
**/
void delay_us(u32 time)
{
#if USE_SYS_TICK_EN > 0
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器
#else
  u16 i;
	while(time--)
	{
#if STM32F_180MHZ_EN > 0
		i=12;
#endif 
		while(i--);
	}
#endif
}

#endif


#if STM32F103EN  >  0

/**
  * @brief  ms delay
  * @param  None
  * @retval : None
**/
void delay_ms(u32 time)
{
#if USE_SYS_TICK_EN > 0
	
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
  u16 i;
	while(time--)
	{
#if STM32F_72MHZ_EN > 0
		i=12000;
#elif STM32F_48MHZ_EN >0
		i=12000;
#endif 
		while(i--);
	}
#endif
}

/**
  * @brief  us delay
  * @param  None
  * @retval : None
**/
void delay_us(u32 time)
{
#if USE_SYS_TICK_EN > 0
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器
#else
  u16 i;
	while(time--)
	{
#if STM32F_72MHZ_EN > 0
		i=12;
#elif STM32F_48MHZ_EN >0
		i=8;
#endif 
		while(i--);
	}
#endif
}

#endif
