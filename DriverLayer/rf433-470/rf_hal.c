/*物理层硬件抽象*/
/********************Includes****************************/
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "rf_433.h"
#include "dl_delay.h"
#include "dl_time.h"
#include "string.h"

#define   dwPolynomial          0x4C11DB7L      /*CRC32多项式*/

/*gpio时钟*/
#define   RF_GPIO_RCC     		  RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOA

///////这是子元科技新的板子的配置
///*pwr_c*/
//#define RF_PWR_GPIO_PORT        GPIOA
//#define RF_PWR_GPIO_PIN         GPIO_Pin_15
//#define RF_PWR_GPIO_RCC         RCC_AHBPeriph_GPIOA

///*scs*/                                                  
//#define RF_nSCS_GPIO_PORT       GPIOB
//#define RF_nSCS_GPIO_PIN        GPIO_Pin_3
//#define RF_nSCS_GPIO_RCC        RCC_AHBPeriph_GPIOB

///*sclk*/                                                
//#define RF_SCLK_GPIO_PORT       GPIOB
//#define RF_SCLK_GPIO_PIN        GPIO_Pin_4
//#define RF_SCLK_GPIO_RCC        RCC_AHBPeriph_GPIOB

///*mosi-sdio*/                                                
//#define RF_MOSI_GPIO_PORT       GPIOB
//#define RF_SDIO_GPIO_PORT       GPIOB
//#define RF_MOSI_GPIO_PIN        GPIO_Pin_5
//#define RF_SDIO_GPIO_PIN        GPIO_Pin_5
//#define RF_MOSI_GPIO_RCC        RCC_AHBPeriph_GPIOB

/////*miso*/                                                
//#define RF_MISO_GPIO_PORT       GPIOB
//#define RF_MISO_GPIO_PIN        GPIO_Pin_7
//#define RF_MISO_GPIO_RCC        RCC_AHBPeriph_GPIOB

///*中断*/        
//#define RF_nIRQ_GPIO_PORT       GPIOB
//#define RF_nIRQ_GPIO_PIN        GPIO_Pin_6
//#define RF_nIRQ_GPIO_RCC        RCC_AHBPeriph_GPIOB
//#define RF_nIRQ_EXIT_LINE       EXTI_Line6
//#define RF_nIRQ_GPIO_PIN_S      GPIO_PinSource6
//#define RF_nIRQ_GPIO_PORT_S 		GPIO_PortSourceGPIOB
//#define RF_nIRQ_IRQ_EXTI_Line		EXTI_Line6
//#define RF_nIRQ_IRQ_CHANNEL			EXTI9_5_IRQn
//#define RF_nIRQ_IRQ_Handler     EXTI9_5_IRQHandler


///这是以前  第五位科技的USB卡的配置
/*pwr_c*/
#define RF_PWR_GPIO_PORT        GPIOA
#define RF_PWR_GPIO_PIN         GPIO_Pin_1
#define RF_PWR_GPIO_RCC         RCC_AHBPeriph_GPIOA

/*scs*/                                                  
#define RF_nSCS_GPIO_PORT       GPIOB             
#define RF_nSCS_GPIO_PIN        GPIO_Pin_5
#define RF_nSCS_GPIO_RCC        RCC_AHBPeriph_GPIOB

/*sclk*/                                                
#define RF_SCLK_GPIO_PORT       GPIOB        
#define RF_SCLK_GPIO_PIN        GPIO_Pin_6
#define RF_SCLK_GPIO_RCC        RCC_AHBPeriph_GPIOB

/*mosi-sdio*/                                                
#define RF_MOSI_GPIO_PORT       GPIOB 
#define RF_SDIO_GPIO_PORT       GPIOB 
#define RF_MOSI_GPIO_PIN        GPIO_Pin_7
#define RF_SDIO_GPIO_PIN        GPIO_Pin_7
#define RF_MOSI_GPIO_RCC        RCC_AHBPeriph_GPIOB

/*miso*/                                                
#define RF_MISO_GPIO_PORT       GPIOB             
#define RF_MISO_GPIO_PIN        GPIO_Pin_7
#define RF_MISO_GPIO_RCC        RCC_AHBPeriph_GPIOB

/*中断*/        
#define RF_nIRQ_GPIO_PORT       GPIOA
#define RF_nIRQ_GPIO_PIN        GPIO_Pin_0
#define RF_nIRQ_GPIO_RCC        RCC_AHBPeriph_GPIOA
#define RF_nIRQ_EXIT_LINE       EXTI_Line0 
#define RF_nIRQ_GPIO_PIN_S      GPIO_PinSource0
#define RF_nIRQ_GPIO_PORT_S 		GPIO_PortSourceGPIOA
#define RF_nIRQ_IRQ_CHANNEL			EXTI0_IRQn
#define RF_nIRQ_IRQ_EXTI_Line		EXTI_Line0
#define RF_nIRQ_IRQ_Handler			EXTI0_IRQHandler

/*Values -------------------------------------------------------*/
EXTI_InitTypeDef RF_EXTI_InitStructure;

void SendHandler(void* p)
{
	RF_433_470.ReceiveHandler();//这个函数可以放到住while中执行
	virtual_timer1Handler->stop_timerEx(virtual_timer1Handler->this_h_id,1);
}


