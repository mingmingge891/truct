#ifndef  RF_H
#define  RF_H

#include "rf_cpu.h"
#include "si4432_reg.h"
#include "a7139_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define A7139_BASE_FRE			  417.875f
	
/*选择芯片*/
#define A7139
//#define SI4432		

typedef struct{
  rf_int16 Rssi;                             /*功率*/
  rf_int16 Afc;                              /*频偏*/
  rf_uint8 length;                           /*数据包长度*/
  rf_uint8 Count;                            /*数据发射包数或者时长(以秒为单位)*/
  rf_uint8 Buffer[RF_FIFO_LENGTH];           /*数据缓冲区*/
}RF_Property_t;	
	
typedef struct 
{
		void (*IrqCall)(RF_Property_t *p);
}RfIrq_callBackCB_t;

typedef struct{
	bool rf_IRQ;
	bool rf_Init_Complete;
}RF_Flag_t;

typedef struct{
rf_uint8 Fre1,Fre0,FreBand;
}Car_t;


/*数据通信交互接口*/
typedef struct
{
	/*判断IRQ的电平*/
	rf_uint8 (*IS_RF_nIRQ_H)(void);
	/*SDIO*/
	rf_uint8 (*RF_SDIO_IN)(void);
	/*MISO*/
	rf_uint8 (*RF_MISO_IN)(void);
	/*MOSI SET*/
	void (*RF_MOSI_SET)(rf_uint8 H_L);
	/*SCK SET*/
	void (*RF_SCLK_SET)(rf_uint8 H_L);
	/*SCS SET*/
	void (*RF_nSCS_SET)(rf_uint8 H_L);
	/*SDN SET*/
	void (*RF_hSDN)(rf_uint8 H_L);
	/*A7139_PWR*/
	void (*A7139_PWR_ENABLE)(void);
	void (*A7139_PWR_DISABLE)(void);
	/*IO SET*/
	void (*RF_SDIO_MO)(void);
	void (*RF_SDIO_MI)(void);
	/*中断开关*/
	void (*RF_nIRQ_ON)(void);
	void (*RF_nIRQ_OFF)(void);
	
	/*延时函数*/
	void (*delay_ms)(rf_uint32);
	void (*delay_us)(rf_uint32);
	
	/*RF HAL initializes*/
	void (*Hal_Init)(void);
	
}RF_Interface_T;

/*实现接口*/
extern RF_Interface_T Implement_RF_Interface;

typedef enum  
{
	STATUS_SUCCESS = 0x00,
	STATUS_ERROR,
	STATUS_ERROR_TIMEOUT,
	STATUS_ERROR_IF_CAL,
	STATUS_ERROR_VCO_CAL,
	STATUS_ERROR_ID_RW,
	STATUS_ERROR_SPI_LOGIC,
	STATUS_ERROR_INVALID_LENGTH,
	STATUS_SUCCESS_SPI_LOGIC,
	STATUS_SUCCESS_IF_VCO_CAL,
	STATUS_SUCCESS_TX,
}PHY_STATUS;

/*API操作函数*/
typedef struct
{
	/*频点设置 0-20*/
	rf_uint16 FP;
	float RF_Base;//基础频率
	rf_uint8 Rf_InitStatus;
	void (*FP_Set)(rf_uint16 FP);
	/*初始化*/
	void (*Initializes)(void);
	void (*DataCallbackSet)(void (*ptr)(RF_Property_t*));
	void (*IrqHandler)(void);
	void (*ReceiveHandler)(void);
	rf_int8 (*Send_Data)(RF_Property_t *pMac_Data);
}RF_API_t;
RF_CPU_EXT RF_API_t RF_433_470;

/*API 向外*/
RF_CPU_EXT void RF_433_470_Initializes(void);
RF_CPU_EXT void RF_433_470_IrqHandler(void);
RF_CPU_EXT void RF_433_470_ReceiveHandler(void);
RF_CPU_EXT void RF_433_470_DataCallbackSet(void (*ptr)(RF_Property_t*));
RF_CPU_EXT rf_int8 RF_Drv_Send_Data(RF_Property_t *pMac_Data);

#ifdef __cplusplus

}
#endif

#endif
