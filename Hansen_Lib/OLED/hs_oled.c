/**
  ******************************************************************************
  * @file    hs_oled.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V0.0.1
  * @date    2-DECEMBER-2017
  * @brief   
  ******************************************************************************
  * @attention
  * http:
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT </center></h2>
  ******************************************************************************
  */
/*include ------------------------------------------------------------------------*/
#include "hs_cpu.h"
#include "hs_oled.h"
#include "string.h"
#include <stdio.h>
#include "stdarg.h"
//#include "user_string.h"

/*Macro ---------------------------------------------------------------------------*/
#define LCD_SCL    PBout(0)//SCLK  时钟 D0（SCLK）
#define LCD_SDA    PAout(7)//SDA   D1（MOSI） 数据
#define LCD_RST    PAout(6)//_RES  hardware reset   复位 
#define LCD_DC     PAout(5)//A0  H/L 命令数据选通端，H：数据，L:命令

/*延时函数*/
#define oled_delay_ms   delay_ms

/*Value ---------------------------------------------------------------------------*/
/*大约1k的显存*/
u8 OLED_GRAM[128][8];
/**
  * @brief  
  * @param
  * @arg
  * @retval
  */
void oled_wr_datd(unsigned char dat)
{
    unsigned char i=8, temp=0;
    LCD_DC=1;  
    for(i=0;i<8;i++) //发送一个八位数据 
    {
        LCD_SCL=0;  
        
        temp = dat&0x80;
        if (temp == 0)
        {
            LCD_SDA = 0;
        }
        else
        {
            LCD_SDA = 1;
        }
        LCD_SCL=1;             
        dat<<=1;    
    }
}
/**
  * @brief  
  * @param  
  * @arg 
  * @retval 
  */                                  
void oled_wr_cmd(unsigned char cmd)
{
    unsigned char i=8, temp=0;
    LCD_DC=0;
    for(i=0;i<8;i++) //发送一个八位数据 
    { 
        LCD_SCL=0; 
       
        temp = cmd&0x80;
        if (temp == 0)
        {
            LCD_SDA = 0;
        }
        else
        {
            LCD_SDA = 1;
        }
        LCD_SCL=1;
        cmd<<=1;;        
    }     
}
/**
  * @brief  LCD 设置坐标
  * @param  
  * @arg 
  * @retval 
  */
void oled_set_pos(unsigned char x, unsigned char y) 
{ 
    oled_wr_cmd(0xb0+y);
    oled_wr_cmd(((x&0xf0)>>4)|0x10);
    oled_wr_cmd((x&0x0f)|0x01);
}
///**
//  * @brief  
//  * @param  
//  * @arg 
//  * @retval 
//  */  
//void OLED_Clear(void)  
//{  
//	u8 i,n;  
//	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
//	OLED_Refresh_Gram();
//}
/**
  * @brief  
  * @param  
  * @arg 
  * @retval 
  */  
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		oled_wr_cmd (0xb0+i);
		oled_wr_cmd (0x00);
		oled_wr_cmd (0x10);
		for(n=0;n<128;n++)oled_wr_datd(OLED_GRAM[n][i]);
	}
}
/**
  * @brief  
  * @param  
  * @arg 
  * @retval 
  */  
void LCD_SetPoint(uint8_t x,uint8_t y,uint8_t data1)
{
	u8 pos,bx,temp=0;
	
	if(x>127||y>63)return;
	
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	
	if(data1)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;
	
//	oled_wr_cmd (0xb0+pos);
//	oled_wr_cmd (0x00);
//	oled_wr_cmd (0x10);
//	oled_wr_datd(OLED_GRAM[x][pos]);
}
/**
  * @brief  
  * @param  
  * @arg 
  * @retval 
  */  
uint32_t OLED_RedaPoinr(uint8_t x,uint8_t y)
{
	oled_set_pos(x,y);
	return (OLED_GRAM[x][y]);
}
/**
  * @brief  
  * @param  
  * @arg 
  * @retval 
  */  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)LCD_SetPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();
}
/**
  * @brief gpio初始化
  * @param 
  * @arg 
  * @retval 
  */
void oled_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;       		
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       		
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief oled初始化
  * @param 
  * @arg 
  * @retval 
  */
void lcd_hw_init(void)
{  
	oled_gpio_init();	
	
	LCD_SCL=1;
	LCD_RST=0;
	oled_delay_ms(50);
	LCD_RST=1;      //从上电到下面开始初始化要有足够的时间，即等待RC复位完毕   
	oled_wr_cmd(0xae);//--turn off oled panel
	oled_wr_cmd(0x00);//---set low column address
	oled_wr_cmd(0x10);//---set high column address
	oled_wr_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_wr_cmd(0x81);//--set contrast control register
	oled_wr_cmd(0xcf); // Set SEG Output Current Brightness
	oled_wr_cmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_wr_cmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_wr_cmd(0xa6);//--set normal display
	oled_wr_cmd(0xa8);//--set multiplex ratio(1 to 64)
	oled_wr_cmd(0x3f);//--1/64 duty
	oled_wr_cmd(0xd3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
	oled_wr_cmd(0x00);//-not offset
	oled_wr_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
	oled_wr_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_wr_cmd(0xd9);//--set pre-charge period
	oled_wr_cmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_wr_cmd(0xda);//--set com pins hardware configuration
	oled_wr_cmd(0x12);
	oled_wr_cmd(0xdb);//--set vcomh
	oled_wr_cmd(0x40);//Set VCOM Deselect Level
	oled_wr_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_wr_cmd(0x02);//
	oled_wr_cmd(0x8d);//--set Charge Pump enable/disable
	oled_wr_cmd(0x14);//--set(0x10) disable
	oled_wr_cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	oled_wr_cmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	oled_wr_cmd(0xaf);//--turn on oled panel
	//oled_fill(0x00); //初始成黑屏
	oled_set_pos(0,0);   
} 
