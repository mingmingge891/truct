/**
  ******************************************************************************
  * @file    dl_delay.h
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
#ifndef __DL_DELAY_H_
#define __DL_DELAY_H_
#include "adapter.h"

///**
//  * @brief  ms??
//  * @param  t (0x00-0xFFFFFFFF)
//  * @retval : None
//**/
//void delay_ms(u32 t);

///**
//  * @brief  us??
//  * @param  t (0x00-0xFFFFFFFF)
//  * @retval : None
//**/
//void delay_us(u32 t);

///**
//  * @brief  s??
//  * @param  t (0x00-0xFFFFFFFF)
//  * @retval : None
//**/
//void delay_s(u32 t);

#define delay_ms(x)  ADP_delay_ms(x)
#define delay_us(x)  ADP_delay_us(x)
#define delay_s(x)   delay_ms(x*1000)

/**
  * @brief  ?????
  * @param  None
  * @retval : None
**/
void delay_initialize(void);

#endif
