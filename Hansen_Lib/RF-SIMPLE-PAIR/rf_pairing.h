/**
  ******************************************************************************
  * @file    rf_pairing.h
  * @author  HANSEN     emil:951868692@qq.com
  * @version V0.0.0
  * @date    14-6-2018
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.tfc.com
	* This code developed by hardware science and technology division 
	* of tfc technology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2018 TFC</center></h2>
  ******************************************************************************
  */
#ifndef _RF_PAIRING_H
#define _RF_PAIRING_H

#ifdef 	STM32

#include "stm32f10x.h"

#else

typedef unsigned char  uint8_t;              /* Unsigned  8 bit quantity                           */
typedef 	 				char  int8;                /* Signed    8 bit quantity                           */
typedef unsigned short uint16_t;             /* Unsigned 16 bit quantity                           */
typedef          short int16;                /* Signed   16 bit quantity                           */
typedef unsigned int   uint32_t;             /* Unsigned 32 bit quantity                           */
typedef 	       int   int32;          			 /* Signed   32 bit quantity                           */
typedef float          fp32;                 /* Single precision floating point                    */
typedef double         fp64;                 /* Double precision floating point                    */
#endif

#define MASTER
//#define SLAVE


#define PAIR_TIME_OUT    		30*1000//��Գ�ʱʱ��
#define ID_LENGTH           12//id�ĳ���
#define CODING_LENGTH 			2//pair ����2�ֽ�
#define RX_BUF_LEN          32//rf�Ľ��ջ����С���ⲿ��������һ��

extern uint8_t  Rx_Buf[RX_BUF_LEN];//����Ľ�������

//������Ϣ
typedef struct
{
	uint8_t  mac[ID_LENGTH];//id����
	uint8_t  special_encoding[2];//ͨ�������������ʶ���������ģʽ��
	uint8_t  bind_flag;//����ָ���ӻ��İ�״̬
}pair_process_info_t;

//������Ϣ
typedef struct
{
	void* data;
	uint16_t length;
}
setint_info_t;

/*interface Def------------------------------------------------------------*/
typedef struct
{
	void (*send_data)(void *);
	void (*delay_ms)(uint32_t);
	void (*delay_us)(uint32_t);
	void (*pair_status)(uint8_t,void *);
	uint8_t* (*get_device_id)(void);
}rf_pair_prop_t;

/*EXTERN VALUE-----------------------------------------------------------------*/
extern rf_pair_prop_t implement_rf_pair_property;													/*!ʵ�ֵĽṹ������*/
extern setint_info_t implement_seting_info_property;

void init_rf_pairing(void);

#ifdef MASTER
void rf_pair_algorithm(void);
void* rf_pair_ack(void);
void rf_pair_state_clear(void);
#endif

#ifdef SLAVE
void rf_start_pair_process_slave(void);
#endif

#endif
