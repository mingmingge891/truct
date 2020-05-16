/**
  ******************************************************************************
  * @file    dl_oled_drv.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    27-05-2019
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.twc.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2019 HansenTech</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "dl_oled096_12864_drv.h"
#include "string.h"
#include <stdio.h>
#include "stdarg.h"


#define X_WIDTH        128
#define Y_WIDTH        64

u8 OLED_GRAM[128][8];//显存
OLED_Interface_T *pOLED_Interface;

/*********************LCD写数据************************************/ 
void LCD_WrDat(unsigned char dat)     
{
    unsigned char i=8, temp=0;
    pOLED_Interface->LCD_DC(1);  
    for(i=0;i<8;i++) //发送一个八位数据 
    {
        pOLED_Interface->LCD_SCL(0);  
        
        temp = dat&0x80;
        if (temp == 0)
        {
            pOLED_Interface->LCD_SDA(0);
        }
        else
        {
            pOLED_Interface->LCD_SDA(1);
        }
        pOLED_Interface->LCD_SCL(1);             
        dat<<=1;    
    }
}
/*********************LCD写命令************************************/                                        
void LCD_WrCmd(unsigned char cmd)
{
    unsigned char i=8, temp=0;
    pOLED_Interface->LCD_DC(0);
    for(i=0;i<8;i++) //发送一个八位数据 
    { 
        pOLED_Interface->LCD_SCL(0); 
       
        temp = cmd&0x80;
        if (temp == 0)
        {
            pOLED_Interface->LCD_SDA(0);
        }
        else
        {
            pOLED_Interface->LCD_SDA(1);
        }
        pOLED_Interface->LCD_SCL(1);
        cmd<<=1;;        
    }     
}
/*********************LCD 设置坐标************************************/
void LCD_Set_Pos(unsigned char x, unsigned char y) 
{ 
    LCD_WrCmd(0xb0+y);
    LCD_WrCmd(((x&0xf0)>>4)|0x10);
    LCD_WrCmd((x&0x0f)|0x01); 
} 
 
void OLED_LCD_Fill(u8 data) 
{
	u8 i,n;		    
	for(n=0;n<128;n++)
	{
		for(i = 0;i<8;i++)
		{
			OLED_GRAM[n][i] = data;
		}
	}
}

/*********************LCD全屏************************************/
void LCD_Fill(unsigned char bmp_dat) 
{
    unsigned char y,x;
    for(y=0;y<8;y++)
    {
        LCD_WrCmd(0xb0+y);
        LCD_WrCmd(0x01);
        LCD_WrCmd(0x10);
        for(x=0;x<X_WIDTH;x++)
            LCD_WrDat(bmp_dat);
    }
}

/***************功能描述：行填充, y为页范围0～7****************/
void LCD_FillLine( unsigned char x,unsigned char y,unsigned char ch,unsigned char width)
{
		uint16_t i;
    LCD_WrCmd(0xb0+y);
    LCD_WrCmd(0x01);
    LCD_WrCmd(0x10); 
    for(i=x;i<width;i++)
        LCD_WrDat(ch);
}

/*********************LCD复位************************************/
void LCD_CLS(void)
{
    unsigned char y,x;    
    for(y=0;y<8;y++)
    {
        LCD_WrCmd(0xb0+y);
        LCD_WrCmd(0x01);
        LCD_WrCmd(0x10); 
        for(x=0;x<X_WIDTH;x++)
            LCD_WrDat(0);
    }
}

/*********************LCD初始化************************************/
void HalLcd_HW_Init(void)     
{  	
		pOLED_Interface = &Implement_OLED_Interface;
		pOLED_Interface->Hal_Init();

    pOLED_Interface->LCD_SCL(1);
    pOLED_Interface->LCD_RST(0);
    pOLED_Interface->delay_ms(50);
    pOLED_Interface->LCD_RST(1);      //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕   
    LCD_WrCmd(0xae);//--turn off oled panel
    LCD_WrCmd(0x00);//---set low column address
    LCD_WrCmd(0x10);//---set high column address
    LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    LCD_WrCmd(0x81);//--set contrast control register
    LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
    LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    LCD_WrCmd(0xa6);//--set normal display
    LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    LCD_WrCmd(0x3f);//--1/64 duty
    LCD_WrCmd(0xd3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    LCD_WrCmd(0x00);//-not offset
    LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    LCD_WrCmd(0xd9);//--set pre-charge period
    LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    LCD_WrCmd(0xda);//--set com pins hardware configuration
    LCD_WrCmd(0x12);
    LCD_WrCmd(0xdb);//--set vcomh
    LCD_WrCmd(0x40);//Set VCOM Deselect Level
    LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    LCD_WrCmd(0x02);//
    LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
    LCD_WrCmd(0x14);//--set(0x10) disable
    LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
    LCD_WrCmd(0xaf);//--turn on oled panel
    LCD_Fill(0x00); //初始成黑屏
    LCD_Set_Pos(0,0);
} 

int LCD_GetPoint(u8 x,u8 y)
{
	return 0;
}

//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		LCD_WrCmd (0xb0+i);    //设置页地址（0~7）
		LCD_WrCmd (0x00);      //设置显示位置—列低地址
		LCD_WrCmd (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)LCD_WrDat(OLED_GRAM[n][i]);
	}   
}

//得到点值
u8 OLED_GetPoint(int x,int y)
{
	return OLED_GRAM[x][y];
}

//打点函数
void OLED_SetPoint(int x,int y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;//这里有个问题就是显示镜像的问题  【pos=y/8 temp=1<<bx】 【pos=(7-y/8) temp=1<<(7-bx】 应该成对出现
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;
}
