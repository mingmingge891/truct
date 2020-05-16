/**
  ******************************************************************************
  * @file 
  * @author  mh.j
  * @version v2.0
  * @date    2016.3.17
  * @brief  物理层-RF驱动
  *       
  *         
  ******************************************************************************
  * @copy
***/ 
/********************Includes****************************/
#include "rf_433.h"
#include "string.h"

#define NCS_NOPS()            pRF_Interface->delay_us(1)/*片选延时*/
#define RF_DELAY_100US(x)     pRF_Interface->delay_us(100*x)/*(10微秒精确延时函数*/
#define RF_Delay_ms				    pRF_Interface->delay_ms

//基础频率，设置频率范围为420.500MHZ~452.375MHZ ,频道差为125KHZ
//#define A7139_BASE_FRE			  420.5f


//控制寄存器组A
typedef enum
{
	//寄存器8
	A7139_REG8_TX1		=	0,			//addr8 page0, 
	A7139_REG8_WOR1		=	1,			//addr8 page1, 
	A7139_REG8_WOR2		=	2,			//addr8 page2, 
	A7139_REG8_RF		=	3,			//addr8 page3, 
	A7139_REG8_POWER	=	4,			//addr8 page4, 
	A7139_REG8_AGCRC	=	5,			//addr8 page5, 
	A7139_REG8_AGCCON1	=	6,			//addr8 page6, 
	A7139_REG8_AGCCON2	=	7,			//addr8 page7, 
	A7139_REG8_GPIO		=	8,			//addr8 page8, 
	A7139_REG8_CKO		=	9,			//addr8 page9, 
	A7139_REG8_VCO		=	10,			//addr8 page10,
	A7139_REG8_CHG1		=	11,			//addr8 page11,
	A7139_REG8_CHG2		=	12,			//addr8 page12,
	A7139_REG8_FIFO		=	13,			//addr8 page13,
	A7139_REG8_CODE		=	14,			//addr8 page14,
	A7139_REG8_WCAL		=	15,			//addr8 page15,
}A7139_PAGE_A;
 
 
//控制寄存器组B
typedef enum
{
	//寄存器9
	A7139_REG9_TX2		=	0,		//addr9 page0, 
	A7139_REG9_IF1		=	1,		//addr9 page1,
	A7139_REG9_IF2		=	2,		//addr9 page2,
	A7139_REG9_ACK		=	3,		//addr9 page3,
	A7139_REG9_ART		=	4,		//addr9 page4,
}A7139_PAGE_B;
 
 
//Strobe命令
typedef enum
{
	A7139_WCR_CMD		=	0x00,	//写控制寄存器
	A7139_RCR_CMD		=	0x80,	//读控制寄存器
	A7139_WID_CMD		=	0x20,	//写ID
	A7139_RID_CMD		=	0xA0,	//读ID
	A7139_WFIFO_CMD		=	0x40,	//写FIFO
	A7139_RFIFO_CMD		=	0xC0,	//读FIFO	
	A7139_RESRF_CMD		=	0x70,	//复位RF
	A7139_RESTFIFO_CMD	=	0x60,	//复位发送FIFO
	A7139_RESRFIFO_CMD	=	0xE0,	//复位接收FIFO
	A7139_SLEEP_CMD		=	0x10,	//SLEEP模式
	A7139_IDLE_CMD		=	0x12,	//IDLE模式
	A7139_STBY_CMD		=	0x14,	//Standby模式
	A7139_PLL_CMD		=	0x16,	//PLL模式
	A7139_RX_CMD		=	0x18,	//RX模式
	A7139_TX_CMD		=	0x1A,	//TX模式
	A7139_TSLEEP_CMD	=	0x1C,	//Deep sleep 模式 三态
	A7139_PSLEEP_CMD	=	0x1F,	//Deep sleep 模式 上拉
}A7139_STROBE_CMD;

//控制寄存器
typedef enum 
{
	A7139_SCLOCK	=	0x00,		//系统时钟寄存器
	A7139_PLL1		=	0x01,		//PLL1
	A7139_PLL2		=	0x02,		//PLL2
	A7139_PLL3		=	0x03,		//PLL3
	A7139_PLL4		=	0x04,		//PLL4
	A7139_PLL5		=	0x05,		//PLL5
	A7139_PLL6		=	0x06,		//PLL6
	A7139_CRYSTAL	=	0x07,		//晶振设置
	A7139_PREG8S	=	0x08,		//寄存器组,由CRYSTAL控制切换
	A7139_PREG9S	=	0x09,		//寄存器组,由CRYSTAL控制切换
	A7139_RX1		=	0x0A,		//接收设置1
	A7139_RX2		=	0x0B,		//接收设置2
	A7139_ADC		=	0x0C,		//ADC
	A7139_PIN		=	0x0D,		//PIN
	A7139_CALIB		=	0x0E,		//Calibration
	A7139_MODE		=	0x0F,		//模式控制
}A7139_CREG;


RF_Property_t RF_Property;
RF_Flag_t RF_Flag;
RF_Interface_T *pRF_Interface = NULL;
RfIrq_callBackCB_t rf_irq_callBack;

