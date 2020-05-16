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

#include "hal_init.h"

/*stm32f103*/
#define STM32F103EN  														0
#define STM32F_72MHZ_EN                         0
#define STM32F_48MHZ_EN                         0

/*stm32f407*/
#define STM32F407EN  														1
#define STM32F_180MHZ_EN                        1

#define USE_SYS_TICK_EN                         0

/**
  * @brief  ms delay
  * @param  None
  * @retval : None
**/
void delay_ms(u32 time);

/**
  * @brief  us delay
  * @param  None
  * @retval : None
**/
void delay_us(u32 time);

/**
  * @brief  This —” ±≥ı ºªØ 
  * @param  None
  * @retval : None
  */
void delay_init(u8 SYSCLK);
	
