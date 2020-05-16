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


/*���ⶨʱ���ĸ���*/
#define 	TIME_EVENT_NUM 								8

/*Ӳ����ʱ���ĸ���*/
#define 	HARDWARE_TIMERX_NUM 					2

typedef struct
{
	void (*t_callBack)(void*);//�ص�����
	uint32_t time_out;//��ʱ�����
	uint32_t time_count;//��ʱ���ļ���
	void *   prg;//��ʱ����������
	uint8_t  nest_timerCnt;//Ƕ�׶�ʱ���ĸ���
}timerEx_callback_t;

/*����4����ʱ���ص�����*/
typedef struct
{
	void (*HS_Timer0_DataCallback)(void);
	void (*HS_Timer1_DataCallback)(void);
	void (*HS_Timer2_DataCallback)(void);
	void (*HS_Timer3_DataCallback)(void);
	
}TIMER_OUTSIDE_DATA_CALLBACK_T;

/*���嶨ʱ��������Ҫ�ĳ��������߲�����
ʵ����Ҫ�û����û���ʵ��*/
typedef struct
{
	void (*Init)(void);       							 /*! Initialize the timer */
	
	void (*Timer_CMD)(u8,u8);                /*! Timer CMD */
}TIMER_PROP_T;

/*�ⲿ��Ҫʵ�ֵĽṹ���еĳ������Լ�һЩ����ʵ��*/
extern TIMER_PROP_T Implement_Timer_Property;

/*�ⲿʹ���������ṹ���еĺ�����ʵ���ڲ��Ѿ�д�á�*/
extern TIMER_OUTSIDE_DATA_CALLBACK_T* pData_callback_timerIrq;

typedef struct
{
	/**
  * @brief  ����һ����ʱ��,�Զ�����
  * @param  timerEx_id:		���
						p_arg��				��������
						ptr��					�ص�����
						timer_period����ʱ����
  * @retval : 0�� fail 
							1�� succ
	**/
	uint8_t (*start_timerEx)(uint8_t h_id,uint8_t timerEx_id,void* p_arg,void (*ptr)(void*),uint32_t timer_period);
	
	/**
  * @brief �ر�һ����ʱ��
  * @param  timerEx_id�����
  * @retval : 0�� fail 
							1�� succ
	**/
	uint8_t (*stop_timerEx)(uint8_t h_id,uint8_t timerEx_id);
	
}virtual_nest_timerExInterface_t;

typedef struct
{
	/**
  * @brief  ����һ����ʱ��,�Զ�����
  * @param  timerEx_id:		���
						p_arg��				��������
						ptr��					�ص�����
						timer_period����ʱ����
  * @retval : 0�� fail 
							1�� succ
	**/
	uint8_t (*start_timerEx)(uint8_t h_id,uint8_t timerEx_id,void* p_arg,void (*ptr)(void*),uint32_t timer_period);
	
	/**
  * @brief �ر�һ����ʱ��
  * @param  timerEx_id�����
  * @retval : 0�� fail 
							1�� succ
	**/
	uint8_t (*stop_timerEx)(uint8_t h_id,uint8_t timerEx_id);
	
	/**
  * @brief ����Ƕ�׶�ʱ��
  * @param  parents_id����ID
						timerExCnt: ����
	**/
	virtual_nest_timerExInterface_t* (*create_nestTimerEx)(uint8_t h_id,uint8_t parents_id,uint8_t timerExCnt);
	
	/*Ӳ��ID*/
	uint8_t this_h_id;
	
}virtual_timerExInterface_t;

/**
  * @brief ����һ����ʱ��
  * @param  hardwareTimerEx_ID Ӳ����ʱ�����
  * @retval : virtual_timerExInterface_t �ṹ��
**/
virtual_timerExInterface_t* create_new_virtual_timerEX(uint8_t hardwareTimerEx_ID);

/**
  * @brief  ѭ��ִ��
  * @param  None
  * @retval : None
**/
void virtualTimerRun(uint8_t id);

/**
  * @brief  This function ��ʼ��ϵͳ��ʱ��
  * @param 	void
  * @retval : None
**/
void init_timerEX(void);

/*extern vlaues */
extern virtual_timerExInterface_t* virtual_timer0Handler;
extern virtual_timerExInterface_t* virtual_timer1Handler;


#endif