static Car_t Car;
const rf_uint8 RMRfSettings[7][14] = {
  {0x1D, 0x41, 0x60, 0x27, 0x52, 0x00, 0x05, 0x13, 0xa9, 0x20, 0x3a, 0x40,  0x0A, 0x20},/*DR: 2.4kbps, DEV: +-36kHz, BBBW: 75.2kHz (crystal:20 ppm)*/
  {0x1D, 0xA1, 0x20, 0x4E, 0xA5, 0x00, 0x0A, 0x27, 0x52, 0x20, 0x48, 0x40,  0x0A, 0x20},/*DR: 4.8kbps, DEV: +-45kHz, BBBW: 95.3kHz (crystal:20 ppm)*/
  {0x1E, 0xD0, 0x00, 0x9D, 0x49, 0x00, 0x24, 0x4e, 0xa5, 0x20, 0x48, 0x40,  0x0A, 0x20},/*DR: 9.6kbps, DEV: +-45kHz, BBBW: 112.8kHz (crystal:20 ppm)*/
  {0x03, 0xD0, 0x00, 0x9D, 0x49, 0x01, 0x3D, 0x9D, 0x49, 0x20, 0x0F, 0x40,  0x0A, 0x1E},/*DR: 19.2kbps, DEV: +-9.6kHz, BBBW: 75.2kHz (crystal:20 ppm)*/
  {0x03, 0x68, 0x01, 0x3A, 0x93, 0x02, 0x78, 0x09, 0xD5, 0x00, 0x1F, 0x40,  0x0A, 0x20},/*DR: 38.4kbps, DEV: +-19.2kHz, BBBW: 83.2kHz (crystal:20 ppm)*/
  {0x06, 0x45, 0x01, 0xD7, 0xDC, 0x03, 0xB8, 0x0E, 0xBF, 0x00, 0x2E, 0x40,  0x0A, 0x2D},/*DR: 57.6kbps, DEV: +-28.8kHz, BBBW: 124.6kHz (crystal:20 ppm)*/
  {0x82, 0x68, 0x01, 0x3A, 0x93, 0x02, 0x78, 0x1D, 0x7E, 0x00, 0x5C, 0x40,  0x0A, 0x50} /*DR: 115.2kbps, DEV: +-57.6kHz, BBBW: 172.8kHz (crystal:20 ppm)*/
};
const rf_uint8 ID_Tab[4]={0x81,0x7E,0x2D,0xD4};/*ID code*/
const rf_uint32 A7139Config[]= /*470MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz)*/
{
//    0x08C7,   /*0000 1000 1100 0111*/
//    0x0A24,		/*PLL1 register,*/
//    0xB805,		/*PLL2 register,	470.001MHz*/
//    0x0000,		/*PLL3 register,*/
//    0x0A20,		/*PLL4 register, 10100010*/
//    0x0028,		/*PLL5 register,*/
//    0x0000,		/*PLL6 register,*/
//    0x0013,		/*CRYSTAL register,*/
//    0x0000,		/*PAGEA,*/
//    0x0000,		/*PAGEB,*/	
//    0x18D0,   /*RX1 register,*/
//    0x7009,		/*RX2 register,	       by preamble*/
//    0x4000,		/*ADC register,	*/   	
//    0x0800,		/*PIN CONTROL register,		Use Strobe CMD*/
//    0x4C45,		/*CALIBRATION register,*/
//    0x20C0		/*MODE CONTROL register, 	Use FIFO mode*/
	0x0021,		//SYSTEM CLOCK register,
		0x0A21,		//PLL1 register,
		0xDA05,		//PLL2 register,    433.301MHz
		0x0000,		//PLL3 register,
		0x0A20,		//PLL4 register,
		0x0024,		//PLL5 register,
		0x0000,		//PLL6 register,
		0x0001,		//CRYSTAL register,
		0x0000,		//PAGEA,
		0x0000,		//PAGEB,
		0x18D4,		//RX1 register,		IFBW=100KHz, ETH=1	
		0x7009,		//RX2 register,		by preamble
		0x4400,		//ADC register,
		0x0800,		//PIN CONTROL register,		Use Strobe CMD
		0x4845,		//CALIBRATION register,
		0x20C0		//MODE CONTROL register, 	Use FIFO mode

	
	
};
const rf_uint32 A7139Config_PageA[]=/*470MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz)*/
{
//    0xFB73,  
//    0x0000,		/*WOR1 register,*/
//    0xF800,		/*WOR2 register,*/
//    0x1107,		/*RFI register, 	Enable Tx Ramp up/down */ 
//    0x8170,		/*PM register,		CST=1*/
//    0x0201,		/*RTH register,*/
//    0x400F,		/*AGC1 register,*/	
//    0x2AC0,		/*AGC2 register, */
//    0xA041,		/*GIO register, 	GIO2=WTR, GIO1=WTR*/
//    0xD183,		/*CKO register*/
//    0x0004,		/*VCB register,*/
//    0x0A21,		/*CHG1 register, 	480MHz*/
//    0x0022,		/*CHG2 register, 	500MHz*/
//    0x003F,		/*FIFO register, 	FEP=63+1=64bytes*/
//    0x150B,	        /*CODE register, Preamble=4bytes, ID=2bytes, crc*/
//    0x0000		/*WCAL register, */ 
	0xF706,		//TX1 register, 	Fdev = 37.5kHz
		0x0000,		//WOR1 register,
		0xF800,		//WOR2 register,
		0x1107,		//RFI register, 	Enable Tx Ramp up/down
		0x0170,		//PM register,
		0x0201,		//RTH register,
		0x400F,		//AGC1 register,
		0x2AC0,		//AGC2 register,
	    0x0041,   	//GIO register      GIO1->WTR GIO2->WTR 
		0xD281,		//CKO register
		0x0004,		//VCB register,
		0x0A21,		//CHG1 register, 	430MHz
		0x0022,		//CHG2 register, 	435MHz
		0x003F,		//FIFO register, 	FEP=63+1=64bytes
		0x1507,		//CODE register, 	Preamble=4bytes, ID=4bytes
		0x0000		//WCAL register,
	
	
};
const rf_uint32 A7139Config_PageB[]=   /*470MHz, 10kbps (IFBW = 100KHz, Fdev = 18.75KHz)*/
{
//    0x0B37,	/*0x0B7F-->0x0B37 733 721功率测试*/
//    0x81EC,     /*IF1 register, 	Enable Auto-IF, IF=100KHz     0x8400,IF=200KHz */
//    0x0000,	/*IF2 register,*/
//    0x0000,	/*ACK register,*/
//    0x0000	/*ART register,*/
	
	0x0337,		//TX2 register, 	
		0x8400,		//IF1 register, 	Enable Auto-IF, IF=200KHz
		0x0000,		//IF2 register,		频率偏移为0
		0x0000,		//ACK register,
		0x0000		//ART register,

};
//const rf_uint32 Freq_Cal_Tab[]=
//{
//	0x0A24, 0xB805,			/*470.001MHz*/
//	0x0A26, 0x4805,			/*490.001MHz*/
//	0x0A27, 0xD805,     /*510.001MHz f0*/
//	
//	0x0A24, 0xBA05,     /*470.101MHz,f1*/
//	0x0A24, 0xC205,     /*470.501MHz,f2*/
//	0x0A24, 0xCA05,     /*470.901MHz,f3*/
//	0x0A24, 0xD205,     /*471.301MHz,f4*/
//	0x0A24, 0xDA05,     /*471.701MHz,f5*/
//	0x0A24, 0xE205,     /*472.101MHz,f6*/
//	0x0A24, 0xEA05,     /*472.501MHz,f7*/
//	0x0A25, 0x1605,     /*474.701MHz,f8*/
//	0x0A25, 0x1E05,     /*475.101MHz,f9*/
//	0x0A25, 0x2605,     /*475.501MHz,f10*/
//	0x0A25, 0x2E05,     /*475.901MHz,f11*/
//	0x0A25, 0x3605,     /*476.301MHz,f12*/
//	0x0A24, 0xBC05,     /*470.201MHz,f13*/
//	0x0A26, 0x4C05,     /*490.201MHz,f14*/
//	0x0A27, 0xDC05,     /*510.201MHz,f15*/

//	0x0A21, 0xD406,			/*433.001MHz f16*/
//	0x0A21, 0xDC06,			/*433.401MHz f17*/
//	0x0A21, 0xE406,     /*433.801MHz f18*/
//	0x0A21, 0xEC06,			/*434.201MHz f19*/
//	0x0A22, 0x1C06,			/*436.601MHz f20*/
//	0x0A22, 0x2406,     /*437.001MHz f21*/
//	0x0A22, 0x2C06,     /*437.401MHz f22*/
//	0x0A22, 0x3406,     /*437.801MHz f23*/
//	
//};
//小数部分相差2048 400Khz
const rf_uint32 Freq_Cal_Tab[]=
{
	//0x0A24, 0xB805,			/*470.001MHz*/
	//0x0A26, 0x4805,			/*490.001MHz*/
	0x0027, 0xD805,     /*510.001MHz f0*/
	
	0x0024, 0xBA05,     /*470.101MHz,f1*/
	0x0024, 0xC205,     /*470.501MHz,f2*/
	0x0024, 0xCA05,     /*470.901MHz,f3*/
	0x0024, 0xD205,     /*471.301MHz,f4*/
	0x0024, 0xDA05,     /*471.701MHz,f5*/
	0x0024, 0xE205,     /*472.101MHz,f6*/
	0x0024, 0xEA05,     /*472.501MHz,f7*/
	0x0025, 0x1605,     /*474.701MHz,f8*/
	0x0025, 0x1E05,     /*475.101MHz,f9*/
	0x0025, 0x2605,     /*475.501MHz,f10*/
	0x0025, 0x2E05,     /*475.901MHz,f11*/
	0x0025, 0x3605,     /*476.301MHz,f12*/
	//0x0024, 0xBC05,     /*470.201MHz,f13*/
	0x0025, 0x3E05,     /*476.701MHz,f13*/
	0x0026, 0x4C05,     /*490.201MHz,f14*/
	0x0027, 0xDC05,     /*510.201MHz,f15*/

	
	0x0021, 0xD406,			/*433.001MHz f16*/
	0x0021, 0xDC06,			/*433.401MHz f17*/
	0x0021, 0xE406,     /*433.801MHz f18*/
	0x0021, 0xEC06,			/*434.201MHz f19*/
	0x0022, 0x1C06,			/*436.601MHz f20*/
	0x0022, 0x2406,     /*437.001MHz f21*/
	0x0022, 0x2C06,     /*437.401MHz f22*/
	0x0022, 0x3406,     /*437.801MHz f23*/
	0x0022, 0x4C06,     /*439.001MHz f24*/
	0x0022, 0x5406,     /*439.401MHz f25*/
	0x0022, 0x5C06,     /*439.801MHz f26*/
	0x0022, 0x6406,     /*440.201MHz f27*/
	0x0022, 0x8C06,     /*442.201MHz f28*/
	0x0022, 0x9406,     /*442.601MHz f29*/
	0x0022, 0x9C06,     /*443.001MHz f30*/
	0x0022, 0xA406,     /*443.401MHz f31*/
	
};

