//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PB13（SCL）
//              D1   接PB15（SDA）
//              RES  接PB11
//              DC   接PB10
//              CS   接P12               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE     0

    

//-----------------测试LED端口定义---------------- 
#define LED_ON GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define LED_OFF GPIO_SetBits(GPIOD,GPIO_Pin_2)

//-----------------OLED端口定义----------------  					   

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

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


void HalLcd_HW_Init(void);
void OLED_Refresh_Gram(void);
u8 OLED_GetPoint(int x,int y);
void OLED_SetPoint(int x,int y,u8 t);

#endif  
	 



