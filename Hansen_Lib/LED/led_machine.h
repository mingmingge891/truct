/**
  ******************************************************************************
  * @file    led_machine.h
  * @author  hansen.jiang 951868692@qq.com
  * @version 1.0
  * @date    2018/11/29
  * @brief   
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
#ifndef __LED_MACHINE_H_
#define __LED_MACHINE_H_


#ifdef 	STM32

#else

typedef unsigned char  uint8_t;              /* Unsigned  8 bit quantity                           */
typedef 	 			 char  int8;                 /* Signed    8 bit quantity                           */
typedef unsigned short uint16_t;             /* Unsigned 16 bit quantity                           */
typedef          short int16;                /* Signed   16 bit quantity                           */
typedef unsigned int   uint32_t;             /* Unsigned 32 bit quantity                           */
typedef 	       int   int32;          			 /* Signed   32 bit quantity                           */
typedef float          fp32;                 /* Single precision floating point                    */
typedef double         fp64;                 /* Double precision floating point                    */

#endif


/*定时器*/
#define LED_MachineTimerStart(x2,x3,x4,x5)     virtual_timer1Handler->start_timerEx(virtual_timer1Handler->this_h_id,x2,x3,x4,x5);
#define LED_MachineTimerStop(x)                virtual_timer1Handler->stop_timerEx(virtual_timer1Handler->this_h_id,x);


/*定义硬件LED*/
#define HAL_LED_R(x) 
#define HAL_LED_G(x)
#define HAL_LED_B(x)

/** 
  * @brief  COLOR_U enum definition
  */
typedef enum
{
	WHITE = 0,
	YELLOW,
	GREEN,
	BLUE,
	PURPLE,
	RED,
	BLACK,
	NONE
}COLOR_U;

/** 
  * @brief  LED Control structure definition
  */
typedef struct
{
	uint16_t open_timer;
	uint16_t close_timer;
	COLOR_U  color;
	uint8_t  openflag;
	uint8_t  num;
}LedCtl_t;

/** 
  * @brief  LED_Machine_PROP structure definition
  */
typedef struct
{
	uint8_t InternelTimerPriority; /* timer priority */
	
}LED_Machine_PROP_T;

/** 
  * @brief  LED Abstract structure definition
  */
typedef struct
{
	/*最后一个参数设置亮灭*/
	void (*SetValue)(COLOR_U,uint8_t);
	
	/*最后一个参数设置亮的时间*/
	
	void (*SetValueA)(COLOR_U,uint16_t);
	
	/*后面两个参数 第一个设置亮的时间,最后一个参数设置灭的时间*/
	void (*SetFlashing)(COLOR_U,uint16_t,uint16_t);
	
	/*可以设置闪烁的次数*/
	void (*SetFlashingA)(COLOR_U,uint16_t,uint16_t,uint8_t);
	
}Machine_LED_Abstract_t;

/* extern values for implements property*/
extern LED_Machine_PROP_T Implement_LED_Machine_Property;

/* extern function */
void Machine_LED_SetFlashing(COLOR_U color ,uint16_t openTimer,uint16_t closeTimer);
void Machine_LED_SetFlashingA(COLOR_U color ,uint16_t openTimer,uint16_t closeTimer,uint8_t num);
void Machine_LED_SetValue(COLOR_U color ,uint8_t value);
void Machine_LED_SetA(COLOR_U color ,uint16_t value);
void Machine_LEDInitializes(void);
void Machine_LED_NEW(Machine_LED_Abstract_t* p);

#endif