RF_CFG_T RF_Config = {
  DEF_TLDC,             /*休眠周期*/
  DEF_RF_HOP,           /*射频频点*/
  DEF_RF_PWR,           /*射频功率*/
  DEF_RF_DR,            /*射频速率*/
  DEF_TX_COUNT,         /*发射计数*/
  0                		  /*空闲计时*/
};
/*************************************************************
函数名称:void RF_Drv_WriteReg(void)
功    能:SPI单字节收发送
参    数:地址和配置字
返 回 值:
说    明:无
*************************************************************/
rf_uint8 RF_TxRx_Byte(rf_uint8 tData)
{
  rf_uint8 rData = 0,i;
	pRF_Interface->RF_MOSI_SET(1);
  pRF_Interface->RF_MOSI_SET(1);
  for(i = 8; i > 0; i--)
  {
    rData <<= 1;
		
    if(pRF_Interface->RF_MISO_IN())
    {
        rData |= 0x01;
    }
    if(tData&0x80)
    {
        pRF_Interface->RF_MOSI_SET(1);
    }
    else
    {
				
        pRF_Interface->RF_MOSI_SET(0);
    }
    pRF_Interface->RF_SCLK_SET(1);
		
    tData <<= 1;
    pRF_Interface->RF_SCLK_SET(0);
  }
  return rData;
}
/*************************************************************
函数名称:RF_Drv_WriteReg
功    能:寄存器写入值
参    数:地址和配置字
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_WriteReg(rf__IO rf_uint8 addr, rf__IO rf_uint32 value)
{
#if defined (A7139)
    rf_uint8 i;
    /*Set SCS=0 to Enable SPI interface*/
    pRF_Interface->RF_nSCS_SET(0);
		
    /*change SDIO output*/
    pRF_Interface->RF_SDIO_MO();
	  
    /*Enable write operation of control registers*/
    addr |= CMD_CTRLW;/*#define CMD_CTRLW 0x00*/
    /*Assign control register’s address by input variable addr*/
    for(i = 0; i < 8; i++)
    {
        if(addr & 0x80)
            pRF_Interface->RF_MOSI_SET(1);/*bit=1*/
        else
            pRF_Interface->RF_MOSI_SET(0);/*bit=0*/
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(1);
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(0);
        addr = addr << 1;
    }
    NCS_NOPS();
    /*Assign control register’s value by input variable dataByte*/
    for(i = 0; i < 16; i++)
    {
        if(value & 0x8000)
            pRF_Interface->RF_MOSI_SET(1);
        else
            pRF_Interface->RF_MOSI_SET(0);
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(1);
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(0);
        value = value << 1;
    }
    /*Set SCS=1 to disable SPI interface.*/
    pRF_Interface->RF_nSCS_SET(1);
#else
    pRF_Interface->RF_nSCS_SET(0);
    NCS_NOPS();
    RF_TxRx_Byte(addr+0x80);    
    RF_TxRx_Byte(value);
    pRF_Interface->RF_nSCS_SET(1);
