/**
  ******************************************************************************
  * @file    dl_key.C
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    8-3-2020
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
#include "dl_system.h"
#include "dl_button.h"
#include "dl_delay.h"
#include "dl_time.h"
#include "stdarg.h"
#include <stdio.h>

KeyEventSet_t KeyEventSet[10];

/*BUTTON抽象函数结构体变量*/
Button_Interface_t* pKEY_Implements;

//定时器
timer_t *buttonTime = NULL;
#define  KEY_TIME_EVENT                              0/**/
#define  KEY_JITTER_TIME_EVENT                       1/**/
#define  KEY_ELIMINATE_JITTER_TIME_OUT               25/*25ms*/
#define  KEY_PRESS_TIME_EVENT                        2/**/

/**
  * @brief  按键事件检测
  * @param  
  *   
  * @param  
  * @retval 
  */
void KeyEventHandler(void* p)
{
	KeyEventSet_t *pKeyEvent = (KeyEventSet_t*)p;
	buttonTime->stop(buttonTime->instance,KEY_TIME_EVENT);
	if(pKeyEvent->SetCallback != NULL){
		pKeyEvent->SetCallback(&pKeyEvent->event);
	}
	pKeyEvent->event.ContinuousPressCnt = 0;
}
/**
  * @brief  
  * @param  
  *   
  * @param  
  * @retval 
  */
void KeyPressTimeHandler(void *p)
{
	KeyEventSet_t *pKeyEvent = (KeyEventSet_t*)p;
	pKeyEvent->event.press_time++;
}
/**
  * @brief  按键消抖公用函数
  * @param  
  *   
  * @param  
  * @retval 
  */
static void KeyJitterCommon(void *p)
{
	KeyEventSet_t *pKeyEvent = (KeyEventSet_t*)p;
	buttonTime->stop(buttonTime->instance,KEY_JITTER_TIME_EVENT);
	if(pKEY_Implements->get_pin_state(pKeyEvent->PinIndex) == 0){
		pKeyEvent->event.KeyState = KEY_DOWN;
		pKeyEvent->event.ContinuousPressCnt++;
		pKeyEvent->event.press_time = 0;
		buttonTime->start(buttonTime->instance,p,KeyPressTimeHandler,1,KEY_PRESS_TIME_EVENT);
	}
	else if(pKEY_Implements->get_pin_state(pKeyEvent->PinIndex) == 1 && pKeyEvent->event.KeyState == KEY_DOWN){
		pKeyEvent->event.KeyState = KEY_UP;
		buttonTime->stop(buttonTime->instance,KEY_PRESS_TIME_EVENT);
	}
	/*如果不识别抬起的话 这里就是1 否则设置为2*/
	if(pKeyEvent->event.ContinuousPressCnt > 1){
		/*双击 或者多击中*/
		pKeyEvent->event.KeyState = KEY_DOUBLE;
	}
	/*如果只识别双击的话 定时时间一般设置为100-200  否则设置为500最好*/
	buttonTime->start(buttonTime->instance,p,KeyEventHandler,500,KEY_TIME_EVENT);
}

/*按键相关*/
static void button1_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[0].SetCallback = ptr;}
static void button2_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[1].SetCallback = ptr;}
static void button3_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[2].SetCallback = ptr;}
static void button4_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[3].SetCallback = ptr;}
static void button5_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[4].SetCallback = ptr;}
static void button6_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[5].SetCallback = ptr;}
static void button7_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[6].SetCallback = ptr;}
static void button8_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[7].SetCallback = ptr;}
static void button9_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[8].SetCallback = ptr;}
static void button10_set_callback(void (*ptr)(KeyEvent_t*)){KeyEventSet[9].SetCallback = ptr;}

/*API*/
key_t button = 
{
	.button1_set_callback  =  button1_set_callback,
	.button2_set_callback  =  button2_set_callback,
	.button3_set_callback  =  button3_set_callback,
	.button4_set_callback  =  button4_set_callback,
	.button5_set_callback  =  button5_set_callback,
	.button6_set_callback  =  button6_set_callback,
	.button7_set_callback  =  button7_set_callback,
	.button8_set_callback  =  button8_set_callback,
	.button9_set_callback  =  button9_set_callback,
	.button10_set_callback =  button10_set_callback,
};

/**
  * @brief  中断回调函数
  * @param  
  *   
  * @param  
  * @retval 
  */
static void Key_irq_callback(u8 index)
{
	KeyEventSet[index].PinIndex = index;
	buttonTime->start(buttonTime->instance,&KeyEventSet[index],KeyJitterCommon,KEY_ELIMINATE_JITTER_TIME_OUT,KEY_JITTER_TIME_EVENT);
}
/**
  * @brief  初始化系统按键
  * @param 	void
  * @retval : None
**/
void init_syskey(void)
{
	buttonTime = timerNew(3);
	/*得到对象*/
	pKEY_Implements = &Button_Interface;
	pKEY_Implements->Init();
	pKEY_Implements->irq_register(Key_irq_callback);
}
