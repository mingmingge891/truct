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
#ifndef __ADP_STM32F10X_DELAY_H_
#define __ADP_STM32F10X_DELAY_H_ 			   

/**
  * @brief  ms??
  * @param  t (0x00-0xFFFFFFFF)
  * @retval : None
**/
extern void ADP_delay_ms(u32 time);

/**
  * @brief  us??
  * @param  t (0x00-0xFFFFFFFF)
  * @retval : None
**/
extern void ADP_delay_us(u32 time);

/**
  * @brief  void ADP_delay_init(void)
  * @param  None
  * @retval : None
**/
extern void ADP_delay_init(void);

#endif