#endif /* A7139 */    
}
/*************************************************************
函数名称:RF_Drv_ReadReg
功    能:读取寄存器当前值
参    数:addr->地址
返 回 值:状态寄存器当前值
说    明:无
*************************************************************/
rf_uint32 RF_Drv_ReadReg(rf__IO rf_uint8 addr) 
{
#if defined (A7139)
  rf_uint8 i;
  static rf_uint32 tmp;
  pRF_Interface->RF_nSCS_SET(0);
  pRF_Interface->RF_SDIO_MO();
  addr |= CMD_CTRLR;
  for(i = 0; i < 8; i++)
  {
      if(addr & 0x80)
          pRF_Interface->RF_MOSI_SET(1);
      else
          pRF_Interface->RF_MOSI_SET(0);
      NCS_NOPS();
      pRF_Interface->RF_SCLK_SET(1);
      NCS_NOPS();
      pRF_Interface->RF_SCLK_SET(0);
      addr = addr << 1;
  }
  pRF_Interface->RF_MOSI_SET(1);
  pRF_Interface->RF_SDIO_MI();
  NCS_NOPS();
  for(i = 0; i < 16; i++)
  {
      if(pRF_Interface->RF_SDIO_IN())
          tmp = (tmp << 1) | 0x01;
      else
          tmp = tmp << 1;
      NCS_NOPS();
      pRF_Interface->RF_SCLK_SET(1);
      NCS_NOPS();
      pRF_Interface->RF_SCLK_SET(0);
  }
  pRF_Interface->RF_nSCS_SET(1);  
  pRF_Interface->RF_SDIO_MO();
  return tmp;
#else  
  rf_uint8 value;
  pRF_Interface->RF_nSCS_SET(0);
  NCS_NOPS();
  RF_TxRx_Byte(addr);
  value = RF_TxRx_Byte(0);
  pRF_Interface->RF_nSCS_SET(1);
  return value;
#endif /* A7139 */ 
}
/*************************************************************
函数名称:RF_Drv_GetItStatusAll
功    能:RF中断状态寄存器1、2
参    数:无
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_GetItStatusAll(void)
{
   RF_Drv_ReadReg(InterruptStatus1);
   RF_Drv_ReadReg(InterruptStatus2);
   RF_Flag.rf_IRQ = FALSE;
}
/*************************************************************
函数名称:RF_Drv_WriteReg
功    能:载波设置
参    数:载波频率
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_SetCarrier(rf_uint16 freCarrier)
{
    rf_uint8  hbsel = 0;
    rf_uint8  fb    = 0;
    rf_uint32  fc    = 0;
    if(freCarrier < 480) {
            hbsel = 0;
            fb = (freCarrier / 10) - 24;
            fc = (freCarrier % 10) * 6400; 	   
    } else {
            hbsel = 1 << 5;
            fb = (freCarrier / 20) - 24;
            fc = (freCarrier % 20) * (6400 / 2); 	   		
    }
    Car.FreBand = (0x40 | hbsel | fb);
    Car.Fre1 = (fc/256);
    Car.Fre0 = (fc%256);
    RF_Drv_WriteReg(FrequencyBandSelect, Car.FreBand);		 
    RF_Drv_WriteReg(NominalCarrierFrequency1, Car.Fre1);		 
    RF_Drv_WriteReg(NominalCarrierFrequency0, Car.Fre0); 
}
/*************************************************************
函数名称:rf_uint8 RF_Drv_Init(void)
功    能:跳频设置 注意：如果使用必需先写入步进所写入通道，写入通道结束会引起IC调整载波频率  
参    数:频率通道设置
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_Hop_Set(rf_uint8 ucChnl)
{
    RF_Drv_WriteReg(FrequencyHoppingStepSize , (DEF_RF_FRE_STEP / 10));
    RF_Drv_WriteReg(FrequencyHoppingChannelSelect , ucChnl);
}
/*******************************************************************************
函 数：RF_SetTXPower
说 明：设置RF发送功率
参 数：power ：0-7  发送功率
返 回：null
日 期：2010.11.22
*******************************************************************************/
void RF_Drv_SetTXPower(rf__IO rf_uint8 tPower)
{
  rf_uint8 nPower = tPower;
  if(nPower > 7)
  {
    nPower = DEF_RF_PWR;
  }
  RF_Drv_WriteReg(TXPower, nPower);
}
/*************************************************************
函数名称:void RF_Drv_DataRate_Set(__I U08 nBaud)
功    能:数据发射速率设置
参    数:数据发射速率
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_DataRate_Set(rf__IO rf_uint8 nBaud)
{
    rf_uint8 nRate = nBaud;
    if(nRate > 7) {
      nRate = 2;   /*  如果入参不合理，设为9600    */
    }
    RF_Drv_WriteReg( IFFilterBandwidth, RMRfSettings[nRate][0] ); 
    RF_Drv_WriteReg( ClockRecoveryOversamplingRatio, RMRfSettings[nRate][1]);
    RF_Drv_WriteReg( ClockRecoveryOffset2, RMRfSettings[nRate][2]); 
    RF_Drv_WriteReg( ClockRecoveryOffset1, RMRfSettings[nRate][3]);
    RF_Drv_WriteReg( ClockRecoveryOffset0, RMRfSettings[nRate][4]);
    RF_Drv_WriteReg( ClockRecoveryTimingLoopGain1, RMRfSettings[nRate][5]);
    RF_Drv_WriteReg( ClockRecoveryTimingLoopGain0, RMRfSettings[nRate][6]);
    RF_Drv_WriteReg( TXDataRate1, RMRfSettings[nRate][7]);
    RF_Drv_WriteReg( TXDataRate0, RMRfSettings[nRate][8]);
    RF_Drv_WriteReg( ModulationModeControl1, RMRfSettings[nRate][9]);
    RF_Drv_WriteReg( FrequencyDeviation, RMRfSettings[nRate][10]);
    RF_Drv_WriteReg( AFCLoopGearshiftOverride, RMRfSettings[nRate][11]);
    RF_Drv_WriteReg( AFCTimingControl, RMRfSettings[nRate][12]);
    RF_Drv_WriteReg( AFCLimiter, RMRfSettings[nRate][13]);
}
/*************************************************************
函数名称:void RF_Drv_PreambleLength_Set(rf_uint32 uiBit)
功    能:设置RF前导码长度
参    数:前导码长度
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_PreambleLength_Set(rf_uint32 uiBit)
{
    if(uiBit < 4) {                 
          RF_Drv_WriteReg(PreambleLength, 0x01);/* 设为最小值*/	
      } 
      else if(uiBit < 8220){
          RF_Drv_WriteReg(PreambleLength, uiBit / 4);/* 设置引导码长度*/
      } 
      else {
          RF_Drv_WriteReg(PreambleLength, 255);/* 设置引导码为最大值*/
      }
}
/*************************************************************
函数名称:void RF_Drv_PreamblsheThreold_Set(rf_uint32 uiBit)
功    能:设置RF前导码门限值
参    数:前导码门限
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_PreamblsheThreold_Set(rf_uint32 uiBit) 
{
    if(uiBit < 4) {                 
            RF_Drv_WriteReg(PreambleDetectionControl, 0x08 | 0x02);/* 设置引导码门槛 */	
      } 
      else if(uiBit < 128){
          uiBit   = uiBit / 4;
          uiBit <<= 3;
          RF_Drv_WriteReg(PreambleDetectionControl, uiBit | 0x02);/* 设置引导码长度,位于bit7~bit3 */
      } 
      else {
          RF_Drv_WriteReg(PreambleDetectionControl, 0xf8 | 0x02);/* 设置引导码为最大值*/
      }
}
/*************************************************************
函数名称:void RF_Drv_StrobeCMD(rf_uint8 cmd)
功    能:Strobe Command
参    数:cmd
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_StrobeCMD(rf_uint8 cmd)
{
    rf_uint8 i;
    pRF_Interface->RF_nSCS_SET(0);
    NCS_NOPS();
    for(i = 0; i < 8; i++)
    {
        if(cmd & 0x80)
            pRF_Interface->RF_MOSI_SET(1);
        else
            pRF_Interface->RF_MOSI_SET(0);
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(1);
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(0);
        cmd = cmd << 1;
    }
    pRF_Interface->RF_nSCS_SET(1);
    NCS_NOPS();
}
/*************************************************************
函数名称:void A7139_WritePageB(unsigned char address, unsigned int dataWord)
功    能:
参    数:address dataWord
返 回 值:无
说    明:无
*************************************************************/
void A7139_WritePageB(rf_uint8 address, rf_uint32 dataWord)
{
    rf_uint32 tmp;
    tmp = address;
    tmp = ((tmp << 7) | A7139Config[CRYSTAL_REG]);
    RF_Drv_WriteReg(CRYSTAL_REG, tmp);
    RF_Drv_WriteReg(PAGEB_REG, dataWord);
}
/*************************************************************
函数名称:void A7139_WritePageA(rf_uint8 address, rf_uint32 dataWord)
功    能:
参    数:address dataWord
返 回 值:无
说    明:无
*************************************************************/
void A7139_WritePageA(rf_uint8 address, rf_uint32 dataWord)
{
    rf_uint32 tmp;
    tmp = address;
    tmp = ((tmp << 12) | A7139Config[CRYSTAL_REG]);  /*it's different here*/
    RF_Drv_WriteReg(CRYSTAL_REG, tmp);
    RF_Drv_WriteReg(PAGEA_REG, dataWord);
}

/*************************************************************
函数名称:void RF_Drv_Config(void)
功    能:配置RF的寄存器
参    数:无
返 回 值:无
说    明:无
*************************************************************/
void A7139_Config(void)
{
    rf_uint8 i;
    for(i=0; i<8; i++)
        RF_Drv_WriteReg(i, A7139Config[i]);

    for(i=10; i<16; i++)
        RF_Drv_WriteReg(i, A7139Config[i]);

    for(i=0; i<16; i++)
        A7139_WritePageA(i, A7139Config_PageA[i]);

    for(i=0; i<5; i++)
        A7139_WritePageB(i, A7139Config_PageB[i]);
	
    /*for check*/
    if((unsigned int)RF_Drv_ReadReg(SYSTEMCLOCK_REG)!= A7139Config[SYSTEMCLOCK_REG])
    {
       //while(1);
    }
}

/*************************************************************************************************************************
* 函数		:	void A7139_SetChannel(u8 Channel)
* 功能		:	A7139设置通信信道
* 参数		:	Channel:通信信道0-650
* 返回		:	无
* 依赖		:	底层宏定义
* 作者		:	cp1300@139.com
* 时间		:	2016-01-02
* 最后修改	: 	2016-01-02
* 说明		: 	用于设置通信频道
				Channel*0.125/0.0125,最小频率偏差为12.5KHZ，设置通道间隔为125KHZ(最少为100KHZ)
*************************************************************************************************************************/
void A7139_SetChannel(u16 Channel)
{
	A7139_WritePageB(A7139_REG9_IF2,Channel*10);
}

