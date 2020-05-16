/**
  ******************************************************************************
  * @file    Weapon3Model_FLA.h
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
#ifndef __DL_BUTTON_H_
#define __DL_BUTTON_H_

/*功能逻辑抽象 function logic abstraction*/

typedef enum{KEY_DOWN =0,KEY_UP,KEY_DOUBLE}KeyState_Type;
typedef struct
{
	KeyState_Type KeyState;
	u8 ContinuousPressCnt;
	u16 press_time;
}KeyEvent_t;
typedef struct
{
	KeyEvent_t event;
	u8 PinIndex;
	void (*SetCallback)(KeyEvent_t*);
}KeyEventSet_t;

/*定义按键操作必要的抽象函数接口或者参数，
实现需要用户在用户层实现*/
typedef struct
{
	void (*Init)(void);       							          /*! Initialize the Key */
	u8   (*get_pin_state)(u8 index);                  /*! get the mcu pin state*/
	void (*irq_register)(void (*ptr)(u8));
}Button_Interface_t;

/*外部需要实现的结构体中的抽象函数以及一些参数实现*/
extern Button_Interface_t  Button_Interface;
void init_syskey(void);

/*定时器操作API
*/
typedef struct
{
	void (*button1_set_callback)(void (*ptr)(KeyEvent_t*));
	void (*button2_set_callback)(void (*ptr)(KeyEvent_t*));
	void (*button3_set_callback)(void (*ptr)(KeyEvent_t*));
	void (*button4_set_callback)(void (*ptr)(KeyEvent_t*));
	void (*button5_set_callback)(void (*ptr)(KeyEvent_t*));
	void (*button6_set_callback)(void (*ptr)(KeyEvent_t*));
	void (*button7_set_callback)(void (*ptr)(KeyEvent_t*));
	void (*button8_set_callback)(void (*ptr)(KeyEvent_t*));
	void (*button9_set_callback)(void (*ptr)(KeyEvent_t*));
	void (*button10_set_callback)(void (*ptr)(KeyEvent_t*));
}key_t;
extern key_t button;

#endif
