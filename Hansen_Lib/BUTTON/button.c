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

/*��������������ʱ������������ʵ��*/
#define  KEY_ELIMINATE_JITTER  delay_ms(20)

/* Value ---------------------------------------------------------------------*/
/*������Ϣ�¼�����*/
key_message_t key_event;

/*�����ص���������*/
callback_keyMesaage key_message;
/* ˵�� ----------------------------------------------------------------------*/
/*��ǰд����������Ҫʵ�ֶ�����������Լ���Դ���룬��Ƭ���ж�Ҫ��ʵ�������½���ͬʱ
����*/

/**
  * @brief    ע���¼��ص�����
  * @param    ����ָ���������button_message_t
  * @retval : None
**/
void register_key_callback(void (*ptr)(key_message_t*))
{
		if(ptr != NULL){
				key_message = ptr;
		}
}
/**
  * @brief  KEY1��������
  * @param  None
  * @retval : None
**/
static void key1_handler(void)
{
		key_event.index = KEY1;
		if(!KEY1_GPIO)
	   {
				/*��Ӧ�İ�������*/
				key_event.event = KEY_DOWN;
		}
		else if(KEY1_GPIO && key_event.event == KEY_DOWN){
				/*��Ӧ�İ���̧��*/
				key_event.event = KEY_UP;
		}
}
/**
  * @brief  KEY2��������
  * @param  None
  * @retval : None
**/
static void key2_handler(void)
{
		key_event.index = KEY2;
		if(KEY2_GPIO)
		{
				/*��Ӧ�İ�������*/
				key_event.event = KEY_DOWN;
		}
		else if(KEY2_GPIO && key_event.event == KEY_DOWN){
				/*��Ӧ�İ���̧��*/
				key_event.event = KEY_UP;
		}
}
/**
  * @brief    �����жϺ����ӿڽ�����������ŵ�ʵ�ʵ��жϺ�����ڴ�
  * @param    KEY_NUMBER ö������ָ������İ������
  * @retval : None
**/
void key_exit_irq_interface(KEY_NUMBER key_index)
{
		/*������������*/
		KEY_ELIMINATE_JITTER;
	
		/*�����ź�Դ�ж�*/
		switch(key_index)
		{
			case KEY1:key1_handler();break;
			
			case KEY2:key2_handler();break;
			
			default:break;
		}
		
		/*�ص�������Ϣ*/
		key_message(&key_event);
}
/**
  * @brief    ��ʼ������GPIO,��ʼ���ж�
  * @param    None
  * @retval : None
**/
void init_button(void)
{
    
}
