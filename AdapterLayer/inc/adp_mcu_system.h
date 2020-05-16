/**
  ******************************************************************************
  * @file    
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
#ifndef __ADP_MCU_SYSTEM_H_
#define __ADP_MCU_SYSTEM_H_

#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_flash.h"
#ifdef CUSTOM_CONFIG
	#include "custom_config.h"
#endif
#include "misc.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)  MEM_ADDR( BITBAND(addr, bitnum)  )
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C
#define GPIOA_IDR_Addr    (GPIOA_BASE+8)  //0x40010808
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  //0x40010C08
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  //0x40011008
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)  //0x40011408
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)  //0x40011808


#define PA(x) 							BIT_ADDR(GPIOA_ODR_Addr, x)
#define PB(x) 							BIT_ADDR(GPIOB_ODR_Addr, x)


/*---------------------------CPUÏà¹ØÅäÖÃ-------------------------------*/
#define SYSTEM_CLK_72MHZ        1
#define SYSTEM_CLK_48MHZ        0
#define OS_EN                   0


void ADP_MCU_Init(void);
void ADP_MCU_Reset(void);
u8*  ADP_MCU_GetUUID(void);

#endif
