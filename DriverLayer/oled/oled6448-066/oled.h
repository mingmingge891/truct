//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PB13��SCL��
//              D1   ��PB15��SDA��
//              RES  ��PB11
//              DC   ��PB10
//              CS   ��P12               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE     0

    

//-----------------����LED�˿ڶ���---------------- 
#define LED_ON GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define LED_OFF GPIO_SetBits(GPIOD,GPIO_Pin_2)

//-----------------OLED�˿ڶ���----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define OLED_RST_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_7)//RES
#define OLED_RST_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define OLED_DC_Clr()   GPIO_ResetBits(GPIOA,GPIO_Pin_6)//DC
#define OLED_DC_Set()   GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_2)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_2)
#define OLED_CS_Clr()   GPIO_ResetBits(GPIOB,GPIO_Pin_0)//CS
#define OLED_CS_Set()   GPIO_SetBits(GPIOB,GPIO_Pin_0)
 		     

#define OLED_PW_OFF()  GPIO_ResetBits(GPIOB,GPIO_Pin_1)//CS
#define OLED_PW_ON()   GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


void HalLcd_HW_Init(void);
void OLED_Refresh_Gram(void);
u8 OLED_GetPoint(int x,int y);
void OLED_SetPoint(int x,int y,u8 t);

#endif  
	 