/*接收中断*/
void RF_nIRQ_IRQ_Handler(void)
{
	if(EXTI_GetITStatus(RF_nIRQ_IRQ_EXTI_Line) != RESET)
	{
		EXTI_ClearITPendingBit(RF_nIRQ_IRQ_EXTI_Line);
		RF_433_470.IrqHandler();//只是对中断的标志位设置不占用大量的时间
		virtual_timer1Handler->start_timerEx(virtual_timer1Handler->this_h_id,1,NULL,SendHandler,1);
	}
}

/****************************************************************************
* 名    称：void RF_NVIC_Init(void)
* 功    能：RF中断源配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/

void RF_NVIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	/* 配置P[A|B|C|D|E]0为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = RF_nIRQ_IRQ_CHANNEL;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* EXTI line gpio config(RF_nIRQ_GPIO_PIN) */	
  GPIO_InitStructure.GPIO_Pin =  RF_nIRQ_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(RF_nIRQ_GPIO_PORT, &GPIO_InitStructure);

	/* EXTI line(RF_nIRQ_GPIO_PIN) mode config */
	GPIO_EXTILineConfig(RF_nIRQ_GPIO_PORT_S, RF_nIRQ_GPIO_PIN_S);
  RF_EXTI_InitStructure.EXTI_Line = RF_nIRQ_EXIT_LINE;
  RF_EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  RF_EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  RF_EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&RF_EXTI_InitStructure);
	
}

/*************************************************************
函数名称:RF_Hal_Init
功    能:RF硬件抽象层初始化
参    数:无
返 回 值:无
说    明:无
*************************************************************/
void RF_Hal_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	//PWR
	GPIO_InitStructure.GPIO_Pin = RF_PWR_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RF_PWR_GPIO_PORT, &GPIO_InitStructure);

	//SDIO
	GPIO_InitStructure.GPIO_Pin = RF_SDIO_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RF_SDIO_GPIO_PORT, &GPIO_InitStructure);

	//SCS
	GPIO_InitStructure.GPIO_Pin = RF_nSCS_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(RF_nSCS_GPIO_PORT, &GPIO_InitStructure);

	//SCK
	GPIO_InitStructure.GPIO_Pin = RF_SCLK_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(RF_SCLK_GPIO_PORT, &GPIO_InitStructure);
	
  /*interrupt init*/
  RF_NVIC_Init();
	
}

static u8   HAL_IS_RF_nIRQ_H(){return GPIO_ReadInputDataBit(RF_nIRQ_GPIO_PORT,RF_nIRQ_GPIO_PIN);}
static u8   HAL_RF_SDIO_IN(){return GPIO_ReadInputDataBit(RF_SDIO_GPIO_PORT,RF_SDIO_GPIO_PIN);}
static u8   HAL_RF_MISO_IN(){return GPIO_ReadInputDataBit(RF_MISO_GPIO_PORT,RF_MISO_GPIO_PIN);}
static void HAL_RF_MOSI_SET(u8 hl){GPIO_WriteBit(RF_MOSI_GPIO_PORT,RF_MOSI_GPIO_PIN,(BitAction)hl);}
static void HAL_RF_SCLK_SET(u8 hl){GPIO_WriteBit(RF_SCLK_GPIO_PORT,RF_SCLK_GPIO_PIN,(BitAction)hl);}
static void HAL_RF_nSCS_SET(u8 hl){GPIO_WriteBit(RF_nSCS_GPIO_PORT,RF_nSCS_GPIO_PIN,(BitAction)hl);}
static void HAL_A7139_PWR_ENABLE(){GPIO_SetBits(RF_PWR_GPIO_PORT,RF_PWR_GPIO_PIN);}
static void HAL_A7139_PWR_DISABLE(){GPIO_ResetBits(RF_PWR_GPIO_PORT,RF_PWR_GPIO_PIN);}

static void HAL_RF_nIRQ_ON()
{
	RF_EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&RF_EXTI_InitStructure);
}
static void HAL_RF_nIRQ_OFF()
{
	RF_EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&RF_EXTI_InitStructure);
}

static void HAL_RF_SDIO_MO()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = RF_SDIO_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RF_SDIO_GPIO_PORT, &GPIO_InitStructure);
}
static void HAL_RF_SDIO_MI()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = RF_SDIO_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RF_SDIO_GPIO_PORT, &GPIO_InitStructure);
}

/*实现接口的方式*/
RF_Interface_T Implement_RF_Interface = 
{
	.Hal_Init            = RF_Hal_Init,
	.IS_RF_nIRQ_H        = HAL_IS_RF_nIRQ_H,
	.RF_SDIO_IN          = HAL_RF_SDIO_IN,
	.RF_MISO_IN          = HAL_RF_MISO_IN,
	.RF_MOSI_SET         = HAL_RF_MOSI_SET,
	.RF_SCLK_SET         = HAL_RF_SCLK_SET,
	.RF_nSCS_SET         = HAL_RF_nSCS_SET,
	.A7139_PWR_ENABLE    = HAL_A7139_PWR_ENABLE,
	.A7139_PWR_DISABLE   = HAL_A7139_PWR_DISABLE,
	.RF_SDIO_MO          = HAL_RF_SDIO_MO,
	.RF_SDIO_MI          = HAL_RF_SDIO_MI,
	.RF_nIRQ_ON          = HAL_RF_nIRQ_ON,
	.RF_nIRQ_OFF         = HAL_RF_nIRQ_OFF,
	.delay_ms            = ADP_delay_ms,
	.delay_us            = ADP_delay_us,
};

