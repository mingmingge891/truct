/**
  ******************************************************************************
  * @file    led_machine.c
  * @author  hansen.jiang 951868692@qq.com
  * @version 1.0
  * @date    2018/11/29
  * @brief   
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "led_machine.h"
#include "timerEX.h"
#include "string.h"
#include "stdio.h"

/*led 控制*/
LedCtl_t LedCtl;

/*定时器抽象函数结构体变量*/
LED_Machine_PROP_T* pLED_MachineProperty;

/* function definition */
static void colorManagement(COLOR_U color);
static void led_handler(void* p_arg);

/**
  * @brief 颜色管理
  * @param  None
  * @retval : None
**/
static void colorManagement(COLOR_U color)
{
	switch(color)
	{
		case BLACK:  HAL_LED_R(0);HAL_LED_G(0);HAL_LED_B(0);
			break;
		case WHITE:  HAL_LED_R(1);HAL_LED_G(1);HAL_LED_B(1);
			break;
		case GREEN:  HAL_LED_R(0);HAL_LED_G(1);HAL_LED_B(0);
			break;
		case BLUE:   HAL_LED_R(0);HAL_LED_G(0);HAL_LED_B(1);
			break;
		case PURPLE: HAL_LED_R(1);HAL_LED_G(0);HAL_LED_B(1);
			break;
		case RED:    HAL_LED_R(1);HAL_LED_G(0);HAL_LED_B(0);
			break;
		default:
			break;
	}
}
/**
  * @brief static void led_handler(void* p_arg)
  * @param  None
  * @retval : None
**/
static void led_handler(void* p_arg)
{
	LedCtl_t *p = (LedCtl_t*)p_arg;
	
	if(p->num > 0){
		static uint16_t cnt = 0;
		if(++cnt == (p->num*2)){
			cnt = 0;
			colorManagement(BLACK);
			LED_MachineTimerStop(pLED_MachineProperty->InternelTimerPriority);
			return ;
		}
	}
	if(p->openflag == 0x01)
	{
		colorManagement(BLACK);
		LedCtl.openflag = 0x00;
		LED_MachineTimerStart(pLED_MachineProperty->InternelTimerPriority,(LedCtl_t*)&LedCtl,led_handler,LedCtl.close_timer);
	}
	else if(p->openflag == 0x00)
	{
		colorManagement(LedCtl.color);
		LedCtl.openflag = 0x01;
		LED_MachineTimerStart(pLED_MachineProperty->InternelTimerPriority,(LedCtl_t*)&LedCtl,led_handler,LedCtl.open_timer);
	}
	else if(p->openflag == 0x02)
	{
		colorManagement(BLACK);
		LED_MachineTimerStop(pLED_MachineProperty->InternelTimerPriority);
	}
}

/**
  * @brief Machine_LED_SetFlashing
  * @param  None
  * @retval : None
**/
void Machine_LED_SetFlashing(COLOR_U color ,uint16_t openTimer,uint16_t closeTimer)
{
	if(color == NONE)
		return;
	LedCtl.num = 0;
	LedCtl.color = color;
	LedCtl.open_timer = openTimer;
	LedCtl.close_timer = closeTimer;
	LedCtl.openflag = 0x01;
	colorManagement(color);
	LED_MachineTimerStart(pLED_MachineProperty->InternelTimerPriority,(LedCtl_t*)&LedCtl,led_handler,openTimer);
}

/**
  * @brief Machine_LED_SetFlickerA
  * @param  None
  * @retval : None
**/
void Machine_LED_SetFlashingA(COLOR_U color ,uint16_t openTimer,uint16_t closeTimer,uint8_t num)
{
	if(color == NONE)
		return;
	LedCtl.num = num;
	LedCtl.color = color;
	LedCtl.open_timer = openTimer;
	LedCtl.close_timer = closeTimer;
	LedCtl.openflag = 0x01;
	colorManagement(color);
	LED_MachineTimerStart(pLED_MachineProperty->InternelTimerPriority,(LedCtl_t*)&LedCtl,led_handler,openTimer);
}

/**
  * @brief Machine_LED_SetValue
  * @param  None
  * @retval : None
**/
void Machine_LED_SetValue(COLOR_U color ,uint8_t value)
{
	if(color == NONE)
		return;
	LED_MachineTimerStop(pLED_MachineProperty->InternelTimerPriority);
	if(value == 0)
	{
		colorManagement(BLACK);
		return;
	}
	colorManagement(color);
}
/**
  * @brief Machine_LED_SetA(COLOR_U color ,uint16_t value)
  * @param  None
  * @retval : None
**/
void Machine_LED_SetA(COLOR_U color ,uint16_t value)
{
	if(color == NONE)
		return;
	colorManagement(color);
	LedCtl.openflag = 0x02;
	LED_MachineTimerStart(pLED_MachineProperty->InternelTimerPriority,(LedCtl_t*)&LedCtl,led_handler,value);
}

/**
  * @brief  void Machine_LED_NEW(Machine_LED_Abstract_t* p)
  * @param  None
  * @retval : None
**/
void Machine_LED_NEW(Machine_LED_Abstract_t* p)
{
	p->SetFlashing =   Machine_LED_SetFlashing;
	p->SetFlashingA =  Machine_LED_SetFlashingA;
	p->SetValue =     Machine_LED_SetValue;
	p->SetValueA =    Machine_LED_SetA;
}

/**
  * @brief LED_MachineInitializes
  * @param  None
  * @retval : None
**/
void Machine_LEDInitializes(void)
{
	pLED_MachineProperty = &Implement_LED_Machine_Property;
	
}
