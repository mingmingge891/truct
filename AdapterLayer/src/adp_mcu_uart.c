/**
  ******************************************************************************
  * @file    xxx.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    24-1-2019
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "adp_mcu_uart.h"
#include "string.h"
#include "stdio.h"


/*中断回调函数*/
typedef struct 
{
	void(*usart1SetCb)(u8);
	void(*usart2SetCb)(u8);
	void(*usart3SetCb)(u8);
	void(*usart4SetCb)(u8);
}usartSetIrqCb_t;

usartSetIrqCb_t usartSetIrqCb;

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval : None
  */
void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)
	{

		USART_ClearFlag(USART1, USART_FLAG_PE);
				USART_ReceiveData(USART1);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET){

		USART_ClearFlag(USART1, USART_FLAG_ORE);
				USART_ReceiveData(USART1);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
	{

		USART_ClearFlag(USART1, USART_FLAG_FE);
				USART_ReceiveData(USART1);
	}
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		if(usartSetIrqCb.usart1SetCb != NULL)
			usartSetIrqCb.usart1SetCb(USART_ReceiveData(USART1));
	}
}
/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval : None
  */
void USART2_IRQHandler(void)
{	
	if (USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)
	{

		USART_ClearFlag(USART2, USART_FLAG_PE);
				USART_ReceiveData(USART2);
	}
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET){

		USART_ClearFlag(USART2, USART_FLAG_ORE);
				USART_ReceiveData(USART2);
	}
	if (USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)
	{

		USART_ClearFlag(USART2, USART_FLAG_FE);
				USART_ReceiveData(USART2);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		if(usartSetIrqCb.usart2SetCb != NULL)
			usartSetIrqCb.usart2SetCb(USART_ReceiveData(USART2));
	}
}
/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval : None
  */
void USART3_IRQHandler(void)
{	
	if (USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)
	{

		USART_ClearFlag(USART3, USART_FLAG_PE);
				USART_ReceiveData(USART3);
	}
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET){

		USART_ClearFlag(USART3, USART_FLAG_ORE);
				USART_ReceiveData(USART3);
	}
	if (USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)
	{

		USART_ClearFlag(USART3, USART_FLAG_FE);
				USART_ReceiveData(USART3);
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		if(usartSetIrqCb.usart3SetCb != NULL)
			usartSetIrqCb.usart3SetCb(USART_ReceiveData(USART3));
	}
}

/**
  * @brief  单片机串口中断配置
  * @param  None
  * @retval : None
  */
void ADP_USART_IT_Config(u8 channel)
{
	USART_TypeDef* USARTx;
	switch(channel)
	{
		case 1:
			USARTx = USART1;
			break;
		case 2:
			USARTx = USART2;
			break;
		case 3:
			USARTx = USART3;
			break;
	}
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
	USART_ITConfig(USARTx, USART_IT_PE, DISABLE);
	USART_ITConfig(USARTx, USART_IT_ERR, DISABLE);
}
/**
  * @brief  单片机串口配置
  * @param  None
  * @retval : None
  */
void ADP_USART_Config(u8 channel,uint32_t USART_BaudRate,u32 DataBits, u32 Parity, u32 StopBits)
{
	USART_InitTypeDef USART_InitStructure;
	USART_TypeDef* USARTx;
	
  USART_InitStructure.USART_BaudRate = USART_BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	switch(channel)
	{
		case 1:
			USARTx = USART1;
			break;
		case 2:
			USARTx = USART2;
			break;
		case 3:
			USARTx = USART3;
			break;
	}
  USART_Init(USARTx, &USART_InitStructure);
	
  /* Enable the USARTx */
  USART_Cmd(USARTx, ENABLE);
}

/**
  * @brief  发送一个字节
  * @param  None
  * @retval : None
  */
void ADP_USART_SendChar(u8 channel,u8 Data)
{
	switch(channel)
	{
		case 1:
				USART_SendData(USART1,Data);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
			break;
		case 2:
				USART_SendData(USART2,Data);
				while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
			break;
		case 3:
			USART_SendData(USART3,Data);
				while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
			break;
	}
}


/**
  * @brief  设置中断回调函数
  * @param  None
  * @retval : None
  */
void ADP_USART_SetIrqCb(u8 channel,void(*ptr)(u8))
{
	if(channel == 1)
	{
		usartSetIrqCb.usart1SetCb = ptr;
	}
	if(channel == 2)
	{
		usartSetIrqCb.usart2SetCb = ptr;
	}
	if(channel == 3)
	{
		usartSetIrqCb.usart3SetCb = ptr;
	}
	if(channel == 4)
	{
		usartSetIrqCb.usart4SetCb = ptr;
	}
}

/**
  * @brief  USART Init
  * @param  None
  * @retval : None
  */
void ADP_USART_Init(u8 channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	switch(channel)
	{
		case 1:
			/*初始化串口一时钟*/
			RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO  , ENABLE);
		
			/*串口一配置 TX*/		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			/*串口一配置 RX*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		break;
		case 2:	
			/*初始化串口二时钟*/
			RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		
			/*串口二配置 TX*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
			/*串口二配置 RX*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		break;
		case 3:
			
			/*初始化串口三时钟*/
			RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		
			/*串口三配置 TX*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			/*串口三配置 RX*/
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		break;
	}
}

