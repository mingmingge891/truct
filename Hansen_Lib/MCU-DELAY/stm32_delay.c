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

static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������	
static u16 i=0,j=0,k=0;

#endif

/**
  * @brief  This ��ʱ��ʼ�� 
  * @param  None
  * @retval : None
  */
void delay_init(u8 SYSCLK)
{
#if USE_SYS_TICK_EN > 0
	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8 1011
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
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����
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
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����
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
