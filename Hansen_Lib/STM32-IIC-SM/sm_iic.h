/*
*********************************************************************************************************
*                                               SM_IIC
*                                         
*
*
*                                (c) Copyright 2018, Micrium, Weston, FL
*                                          All Rights Reserved
*
*                                           ARM Cortex-M3 Port
*
* File      : sm_iic.H
* Version   : V2.00
* By        : MING HONG. JIANG
*
* For       : ARMv7M Cortex-M3
* Mode      : Thumb2
* Toolchain : RealView Development Suite
*             RealView Microcontroller Development Kit (MDK)
*             ARM Developer Suite (ADS)
*             Keil uVision
*********************************************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SM_IIC_H
#define __SM_IIC_H

/*ƽ̨�������*/
/*�������Ͷ���  ��ͬ��ƽ̨����λ����ͬ*/
typedef unsigned char  smiic_uint8; 
typedef 			   char  smiic_int8; 
typedef unsigned short smiic_uint16;
typedef          short smiic_int16;
typedef unsigned int   smiic_uint32;
typedef 			   int   smiic_int32;
typedef float          smiic_fp32;
typedef double         smiic_fp64;

/*IIC ʱ�����*/
#define  D_START_STOP_1      4
#define  D_WAIT_ACK_2        1
#define  D_SET_ACK           2
#define  D_SEND_BYTE         2
#define  D_READ_BYTE         5

/**
  * @brief ����һ������ṹ��
  */
typedef struct
{
	/*��ʼ��*/
	void (*Init)(void);
	/*����ΪSDA IN*/
	void (*SDA_IN)(void);
	/*����ΪSDA	OUT*/
	void (*SDA_OUT)(void);
	/*IIC_SCL_SET*/
	void (*IIC_SCL_SET)(smiic_uint8 h_l);
	/*IIC_SDA_SET*/
	void (*IIC_SDA_SET)(smiic_uint8 h_l);
	/*IIC_SDA_SET*/
	smiic_uint8 (*IIC_SDA_READ)(void);
	/*delay*/
	void (*delay_us)(smiic_uint16 us);
	
}SM_IIC_Abstract_t;

/*����ṹ��ʵ��*/
extern SM_IIC_Abstract_t Implement_SMIIC_Abstract;

void SM_IIC_Initialize(void);
void IIC_Start(void);
void IIC_Stop(void);
smiic_uint8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(smiic_uint8 txd);
smiic_uint8 IIC_Read_Byte(unsigned char ack);

#endif

