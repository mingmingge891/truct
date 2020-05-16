/**
  ******************************************************************************
  * @file    hs_power.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V0.0.0
  * @date    28-September-2017
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.tfc.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2017 HANSEN</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "string.h"


/* Instruction ----------------------------------------------------------------*/


/* Macro ---------------------------------------------------------------------*/
#define  KEY1_GPIO   (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) & 0x01)

/*长安开机时间 100ms 单位*/
#define  BOOT_TIMER  			10

/*长安关机时间100ms 单位*/
#define  SHUTDOWN_TIMER  	10

/*按键消除抖动延时函数可以自行实现*/
#define  KEY_ELIMINATE_JITTER  delay_ms(20)

/*电源开机*/
#define  POWER_ON

/*电源关机*/
#define  POWER_OFF

/**
  * @brief  	This function power_check
  * @param  	None
  * @retval : None
**/
void power_check_on(void)
{
		static uint8_t pk_downCnt = 0;
		while(1)
		{
				if(!KEY1_GPIO)
				{
						KEY_ELIMINATE_JITTER;//按键消抖动					  
						if(!KEY1_GPIO)
						{	
								/*是否松开按键*/
								while(!KEY1_GPIO)
								{
										delay_ms(100);
										if(++pk_downCnt == BOOT_TIMER)
										{
												pk_downCnt = 0;
												POWER_ON;
												return;
										}
								} 
								pk_downCnt = 0;
						}
				}
		}
}

/**
  * @brief  	This function power_check
  * @param  	None
  * @retval : None
**/
void power_check_off(void)
{
		static uint8_t pk_downCnt = 0;
		while(1)
		{
				if(!KEY1_GPIO)
				{
						KEY_ELIMINATE_JITTER;//按键消抖动					  
						if(!KEY1_GPIO)
						{	
								
								/*是否松开按键*/
								while(!KEY1_GPIO)
								{
										delay_ms(100);
										if(++pk_downCnt == BOOT_TIMER)
										{
												pk_downCnt = 0;
												POWER_OFF;
												return;
										}
								}
								pk_downCnt = 0;
						}
				}
		}
}