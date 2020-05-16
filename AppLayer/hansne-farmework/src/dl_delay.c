/**
  ******************************************************************************
  * @file    dl_delay.c
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
#include "dl_delay.h"
#include "string.h"

/*说明:
	使用一个优先级最高的定时器中断来做滴答时钟判断延时函数		
			
	*/

/* Values---------------------------------------------------------------------*/

///**
//  * @brief  s延时
//  * @param  t (0x00-0xFFFFFFFF)
//  * @retval : None
//**/
//void delay_s(u32 t)
//{
//	delay_ms(t*1000);
//}

///**
//  * @brief  ms延时
//  * @param  t (0x00-0xFFFFFFFF)
//  * @retval : None
//**/
//void delay_ms(u32 t)
//{
//	ADP_delay_ms(t);
//}

///**
//  * @brief  us延时
//  * @param  t (0x00-0xFFFFFFFF)
//  * @retval : None
//**/
//void delay_us(u32 t)
//{
//	ADP_delay_us(t);
//}

/**
  * @brief  延时初始化
  * @param  None
  * @retval : None
**/
void delay_initialize(void)
{
	
	ADP_delay_init();
	
}
