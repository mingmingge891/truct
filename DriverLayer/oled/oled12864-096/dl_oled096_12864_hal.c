/**
  ******************************************************************************
  * @file    dl_oled_hal.c
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
#include "dl_oled096_12864_drv.h"
#include "adp_mcu_delay.h"
#include "sys.h"


#define SCL_GPIO_PORT       GPIOE
#define SCL_GPIO_PIN        GPIO_Pin_11
                                  
#define SDA_GPIO_PORT       GPIOE
#define SDA_GPIO_PIN        GPIO_Pin_12
                                        
#define RST_GPIO_PORT       GPIOE
#define RST_GPIO_PIN        GPIO_Pin_13
                                           
#define DC_GPIO_PORT        GPIOE
#define DC_GPIO_PIN         GPIO_Pin_14

static void OLED_LCD_SCL(u8 hl){GPIO_WriteBit(SCL_GPIO_PORT,SCL_GPIO_PIN,(BitAction)hl);}//SCLK  时钟 D0（SCLK）
static void OLED_LCD_SDA(u8 hl){GPIO_WriteBit(SDA_GPIO_PORT,SDA_GPIO_PIN,(BitAction)hl);}//SDA   D1（MOSI） 数据
static void OLED_LCD_RST(u8 hl){GPIO_WriteBit(RST_GPIO_PORT,RST_GPIO_PIN,(BitAction)hl);}//RES  hardware reset   复位 
static void OLED_LCD_DC(u8 hl) {GPIO_WriteBit(DC_GPIO_PORT, DC_GPIO_PIN,(BitAction)hl);}//H/L 命令数据选通端，H：数据，L:命令

//HAL_Init
static void OLED_HalInitializes(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	//SCL
	GPIO_InitStructure.GPIO_Pin = SCL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStructure);

	//SDA
	GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);

	//RST
	GPIO_InitStructure.GPIO_Pin = RST_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(RST_GPIO_PORT, &GPIO_InitStructure);

	//DC
	GPIO_InitStructure.GPIO_Pin = DC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(DC_GPIO_PORT, &GPIO_InitStructure);
}

OLED_Interface_T Implement_OLED_Interface = 
{
	.LCD_SCL  = OLED_LCD_SCL,
	.LCD_SDA  = OLED_LCD_SDA,
	.LCD_RST  = OLED_LCD_RST,
	.LCD_DC   = OLED_LCD_DC,
	.Hal_Init = OLED_HalInitializes,
	.delay_ms = ADP_delay_ms,
	.delay_us = ADP_delay_us,
};


