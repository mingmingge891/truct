/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#ifndef __DL_NRF24l01_H_
#define __DL_NRF24l01_H_



/***********NRF24L01P指令*****************************/
#define RF_R_REGISTER          0x00  //读命令/状态寄存器    
#define RF_W_REGISTER          0x20  //写命令/状态寄存器    
#define RF_R_RX_PAYLOAD        0x61  //读RX载荷             
#define RF_W_TX_PAYLOAD        0xA0  //写TX载荷             
#define RF_FLUSH_TX            0xE1  //清除TX FIFO         
#define RF_FLUSH_RX            0xE2  //清除RX FIFO          
#define RF_REUSE_TX_PL         0xE3  //重新使用上一个TX载荷 
#define RF_R_RX_PL_WID         0x60  //读RX载荷宽度

#define RF_W_ACK_PAYLOAD_P0    0xA8  //数据通道0 ACK
#define RF_W_ACK_PAYLOAD_P1    0xA9  //数据通道1 ACK
#define RF_W_ACK_PAYLOAD_P2    0xAA  //数据通道2 ACK
#define RF_W_ACK_PAYLOAD_P3    0xAB  //数据通道3 ACK
#define RF_W_ACK_PAYLOAD_P4    0xAC  //数据通道4 ACK
#define RF_W_ACK_PAYLOAD_P5    0xAD  //数据通道5 ACK

#define RF_W_TX_PAYLOAD_NOACK  0xB0  //禁用ACK
#define RF_NOP                 0xFF  //空操作               

/**********NRF24L01P寄存器**********/
#define RF_CONFIG       0x00  //配置寄存器                
#define RF_EN_AA        0x01  //使能自动应答
#define RF_EN_RXADDR    0x02  //使能数据通道         
#define RF_SETUP_AW     0x03  //设置地址宽度
#define RF_SETUP_RETR   0x04  //配置自动重发             
#define RF_RF_CH        0x05  //RF通道                        
#define RF_RF_SETUP     0x06  //RF配置寄存器                    
#define RF_STATUS       0x07  //状态寄存器                   

#define RF_RX_OK        0x40  //接收数据中断
#define RF_TX_OK        0x20  //数据发送完成中断
#define RF_MAX_TX       0x10  //达到最多次重发中断

#define RF_OBSERVE_TX   0x08  //发送检测寄存器       
#define RF_RPD          0x09  //载波检测寄存器         
#define RF_RX_ADDR_P0   0x0A  //数据通道0接收地址 
#define RF_RX_ADDR_P1   0x0B  //数据通道1接收地址 
#define RF_RX_ADDR_P2   0x0C  //数据通道2接收地址 
#define RF_RX_ADDR_P3   0x0D  //数据通道3接收地址 
#define RF_RX_ADDR_P4   0x0E  //数据通道4接收地址  
#define RF_RX_ADDR_P5   0x0F  //数据通道5接收地址 
#define RF_TX_ADDR      0x10  //发送地址           
#define RF_RX_PW_P0     0x11  //接收数据通道0有效数据宽度
#define RF_RX_PW_P1     0x12  //接收数据通道1有效数据宽度
#define RF_RX_PW_P2     0x13  //接收数据通道2有效数据宽度
#define RF_RX_PW_P3     0x14  //接收数据通道3有效数据宽度
#define RF_RX_PW_P4     0x15  //接收数据通道4有效数据宽度
#define RF_RX_PW_P5     0x16  //接收数据通道5有效数据宽度
#define RF_FIFO_STATUS  0x17  //FIFO状态寄存器              
#define RF_DYNPD        0x1C  //使能/禁止动态载荷                       
#define RF_FEATURE      0x1D  //功能寄存器


typedef struct
{
	u8  *FIFO;
	const u16  FIFO_length;
	const u8 TX_ADR_WIDTH;
	const u8 RX_ADR_WIDTH;
	u8*  RX_ADDRESS0;
	u8*  rfRxAddrCh0;
	u8*  rfTxAddrCh0;
	void (*Init)(void);       							          /*! Initialize the LED */
	void (*RF_csnEn)(void);
	void (*RF_csnDis)(void);
	void (*RF_ceEn)(void);
	void (*RF_ceDis)(void);
	uint8_t (*readWriteByte)(uint8_t d);
	void (*irq_register)(void (*ptr)(void));

}NRF24l01_Interface_t;

/*NRF24l01操作API
*/
typedef struct
{
	/*发送一包数据*/
	void (*SendData)(void*,uint8_t);
	void (*irqTxF)(u8* TxRxaddr,u8 *buf, u8 len);
	/*设置RF无线电信道*/
	void (*setChannel)(u8 ch);
	/*RF无线电配对通道设置*/
	void (*setPairChannel)(u8 ch);
	u8   (*Get_FIFO_Length)(void);
	bool (*Get_CompleteState)(void);
	void (*ToSendDataReady)(void);
	u8*  (*Get_FIFO_BUF)(void);
	u8   (*Checkchip)(void);
}nrf24l01_t;
extern nrf24l01_t nrf24l01;
extern NRF24l01_Interface_t NRF24l01_Interface;

void init_nrf24l01(void);

#endif