/*************************************************************************************************************************
* 函数		:	void A7139_SetTrafficRate(u8 Rate)
* 功能		:	A7139设置通信速率，单位Kbps
* 参数		:	Rate:通信速率，单位Kbps
* 返回		:	无
* 依赖		:	底层宏定义
* 作者		:	cp1300@139.com
* 时间		:	2016-01-02
* 最后修改	: 	2016-01-02
* 说明		: 	用于设置通信速率
				范围2-100Kbps
				设置系统时钟2分频，设置为12.8MHZ后如果IFBW设置过小会导致初始化时自动校准失败
				如果设置为50会出现校准失败
*************************************************************************************************************************/
void A7139_SetTrafficRate(u8 Rate)
{
	u16 SDR;
	
	if(Rate < 2) Rate = 2;
	if(Rate > 100) Rate = 100;
	//IFBW设置 DMOS=1 64分频  ETH=1	 CSC=0 FCSCK=12.8MHZ
	if(Rate <= 50)	//IFBW=100KHZ 
	{
		RF_Drv_WriteReg(A7139_RX1, 0x18D0 | (1<<2));
	}
	else 	//IFBW=100KHZ
	{
		RF_Drv_WriteReg(A7139_RX1, 0x18D0 | (1<<2));
	}
	SDR = 100/Rate;
	SDR -= 1;			//计算波特率分频值
 
	RF_Drv_WriteReg(A7139_SCLOCK,0x0021|(SDR<<9)); //CSC=1 GRC=1	SDR
}
/*************************************************************************************************************************
* 函数		:	void A7139_SetBaseFreq(float RfFreq)
* 功能		:	A7139 配置RF基础频率
* 参数		:	rfFreq:RF频率,单位MHz
* 返回		:	无
* 依赖		:	底层宏定义
* 作者		:	cp1300@139.com
* 时间		:	2015-07-19
* 最后修改	: 	2015-07-19
* 说明		: 	初始化配置
*************************************************************************************************************************/
void A7139_SetBaseFreq(float RfFreq)
{
	 float  divFreq = RfFreq / 12.800f;  
	 u8  intFreq = (u8)(divFreq); //integer part
	 float  fltFreq = divFreq - intFreq * 1.000f; //fraction part
	 u16 fpFreg	= (u16)(fltFreq * 65536);  //FP register val
	 u16 orgVal;
	
	 RF_Drv_StrobeCMD(A7139_STBY_CMD); //enter stand-by mode
	 //AFC[15:15] = 0
	 orgVal = A7139Config[A7139_PLL3] & 0x7FFF;
	 RF_Drv_WriteReg(A7139_PLL3,orgVal);
	 //RFC[15:12] = 0000
	 orgVal = A7139Config[A7139_PLL6] & 0x0FFF;
	 RF_Drv_WriteReg(A7139_PLL6,orgVal);
	 //MD1[12:12]=0,1
	 if(RfFreq < 860)	//433-510
		orgVal = A7139Config[A7139_PLL4] & 0xEFFF;
	 else	 //868-915
		orgVal = A7139Config[A7139_PLL4] | 0x1000;
	 RF_Drv_WriteReg(A7139_PLL4,orgVal);
	 		//IP[8:0] = intg
	 orgVal = A7139Config[A7139_PLL1] & 0xFF00;
//	 RF_Drv_WriteReg(A7139_PLL1,orgVal|intFreq);
	 RF_Drv_WriteReg(A7139_PLL1,orgVal|Freq_Cal_Tab[RF_433_470.FP*2]);//Freq_Cal_Tab[0] intFreq
	 		//FP[15:0] =  fpFreg
//	 RF_Drv_WriteReg(A7139_PLL2,fpFreg); 
	 RF_Drv_WriteReg(A7139_PLL2,Freq_Cal_Tab[RF_433_470.FP*2+1]); //fpFreg
	//FPA[15:0] = 0x0000,
	 A7139_WritePageB(A7139_REG9_IF2,0x0000);	//偏移为0
}

/*************************************************************************************************************************
* 函数		:	void A7139_SetTxPowerSupply(u8 PowerSupply)
* 功能		:	设置A7139发射功率
* 参数		:	PowerSupply：功率等级0-7
* 返回		:	无
* 依赖		:	底层宏定义
* 作者		:	cp1300@139.com
* 时间		:	2016-01-02
* 最后修改	: 	2016-01-02
* 说明		: 	用于设置发射功率
				由于不同频率下TX驱动电流与PA驱动电流并不相同，因此按照文档设置
				目前只支持433频段设置，其它频段请按照文档进行设置。
*************************************************************************************************************************/
void A7139_SetTxPowerSupply(u8 PowerSupply)
{	
	if(PowerSupply>6)PowerSupply=6;	//最大功率为6
	if(PowerSupply == 0)
	{
		A7139_WritePageB(A7139_REG9_TX2, 0x0300);	//功率最小-34dBm,PAC=0,TDC=0,TBG=增益
	}
	else
	{
		A7139_WritePageB(A7139_REG9_TX2, 0x0300|(1<<5)|(1<<3)|PowerSupply);	//PAC=1,TDC=1,TBG=增益
	}
}

