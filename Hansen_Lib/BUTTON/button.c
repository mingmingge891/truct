/**
  ******************************************************************************
  * @file    button.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V0.0.0
  * @date    28-September-2017
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
#include "button.h"
#include "string.h"

/* Macro ---------------------------------------------------------------------*/
#define  KEY1_GPIO   (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3) & 0x01)
#define  KEY2_GPIO   (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) & 0x01)

/*按键消除抖动延时函数可以自行实现*/
#define  KEY_ELIMINATE_JITTER  delay_ms(20)

/* Value ---------------------------------------------------------------------*/
/*按键消息事件变量*/
key_message_t key_event;

/*按键回调函数变量*/
callback_keyMesaage key_message;
/* 说明 ----------------------------------------------------------------------*/
/*当前写了两个按键要实现多个按键可以自己改源代码，单片机中断要能实现上升下降沿同时
进行*/

/**
  * @brief    注册事件回调函数
  * @param    函数指针参数类型button_message_t
  * @retval : None
**/
void register_key_callback(void (*ptr)(key_message_t*))
{
		if(ptr != NULL){
				key_message = ptr;
		}
}
/**
  * @brief  KEY1按键处理
  * @param  None
  * @retval : None
**/
static void key1_handler(void)
{
		key_event.index = KEY1;
		if(!KEY1_GPIO)
	   {
				/*相应的按键按下*/
				key_event.event = KEY_DOWN;
		}
		else if(KEY1_GPIO && key_event.event == KEY_DOWN){
				/*相应的按键抬起*/
				key_event.event = KEY_UP;
		}
}
/**
  * @brief  KEY2按键处理
  * @param  None
  * @retval : None
**/
static void key2_handler(void)
{
		key_event.index = KEY2;
		if(KEY2_GPIO)
		{
				/*相应的按键按下*/
				key_event.event = KEY_DOWN;
		}
		else if(KEY2_GPIO && key_event.event == KEY_DOWN){
				/*相应的按键抬起*/
				key_event.event = KEY_UP;
		}
}
/**
  * @brief    按键中断函数接口将，这个函数放到实际的中断函数入口处
  * @param    KEY_NUMBER 枚举类型指明具体的按键编号
  * @retval : None
**/
void key_exit_irq_interface(KEY_NUMBER key_index)
{
		/*按键消除抖动*/
		KEY_ELIMINATE_JITTER;
	
		/*根据信号源判断*/
		switch(key_index)
		{
			case KEY1:key1_handler();break;
			
			case KEY2:key2_handler();break;
			
			default:break;
		}
		
		/*回调按键消息*/
		key_message(&key_event);
}
/**
  * @brief    初始化按键GPIO,初始化中断
  * @param    None
  * @retval : None
**/
void init_button(void)
{
    
}
