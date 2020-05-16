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
#include "stm32f10x_gpio.h"
#include "adp_mcu_gpio.h"
#include "string.h"
#include "stdio.h"

/*Gpio Map list Type define */
typedef struct
{
	GPIO_TypeDef* GPIO;
	uint16_t Pin;
}Stm32_Gpio_Map_t;

/*Gpio Map list Config GPIO Connect Logic to Physical*/
Stm32_Gpio_Map_t G_Map[] = 
{
	{GPIOA,GPIO_Pin_0},	/*logic pin 0*/
	{GPIOA,GPIO_Pin_1},	/*logic pin 1*/
	{GPIOA,GPIO_Pin_2},	/*logic pin 2*/
	{GPIOA,GPIO_Pin_3},	/*logic pin 3*/
	{GPIOA,GPIO_Pin_4},	/*logic pin 4*/
	{GPIOA,GPIO_Pin_5},	/*logic pin 5*/
	{GPIOA,GPIO_Pin_6},	/*logic pin 6*/
	{GPIOA,GPIO_Pin_7},	/*logic pin 7*/
	{GPIOA,GPIO_Pin_8},	/*logic pin 8*/
	{GPIOA,GPIO_Pin_9},	/*logic pin 9*/
	{GPIOA,GPIO_Pin_10},/*logic pin 10*/
	{GPIOA,GPIO_Pin_11},/*logic pin 11*/
	{GPIOA,GPIO_Pin_12},/*logic pin 12*/
	{GPIOA,GPIO_Pin_13},/*logic pin 13*/
	{GPIOA,GPIO_Pin_14},/*logic pin 14*/
	{GPIOA,GPIO_Pin_15},/*logic pin 15*/
	
	{GPIOB,GPIO_Pin_0},	/*logic pin 16*/
	{GPIOB,GPIO_Pin_1},	/*logic pin 17*/
	{GPIOB,GPIO_Pin_2},	/*logic pin 18*/
	{GPIOB,GPIO_Pin_3},	/*logic pin 19*/
	{GPIOB,GPIO_Pin_4},	/*logic pin 20*/
	{GPIOB,GPIO_Pin_5},	/*logic pin 21*/
	{GPIOB,GPIO_Pin_6},	/*logic pin 22*/
	{GPIOB,GPIO_Pin_7},	/*logic pin 23*/
	{GPIOB,GPIO_Pin_8},	/*logic pin 24*/
	{GPIOB,GPIO_Pin_9},	/*logic pin 25*/
	{GPIOB,GPIO_Pin_10},/*logic pin 26*/
	{GPIOB,GPIO_Pin_11},/*logic pin 27*/
	{GPIOB,GPIO_Pin_12},/*logic pin 28*/
	{GPIOB,GPIO_Pin_13},/*logic pin 29*/
	{GPIOB,GPIO_Pin_14},/*logic pin 30*/
	{GPIOB,GPIO_Pin_15},/*logic pin 31*/
};

/**
  * @brief  Sets or clears the selected data port bit.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  *   This parameter can be one of GPIO_Pin_x where x can be (0..15).
  * @param  BitVal: specifies the value to be written to the selected bit.
  *   This parameter can be one of the BitAction enum values:
  *     @arg Bit_RESET: to clear the port pin
  *     @arg Bit_SET: to set the port pin
  * @retval None
  */
void ADP_GPIO_WriteBit(uint16_t indexPin, uint8_t BitVal)
{	
	GPIO_WriteBit(G_Map[indexPin].GPIO,G_Map[indexPin].Pin,(BitAction)BitVal);
}

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @param  GPIO_Pin:  specifies the port bit to read.
  *   This parameter can be GPIO_Pin_x where x can be (0..15).
  * @retval The input port pin value.
  */
uint8_t ADP_GPIO_ReadInputDataBit(uint16_t indexPin)
{
	return GPIO_ReadInputDataBit(G_Map[indexPin].GPIO,G_Map[indexPin].Pin);
}