/************
函数		:	bool A7139_Init(u8 Channel, u16 RfID, u8 PowerSupply, u8 Rate)
* 功能		:	A7139初始化
* 参数		:	Channel:通信信道0-80，RfID:RF ID,PowerSupply:发射功率0-7;Rate:通信波特率2-100Kbps
* 返回		:	TRUE:成功;FALSE:失败
* 依赖		:	底层宏定义
* 作者		:	cp1300@139.com
* 时间		:	2015-07-19
* 最后修改	: 	2016-01-02
* 说明		: 	RF_ID：用于区分网络
				2-100Kbps频率间隔至少为100KHZ,150~200KHZ频道间隔必须大于200KHZ
********************************************************************************/
bool A7139_Init(u8 Channel, u16 RfID, u8 Rate)
{
	
	if(Rate < 2) Rate = 2;
	if(Rate > 100) Rate = 100;


	

	return TRUE;
}
/************************************************************************
**  ByteSend
************************************************************************/
void ByteSend(rf_uint8 src)
{
    rf_uint8 i;
    for(i = 0; i < 8; i++)
    {
        if(src & 0x80)
            pRF_Interface->RF_MOSI_SET(1);
        else
            pRF_Interface->RF_MOSI_SET(0);
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(1);
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(0);
				
        src = src << 1;
    }
}
/************************************************************************
**  ByteRead
************************************************************************/
unsigned char ByteRead(void)
{
    rf_uint8 i,tmp;
    pRF_Interface->RF_MOSI_SET(1);
    pRF_Interface->RF_SDIO_MI();
    for(i = 0; i < 8; i++)
    {
        if(pRF_Interface->RF_SDIO_IN())
            tmp = (tmp << 1) | 0x01;
        else
            tmp = tmp << 1;
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(1);
        NCS_NOPS();
        pRF_Interface->RF_SCLK_SET(0);
    }
    pRF_Interface->RF_SDIO_MO(); 
    return tmp;
}
/*************************************************************
函数名称:PHY_STATUS A7139_WriteID(void)
功    能:配置RF的寄存器
参    数:无
返 回 值:无
说    明:无
*************************************************************/
PHY_STATUS A7139_WriteID(void)
{
    rf_uint8 i;
    rf_uint8 d1,d2,d3,d4;
    
    pRF_Interface->RF_nSCS_SET(0);
    ByteSend(CMD_IDW);
    for (i=0; i < 4; i++)
      ByteSend(ID_Tab[i]);
    pRF_Interface->RF_nSCS_SET(1);
    pRF_Interface->RF_nSCS_SET(0);
    /*for check*/
    ByteSend(CMD_IDR);
    d1 = ByteRead();
    d2 = ByteRead();
    d3 = ByteRead();
    d4 = ByteRead();
    
    pRF_Interface->RF_nSCS_SET(1);
    
    if((d1!=ID_Tab[0])||(d2!=ID_Tab[1])||(d3!=ID_Tab[2])||(d4!=ID_Tab[3]))
      return STATUS_ERROR_ID_RW;
    return STATUS_SUCCESS_SPI_LOGIC;
}
/*************************************************************
函数名称:rf_uint32 A7139_ReadPageA(rf_uint8 address)
功    能:
参    数:address
返 回 值:无
说    明:无
*************************************************************/
rf_uint32 A7139_ReadPageA(rf_uint8 address)
{
    rf_uint32 tmp;
    tmp = address;
    tmp = ((tmp << 12) | A7139Config[CRYSTAL_REG]);
    RF_Drv_WriteReg(CRYSTAL_REG, tmp);
    tmp = RF_Drv_ReadReg(PAGEA_REG);
    /*here lost  A7108_WriteReg(CRYSTAL_REG, temp_Crystal_Reg);
    is it because never change CRYSTAL_REG?*/
    return tmp; 
}
/*************************************************************************************************************************
* 函数		:	bool A7139_Cali(void)
* 功能		:	A7139 校准
* 参数		:	无
* 返回		:	TRUE:校准成功;FALSE:校准失败
* 依赖		:	底层宏定义
* 作者		:	cp1300@139.com
* 时间		:	2015-07-19
* 最后修改	: 	2015-01-02
* 说明		: 	A7139 频率校准
*************************************************************************************************************************/
bool A7139_Cali(void)
{
	u16  fbcf;	//IF Filter
	u16  vbcf;	//VCO Current
	u16  vccf;	//VCO Band
	u16 tmp;
	
    //IF calibration procedure @STB state
	RF_Drv_WriteReg(A7139_MODE, A7139Config[A7139_MODE] | 0x0802);			//IF Filter & VCO Current Calibration
     do{
		tmp = RF_Drv_ReadReg(A7139_MODE);
			  RF_Delay_ms(1);
     }while(tmp & 0x0802);
    //for check(IF Filter)
     tmp = RF_Drv_ReadReg(A7139_CALIB);
     //fb = tmp & 0x0F;
     //fcd = (tmp>>11) & 0x1F;
     fbcf = (tmp>>4) & 0x01;
     if(fbcf)
     {
		 //FBCF：IF 滤波器自动校准标志（只读）
		return FALSE;
     }
	//for check(VCO Current)
     tmp = A7139_ReadPageA(A7139_REG8_VCO);
	//vcb = tmp & 0x0F;
	vccf = (tmp>>4) & 0x01;
	if(vccf)
	{
        return FALSE;
     }
    //RSSI Calibration procedure @STB state
	RF_Drv_WriteReg(A7139_ADC, 0x4C00);									//set ADC average=64
     A7139_WritePageA(A7139_REG8_WOR2, 0xF800);								//set RSSC_D=40us and RS_DLY=80us
	A7139_WritePageA(A7139_REG8_TX1, A7139Config_PageA[A7139_REG8_TX1] | 0xE000);	//set RC_DLY=1.5ms
     RF_Drv_WriteReg(A7139_MODE, A7139Config[A7139_MODE] | 0x1000);			//RSSI Calibration
     do{
		tmp = RF_Drv_ReadReg(A7139_MODE);
     }while(tmp & 0x1000);
	RF_Drv_WriteReg(A7139_ADC, A7139Config[A7139_ADC]);
     A7139_WritePageA(A7139_REG8_WOR2, A7139Config_PageA[A7139_REG8_WOR2]);
	A7139_WritePageA(A7139_REG8_TX1, A7139Config_PageA[A7139_REG8_TX1]);
    //VCO calibration procedure @STB state
	RF_Drv_WriteReg(A7139_MODE, A7139Config[A7139_MODE] | 0x0004);		//VCO Band Calibration
	do{
		tmp = RF_Drv_ReadReg(A7139_MODE);
	}while(tmp & 0x0004);
		//for check(VCO Band)
	tmp = RF_Drv_ReadReg(A7139_CALIB);
	//vb = (tmp >>5) & 0x07;
	vbcf = (tmp >>8) & 0x01;
	if(vbcf)
	{
		return FALSE;
	}
	
	return TRUE;
}
/*************************************************************
函数名称:rf_uint8 A7139_Cal(void)
功    能:配置RF的寄存器
参    数:无
返 回 值:无
说    明:无
*************************************************************/
rf_uint8 A7139_Cal(void)
{
    rf_uint8 i;
    rf_uint8 fbcf;/*IF Filter*/
    rf_uint8 vbcf;/*VCO Current*/
    rf_uint8 vccf;/*VCO Band*/
    static rf_uint32 tmp = 0;
    
    /*IF calibration procedure @STB state*/
    RF_Drv_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0802);/*IF Filter & VCO Current Calibration*/
    do{
         tmp = RF_Drv_ReadReg(MODE_REG);
				 RF_Delay_ms(1);
				 if(tmp == 0XFFFFFFFF){
						if(++i == 200){
								return 0;
						}
				 }	
    }while(tmp & 0x0802);
    /*for check(IF Filter)*/
    tmp = RF_Drv_ReadReg(CALIBRATION_REG);
    fbcf = (tmp>>4) & 0x01;
    if(fbcf)
    {
        //Err_State();
				return 0x00;
    }
    /*for check(VCO Current)*/
    tmp = A7139_ReadPageA(VCB_PAGEA);
    vccf = (tmp>>4) & 0x01;
    if(vccf)
    {
        //Err_State();
				return 0x00;
    }
    /*RSSI Calibration procedure @STB state*/
    RF_Drv_WriteReg(ADC_REG, 0x4C00);/*set ADC average=64*/
    A7139_WritePageA(WOR2_PAGEA, 0xF800);/*set RSSC_D=40us and RS_DLY=80us*/
    A7139_WritePageA(TX1_PAGEA, A7139Config_PageA[TX1_PAGEA] | 0xE000);/*set RC_DLY=1.5ms*/
    RF_Drv_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x1000);/*RSSI Calibration*/
    do{
        tmp = RF_Drv_ReadReg(MODE_REG);
    }while(tmp & 0x1000);
    RF_Drv_WriteReg(ADC_REG, A7139Config[ADC_REG] | 0x8000);
    A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA]);
    A7139_WritePageA(TX1_PAGEA, A7139Config_PageA[TX1_PAGEA]);
    /*VCO calibration procedure @STB state*/
    for(i=0; i<3; i++)
    {
       // RF_Drv_WriteReg(PLL1_REG, Freq_Cal_Tab[i*2]);
       // RF_Drv_WriteReg(PLL2_REG, Freq_Cal_Tab[i*2+1]);
        RF_Drv_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0004);/*VCO Band Calibration*/
        do{
                tmp = RF_Drv_ReadReg(MODE_REG);
        }while(tmp & 0x0004);
        /*for check(VCO Band)*/
        tmp = RF_Drv_ReadReg(CALIBRATION_REG);
        vbcf = (tmp >>8) & 0x01;
        if(vbcf)
        {
           //Err_State();
					 return 0x00;
        }
    }
    /*设置频点*/
   // RF_Drv_WriteReg(PLL1_REG, Freq_Cal_Tab[(RF_433_470.FP + 2)*2]);
   // RF_Drv_WriteReg(PLL2_REG, Freq_Cal_Tab[(RF_433_470.FP + 2)*2+1]);
		/*固定零频点*/
