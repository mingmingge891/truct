#ifndef  _DL_IRFRARED_H
#define  _DL_IRFRARED_H

#define  hstp_CPU_EXT  extern
/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/
typedef unsigned char  hstp_uint8;                /* Unsigned  8 bit quantity                           */
typedef signed	 char  hstp_int8;                 /* Signed    8 bit quantity                          */
typedef unsigned short hstp_uint16;               /* Unsigned 16 bit quantity                           */
typedef          short hstp_int16;                /* Signed   16 bit quantity                           */
typedef unsigned int   hstp_uint32;               /* Unsigned 32 bit quantity                           */
typedef 	       int   hstp_int32;          /* Signed   32 bit quantity                           */
typedef float          hstp_fp32;                 /* Single precision floating point                    */
typedef double         hstp_fp64;                 /* Double precision floating point                    */
#define hstp__IO  volatile 
	
/* Bits */
#define BIT0            0x01
#define BIT1            0x02
#define BIT2            0x04
#define BIT3            0x08
#define BIT4            0x10
#define BIT5            0x20
#define BIT6            0x40
#define BIT7            0x80

#include 	"stm32f10x.h"
#include  "dl_irfrared_llc.h"
#include  "dl_irfrared_mac.h"

#ifdef __cplusplus
extern "C" {
#endif
	
typedef void (*callBackReceiveData)
(
		__User_Data_t*
);

typedef struct 
{
		callBackReceiveData  User_ReceiveData;
}User_callBackCB_t;

/*定义抽象类函数需要具体实现*/

typedef struct
{
	void (*Init)(void);       													 			/*! Initialize the HAL */
	hstp_uint8* (*Get_UID)(void);															/*得到ID号*/
	void (*delay_ms)(u16 t);
	void (*delay_us)(u16 t);
	u8 (*ReadPin)(u8 PinIndex);
	void (*irq_rx_register)(void (*ptr)(u8));
	void (*irq38khz_tx_register)(void (*ptr)(void));
	void (*irq_off)(u8 index);
	void (*irq_on)(u8 index);
	void (*sendPin)(u8 h_l);
	//打开脉冲定时器
	void (*pulse_open)(void);
	void (*pulse_close)(void);
	//关闭脉冲定时器
}Dl_Irfrared_Interface_t;


/*用户层需要实现的接口*/
extern  Dl_Irfrared_Interface_t Implement_IR_Interface;

/*初始化 HSTP*/
void IrRemote_Init(void);


/*IR_REFRARED操作API
*/
typedef struct
{
	//是否是轮询模式处理数据
	bool IsPollingHandler;
	//接收中断的id
	u8 rev_irq_id;
	//发送数据
	void (*SendCustom_Data)(u8*,u8 len);
	//O;SUC -1;SEND_ERR 
	hstp_int16 (*SendLLC_Data)(__Data_LLC_t *user_SDH);
	void (*setRxCallback)(void (*ptr)(__User_Data_t*));
	void (*RemotePollingHandler)(u8);
}ir_remote_t;
extern ir_remote_t ir_remote;

#ifdef __cplusplus

}
#endif

#endif
