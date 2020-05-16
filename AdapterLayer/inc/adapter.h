/**
  ******************************************************************************
  * @file    adapter.h
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
#ifndef __ADAPTER_H_
#define __ADAPTER_H_

typedef unsigned char  uint8_t;              /* Unsigned  8 bit quantity                           */
typedef 	 			 char  int8;                 /* Signed    8 bit quantity                           */
typedef unsigned short uint16_t;             /* Unsigned 16 bit quantity                           */
typedef          short int16;                /* Signed   16 bit quantity                           */
typedef unsigned int   uint32_t;             /* Unsigned 32 bit quantity                           */
typedef 	       int   int32;          			 /* Signed   32 bit quantity                           */
typedef float          fp32;                 /* Single precision floating point                    */
typedef double         fp64;                 /* Double precision floating point                    */
#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t


#include "adp_mcu_time.h"
#include "adp_mcu_uart.h"
#include "adp_mcu_delay.h"
#include "adp_mcu_system.h"
#include "malloc.h"


#endif