//		RF_Drv_WriteReg(PLL1_REG, Freq_Cal_Tab[0]);
//    RF_Drv_WriteReg(PLL2_REG, Freq_Cal_Tab[1]);
    RF_Drv_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0004);/*VCO Band Calibration*/
    do{
      tmp = RF_Drv_ReadReg(MODE_REG);
    }while(tmp & 0x0004);
    /*for check(VCO Band)*/
    tmp = RF_Drv_ReadReg(CALIBRATION_REG);
    vbcf = (tmp >>8) & 0x01;
    if(vbcf)
    {
        //Err_State();
			  return 0x00;
    }
		return 0x01;
}
/*************************************************************
函数名称:rf_uint8 RF_Drv_Init(void)
功    能:配置RF的寄存器
参    数:无
返 回 值:无
说    明:无
*************************************************************/
rf_uint8 RF_Drv_Init(void)
{
	rf_uint8 val = 0;
#if defined (A7139)
	for(val = 0;val < 3;val++)
	{
		pRF_Interface->RF_nIRQ_OFF();
		RF_Flag.rf_Init_Complete = FALSE;
		RF_Flag.rf_IRQ = FALSE;
		pRF_Interface->A7139_PWR_DISABLE();
    RF_Delay_ms(500);
    pRF_Interface->A7139_PWR_ENABLE();
    RF_Delay_ms(200);
    
    pRF_Interface->RF_nSCS_SET(1);
    pRF_Interface->RF_SCLK_SET(0);
    pRF_Interface->RF_MOSI_SET(1);
    
		pRF_Interface->RF_nSCS_SET(0);
    RF_Drv_StrobeCMD(CMD_RF_RST);
		pRF_Interface->RF_nSCS_SET(1);
		
    RF_Delay_ms(100);
    
    A7139_Config();
		A7139_SetBaseFreq(RF_433_470.RF_Base);//设置基础频率
	  A7139_SetTrafficRate(100);//设置通信速率
		
    RF_Delay_ms(100);
    A7139_WriteID();
    if(!A7139_Cali())
		{
			continue;
		}
		
		A7139_SetTxPowerSupply(6);		//设置发射功率
		RF_Flag.rf_IRQ = FALSE;
		RF_Flag.rf_Init_Complete = TRUE;
		pRF_Interface->RF_nIRQ_ON();
		return 0x01;
	}
	A7139_SetTxPowerSupply(6);		//设置发射功率
	RF_Flag.rf_IRQ = FALSE;
	RF_Flag.rf_Init_Complete = TRUE;
	pRF_Interface->RF_nIRQ_ON();
	return 0x00;
#else
  for(val = 0;val < 1;val++)
  {  
    /*Turn on the radio by pulling down the SDN pin
      Wait at least 15ms befory any initialization SPI commands are sent to the radio
      (wait for the power on reset sequence)*/
			pRF_Interface->RF_hSDN(1);
      RF_Delay_ms(1);
      pRF_Interface->RF_hSDN(0);

      RF_Delay_ms(30);
      RF_Drv_WriteReg(OperatingFunctionControl1,0x80);

      RF_Delay_ms(1);
      RF_Drv_GetItStatusAll();
  }
  /*disable all ITs, except 'ichiprdy'*/
  RF_Drv_WriteReg(InterruptEnable1, 0x00);
  RF_Drv_WriteReg(InterruptEnable2, 0x00);
  RF_Drv_GetItStatusAll();
  /*set the VCO and PLL*/
  RF_Drv_WriteReg(ChargepumpCurrentTrimming_Override, 0x80);
  RF_Drv_WriteReg(DividerCurrentTrimming, 0xC0);
  RF_Drv_WriteReg(VCOCurrentTrimming, 0x7F);
  /*select nothing to the Analog Testbush*/
  RF_Drv_WriteReg(AnalogTestBus, 0x0B);
  /*reset digital testbus, disable scan test*/
  RF_Drv_WriteReg(DigitalTestBus, 0x00);
  
  RF_Drv_SetCarrier(DEF_CARRIER_FRE);
  /*set Crystal Oscillator Load Capacitance register-max:12.5pf，step:0.097pf
    Cint = 1.8 pF + 0.085 pF x xlc[6:0] + 3.7 pF x xtalshift*/
  val  = 0xB5 + DEF_CINT;
  RF_Drv_WriteReg(CrystalOscillatorLoadCapacitance, val);
  
  RF_Drv_WriteReg(HeaderControl1, 0x00);
  RF_Drv_WriteReg(HeaderControl2, 0x0A);
  RF_Drv_WriteReg(SyncWord3, 0x81);
  RF_Drv_WriteReg(SyncWord2, 0x7E);
  
  /*set the GPIO's according to the RF switch */
  RF_Drv_WriteReg(RF_RX_GPIO, 0x15); 
  RF_Drv_WriteReg(RF_TX_GPIO, 0x12);
  RF_Drv_WriteReg(RF_NULL_GPIO, GPIO_TO_GND);
  
  /*by baud to set frequency*/
  RF_Drv_Hop_Set(RF_Config.nHop);
  
  /* TX output power */
  RF_Drv_SetTXPower(RF_Config.nPower);
  RF_Drv_DataRate_Set(RF_Config.nBaud);
  RF_Drv_WriteReg(DataAccessControl, 0x88);
  RF_Drv_WriteReg(ModulationModeControl2, 0x63);
  
  /*set preamble length & detection threshold*/
  RF_Drv_PreambleLength_Set(32);
  RF_Drv_PreamblsheThreold_Set(16);
  RF_Drv_WriteReg(AGCOverride1, 0x60);
  RF_Drv_WriteReg(AGCOverride2, 0x0B);
  RF_Drv_WriteReg(DeltasigmaADCTuning2, 0x04);
  RF_Drv_WriteReg(ClockRecoveryGearshiftOverride, 0x03);
  RF_Drv_WriteReg(TransmitPacketLength, RF_FIFO_LENGTH);
  RF_Flag.rf_Init_Complete = TRUE;
  return 0x00;
#endif /* A7139 */
}

/*************************************************************
函数名称:void RF_Drv_Idle(void)
功    能:配置SI4432进入IDLE空闲状态
参    数:无
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_Idle(void)
{
  /*enable Idle chain*/
  RF_Drv_WriteReg(OperatingFunctionControl1, 0x01);
  /*enable the wanted ITs*/
  RF_Drv_WriteReg(InterruptEnable1, 0x00); 
  RF_Drv_WriteReg(InterruptEnable2, 0x00);
  /*read interrupt status registers to release all pending interrupts*/
  RF_Drv_GetItStatusAll();/*读RF中断状态寄存器1、2*/
}
/*************************************************************
函数名称:void RF_Drv_Receive(void)
功    能:配置RF进入接收状态
参    数:无
返 回 值:无
说    明:无
*************************************************************/
void RF_Drv_Receive(void)
{
  RF_Flag.rf_Init_Complete = FALSE;
#if defined (A7139)
    RF_Drv_StrobeCMD(CMD_STBY); 
    RF_Drv_StrobeCMD(CMD_RX);
    RF_Drv_StrobeCMD(CMD_RFR);
#else  
    RF_Drv_Idle();
    RF_Drv_WriteReg(TransmitPacketLength, RF_FIFO_LENGTH);
    RF_Drv_WriteReg(OperatingFunctionControl2, 0x02);	
    RF_Drv_WriteReg(OperatingFunctionControl2, 0x00);	
    RF_Drv_WriteReg(OperatingFunctionControl1, 0x05);
    RF_Drv_WriteReg(InterruptEnable1, 0x02);
    RF_Drv_WriteReg(InterruptEnable2, 0x00);
    /*enable receiver chain*/
    RF_Drv_WriteReg(RF_RX_GPIO, 0x15);
    RF_Drv_GetItStatusAll();
#endif /* A7139 */
    RF_Flag.rf_Init_Complete = TRUE;
}

