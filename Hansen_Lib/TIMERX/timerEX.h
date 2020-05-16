/**
  ******************************************************************************
  * @file    timerEX.h
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
#ifndef __TIMEREX_H_
#define __TIMEREX_H_

#ifdef 	STM32

#include "hal_init.h"

#else

typedef unsigned char  uint8_t;              /* Unsigned  8 bit quantity                           */
typedef 	 			 char  int8;                 /* Signed    8 bit quantity                           */
typedef unsigned short uint16_t;             /* Unsigned 16 bit quantity                           */
typedef          short int16;                /* Signed   16 bit quantity                           */
typedef unsigned int   uint32_t;             /* Unsigned 32 bit quantity                           */
typedef 	       int   int32;          			 /* Signed   32 bit quantity                           */
typedef float          fp32;                 /* Single precision floating point                    */
typedef double         fp64;                 /* Double precision floating point                    */
#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t

#endif


/*虚拟定时器的个数*/
#define 	TIME_EVENT_NUM 								8

/*硬件定时器的个数*/
#define 	HARDWARE_TIMERX_NUM 					2

typedef struct
{
	void (*t_callBack)(void*);//回调函数
	uint32_t time_out;//定时器溢出
	uint32_t time_count;//定时器的计数
	void *   prg;//定时器参数传递
	uint8_t  nest_timerCnt;//嵌套定时器的个数
}timerEx_callback_t;

/*定义4个定时器回调函数*/
typedef struct
{
	void (*HS_Timer0_DataCallback)(void);
	void (*HS_Timer1_DataCallback)(void);
	void (*HS_Timer2_DataCallback)(void);
	void (*HS_Timer3_DataCallback)(void);
	
}TIMER_OUTSIDE_DATA_CALLBACK_T;

/*定义定时器操作必要的抽象函数或者参数，
实现需要用户在用户层实现*/
typedef struct
{
	void (*Init)(void);       							 /*! Initialize the timer */
	
	void (*Timer_CMD)(u8,u8);                /*! Timer CMD */
}TIMER_PROP_T;

/*外部需要实现的结构体中的抽象函数以及一些参数实现*/
extern TIMER_PROP_T Implement_Timer_Property;

/*外部使用这个抽象结构体中的函数，实现内部已经写好。*/
extern TIMER_OUTSIDE_DATA_CALLBACK_T* pData_callback_timerIrq;

typedef struct
{
	/**
  * @brief  启动一个定时器,自动重载
  * @param  timerEx_id:		编号
						p_arg：				参数传递
						ptr：					回调函数
						timer_period：定时周期
  * @retval : 0： fail 
							1： succ
	**/
	uint8_t (*start_timerEx)(uint8_t h_id,uint8_t timerEx_id,void* p_arg,void (*ptr)(void*),uint32_t timer_period);
	
	/**
  * @brief 关闭一个定时器
  * @param  timerEx_id：编号
  * @retval : 0： fail 
							1： succ
	**/
	uint8_t (*stop_timerEx)(uint8_t h_id,uint8_t timerEx_id);
	
}virtual_nest_timerExInterface_t;

typedef struct
{
	/**
  * @brief  启动一个定时器,自动重载
  * @param  timerEx_id:		编号
						p_arg：				参数传递
						ptr：					回调函数
						timer_period：定时周期
  * @retval : 0： fail 
							1： succ
	**/
	uint8_t (*start_timerEx)(uint8_t h_id,uint8_t timerEx_id,void* p_arg,void (*ptr)(void*),uint32_t timer_period);
	
	/**
  * @brief 关闭一个定时器
  * @param  timerEx_id：编号
  * @retval : 0： fail 
							1： succ
	**/
	uint8_t (*stop_timerEx)(uint8_t h_id,uint8_t timerEx_id);
	
	/**
  * @brief 创建嵌套定时器
  * @param  parents_id：父ID
						timerExCnt: 个数
	**/
	virtual_nest_timerExInterface_t* (*create_nestTimerEx)(uint8_t h_id,uint8_t parents_id,uint8_t timerExCnt);
	
	/*硬件ID*/
	uint8_t this_h_id;
	
}virtual_timerExInterface_t;

/**
  * @brief 创建一个定时器
  * @param  hardwareTimerEx_ID 硬件定时器编号
  * @retval : virtual_timerExInterface_t 结构体
**/
virtual_timerExInterface_t* create_new_virtual_timerEX(uint8_t hardwareTimerEx_ID);

/**
  * @brief  循环执行
  * @param  None
  * @retval : None
**/
void virtualTimerRun(uint8_t id);

/**
  * @brief  This function 初始化系统定时器
  * @param 	void
  * @retval : None
**/
void init_timerEX(void);

/*extern vlaues */
extern virtual_timerExInterface_t* virtual_timer0Handler;
extern virtual_timerExInterface_t* virtual_timer1Handler;


#endif
