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
#ifndef __ADP_STM32F10X_UART_H_
#define __ADP_STM32F10X_UART_H_ 			   


/**
  * @brief  发送一个字节
  * @param  None
  * @retval : None
  */
void ADP_USART_SendChar(u8 channel,u8 Data);

/**
  * @brief  USART Init
  * @param  None
  * @retval : None
  */
void ADP_USART_Init(u8 channel);

/**
  * @brief  单片机串口配置
  * @param  None
  * @retval : None
  */
void ADP_USART_Config(u8 channel,uint32_t USART_BaudRate,u32 DataBits, u32 Parity, u32 StopBits);

/**
  * @brief  设置中断回调函数
  * @param  None
  * @retval : None
  */
void ADP_USART_SetIrqCb(u8 channel,void(*ptr)(u8));

/**
  * @brief  单片机串口中断配置
  * @param  None
  * @retval : None
  */
void ADP_USART_IT_Config(u8 channel);

#endif