/*************************************************************
函数名称:void RF_Drv_Send(void)
功    能:配置RF进入发射状态
参    数:
返 回 值:
说    明:
*************************************************************/
void RF_Drv_Send(void)
{
  /* Clear the TX FIFO */
  RF_Drv_WriteReg(OperatingFunctionControl2, 0x01);		
  RF_Drv_WriteReg(OperatingFunctionControl2, 0x00);	
  
  /*Disable all other interrupts and enable the packet sent interrupt only.
  This will be used for indicating the successfull packet transmission for the MCU*/
  RF_Drv_WriteReg(InterruptEnable1, 0x04);
  RF_Drv_WriteReg(InterruptEnable2, 0x00);
  RF_Drv_WriteReg(RF_TX_GPIO, 0x12);
  
  /*Read interrupt status regsiters. It clear all pending interrupts 
  and the nIRQ pin goes back to high.*/
  RF_Drv_GetItStatusAll();
}
/*************************************************************
函数名称:
功    能:
参    数:
返 回 值:
说    明:
*************************************************************/
rf_uint8 RF_Drv_Listening(void)
{
  static rf__IO rf_uint8 ItStatus1,ItStatus2;
  ItStatus1 = RF_Drv_ReadReg(InterruptStatus1);
  ItStatus2 = RF_Drv_ReadReg(InterruptStatus2);
  /*有效数据、同步字监测、有效引导码监测*/
  if(((ItStatus1&BIT1)||(ItStatus2&BIT7))||(ItStatus2&BIT6))
  {
    return (1);
  }
  return (0);
}
/*************************************************************
函数名称:rf_int8 RF_Drv_Send_Data(MAC_t *pMac_Data)
功    能:RF发送数据包
参    数:发射次数回传
返 回 值:-1->硬件错误；-2->无效指针;-3->数据溢出;
        -4->信道使用中；>0 ->发送成功
说    明:无
*************************************************************/
rf_int8 RF_Drv_Send_Data(RF_Property_t *pMac_Data)
{
  rf_uint16 time_out = 0;
  rf_uint8 i = 0;
#if defined(SI4432)
  rf_uint16 cnt = 0;
#endif /*SI4432*/
  if(pMac_Data == NULL)
  {
      return -0x02;
  }
  if(pMac_Data->length > (RF_FIFO_LENGTH - 1))
  {
      return -0x03;
  }
#if defined(SI4432)
  if(RF_Drv_Listening())
  {
      return (-0x04);
  }
#endif /*SI4432*/
  pRF_Interface->RF_nIRQ_OFF();
#if defined (A7139)
  if(pMac_Data->length > 0)
  {
      /*write data to tx fifo*/  
      RF_Drv_StrobeCMD(CMD_STBY);
      /*TX FIFO address pointer reset*/
      RF_Drv_StrobeCMD(CMD_TFR);
      pRF_Interface->RF_nSCS_SET(0);
      /*TX FIFO write command*/
      ByteSend(CMD_FIFO_W);
      ByteSend(pMac_Data->length);
      for(i = 0;i< pMac_Data->length;i++)
      {
          ByteSend(*(pMac_Data->Buffer + i));
      }
      pRF_Interface->RF_nSCS_SET(1);
      /*开始发送*/
      RF_Drv_StrobeCMD(CMD_TX);
      time_out = 0x00;
      while(pRF_Interface->IS_RF_nIRQ_H())/*wait transmit completed  等IRQ*/
      {
          NCS_NOPS();
          time_out++;
          if(time_out > DEF_TIMEOUT) 
          {
              pRF_Interface->RF_nIRQ_ON();
						
              return -5;/*超时*/
          } 
      }
  }
#else
    RF_Drv_Send();/* 进入发射模式 */
    /*SET THE CONTENT OF THE PACKET*/
    RF_Drv_WriteReg(TransmitPacketLength, pMac_Data->length + 1);
    RF_Drv_WriteReg(FIFOAccess, pMac_Data->length);
    for(i=0;i<pMac_Data->length;i++)
    {
        RF_Drv_WriteReg(FIFOAccess, pMac_Data->Buffer[i]);
    }
    /* 按照次数进行发射 */
    while(cnt < RF_Config.tCount)
    {
      RF_Drv_WriteReg(OperatingFunctionControl1, 0x09);
      /*wait for the packet sent interrupt*/
      time_out = 0;
      do
      {
        RF_DELAY_100US(1);
        time_out++;
        /*64字节,2400波特率,300ms*/
        if(time_out > DEF_TIMEOUT)
        {
          pRF_Interface->RF_nIRQ_ON();
          return (-5);
        }
      }
      while(pRF_Interface->IS_RF_nIRQ_H());
      cnt++;
      /*read interrupt status registers to release the interrupt flags*/
      RF_Drv_GetItStatusAll();
    }
    RF_Delay_ms(10);/*不要立马进入空闲*/
    /*read interrupt status registers to release the interrupt flags*/
    RF_Drv_GetItStatusAll();
#endif /* A7139 */ 
    RF_Drv_Receive();
    pRF_Interface->RF_nIRQ_ON();
    return pMac_Data->length;
}
/*************************************************************
函数名称:rf_uint8 A7139_Drv_Read_FIFO(rf_uint8* pData)
功    能:
参    数:
返 回 值:
        
说    明:
*************************************************************/
rf_uint8 A7139_Drv_Read_FIFO(rf_uint8* pData)
{
    rf_uint8 rLen;
    rf_uint8 i;
    pRF_Interface->RF_nSCS_SET(0);
    ByteSend(CMD_DATAR);/*send read fifo command*/
    rLen = ByteRead();
    if(rLen > RF_FIFO_LENGTH)
      rLen = RF_FIFO_LENGTH;
    for(i=0;i<rLen;i++)
      pData[i] = ByteRead();
    pRF_Interface->RF_nSCS_SET(1);
    return rLen;
}
/*************************************************************
函数名称:static void Err_State(void)
功    能:
参    数:
返 回 值:
        
说    明:
*************************************************************/
void RF_Drv_WORexecute(rf_uint8 cLength)
{
    switch(cLength)
    {
        case 0:/*进入睡眠模式*/
                RF_Drv_StrobeCMD(CMD_STBY);
                RF_Drv_StrobeCMD(CMD_SLEEP);
                break;
        case 1:/*进入接收模式*/
                RF_Drv_StrobeCMD(CMD_STBY);
                RF_Drv_StrobeCMD(CMD_RX);
                RF_Drv_StrobeCMD(CMD_RFR);
                break;
        default:break;
    }
    RF_Flag.rf_IRQ = FALSE;
}


/**
  * @brief  This function 读RF——FIFO
  * @param  out-> -1：指针无效；-2:非有效数据中断；>0:收到有效数据
  * @retval : None
*/
rf_int8 RF_Receive_Data(RF_Property_t *pMac_Data)
{
#if defined (SI4432)
    static __IO rf_uint8 ItStatus1,ItStatus2;
    rf_uint8 val,i;
#endif /* SI4432 */ 
    rf_int16 status = -1;
    if(pMac_Data != NULL)
    {
#if defined (A7139)
        pMac_Data->length = A7139_Drv_Read_FIFO(pMac_Data->Buffer);
        status = pMac_Data->length;
    }
    return (status);
#else 
        ItStatus1 = RF_Drv_ReadReg(InterruptStatus1);
        if(ItStatus1&0x02)
        {
            val = RF_Drv_ReadReg(ReceivedSignalStrengthIndicator);/*rssi*/
            val = val/2;
						
            pMac_Data->Rssi = (rf_int16)(val - 132);
            pMac_Data->length = RF_Drv_ReadReg(FIFOAccess);
            
            if(pMac_Data->length > RF_FIFO_LENGTH )
            {
               pMac_Data->length = RF_FIFO_LENGTH;
            }
            for(i=0;i < pMac_Data->length;i++)
            {
               pMac_Data->Buffer[i] = RF_Drv_ReadReg(FIFOAccess);
            }
            status = pMac_Data->length;
        }
        else
        {
          status = -2;
        }
    }
    RF_Drv_Receive();
    return (status);
#endif /* A7139 */ 
}

/*RF 频点设置*/
void RF_433_470_FP_Set(rf_uint16 fp)
{
	if(fp > 31){
		fp =31;
	}
	RF_433_470.FP = fp;
	
	//A7139_SetChannel(RF_433_470.FP);//设置信道
}

/*RF初始化函数*/
void RF_433_470_Initializes(void)
{
	/*接口实例化*/
	pRF_Interface = &Implement_RF_Interface;
	/*硬件抽象层的初始化*/
	pRF_Interface->Hal_Init();	
	/*初始化*/
	RF_433_470.Rf_InitStatus =  RF_Drv_Init();
	/*接收模式*/
	RF_Drv_Receive();
}

/*中断函数执行*/
void RF_433_470_IrqHandler(void)
{
	if(RF_Flag.rf_Init_Complete == TRUE)
  {
		RF_Flag.rf_IRQ = TRUE;
  }
}

/*接收数据处理*/
void RF_433_470_ReceiveHandler(void)
{
	static RF_Property_t RF_MAC_Data;
	if(RF_Flag.rf_IRQ == TRUE){
		RF_Flag.rf_IRQ = FALSE;
		if(RF_Receive_Data(&RF_MAC_Data) > 0x00){
			/*开始回调*/
			rf_irq_callBack.IrqCall(&RF_MAC_Data);
		}
		RF_Drv_Receive();
	}
}

/*回调函数设置*/
void RF_433_470_DataCallbackSet(void (*ptr)(RF_Property_t*))
{
	if(ptr != NULL){
		/*函数指针赋值*/
		rf_irq_callBack.IrqCall = ptr;
	}
}

/*api 函数接口*/
RF_API_t RF_433_470 = 
{
	.Initializes = RF_433_470_Initializes,
	.DataCallbackSet = RF_433_470_DataCallbackSet,
	.IrqHandler = RF_433_470_IrqHandler,
	.FP_Set = RF_433_470_FP_Set,
	.Send_Data = RF_Drv_Send_Data,
	.ReceiveHandler = RF_433_470_ReceiveHandler,
};

