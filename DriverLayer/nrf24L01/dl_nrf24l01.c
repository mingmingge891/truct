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
#include "stm32f10x.h"
#include "dl_nrf24l01.h"
#include "apl_bll.h"
#include "string.h"
#include "stdio.h"

/*value-------------------------------------------------------------*/
/*LED抽象函数结构体变量*/
NRF24l01_Interface_t* pNRF24L01_Implements;

u8 pairing_flag =0;
bool receiveCompltetState = FALSE;

/*****************************************************************************
 函 数 名  : u8 RF_readRegF(u8 readAddr)  
 功能描述  : 读芯片寄存器
 输入参数  : u8 readAddr  
 输出参数  : 
 返 回 值  : 
 日    期  : 2017年3月2日
 作    者  : ljk
*****************************************************************************/
u8 RF_readRegF(u8 readAddr)   
{
	u8 readData;    
	pNRF24L01_Implements->RF_csnEn();
	pNRF24L01_Implements->readWriteByte(readAddr);
	readData = pNRF24L01_Implements->readWriteByte(0xFF);
	pNRF24L01_Implements->RF_csnDis();
	return readData;
}   
/*****************************************************************************
 函 数 名  : void RF_writeRegF(u8 writeAddr, u8 writeData) 
 功能描述  : 写芯片寄存器
 输入参数  : u8 writeAddr  
             u8 writeData  
 输出参数  : 
 返 回 值  : 
 日    期  : 2017年3月2日
 作    者  : ljk
*****************************************************************************/
void RF_writeRegF(u8 writeAddr, u8 writeData)  
{
	pNRF24L01_Implements->RF_csnEn();
	pNRF24L01_Implements->readWriteByte(writeAddr);
	pNRF24L01_Implements->readWriteByte(writeData);
	pNRF24L01_Implements->RF_csnDis();                
}
/*****************************************************************************
 函 数 名  : void RF_readRegsF(u8 readAddr, u8 *readData, u8 len)  
 功能描述  : 读芯片寄存器
 输入参数  : u8 readAddr   
             u8 *readData  
             u8 len        
 输出参数  : 
 返 回 值  : 
 日    期  : 2017年3月2日
 作    者  : ljk
*****************************************************************************/
void RF_readRegsF(u8 readAddr, u8 *readData, u8 len)  
{
	u8 byte_ctr;    
	pNRF24L01_Implements->RF_csnEn();
	pNRF24L01_Implements->readWriteByte(readAddr);
	for(byte_ctr = 0; byte_ctr < len; byte_ctr++)
			readData[byte_ctr] = pNRF24L01_Implements->readWriteByte(0xFF);
	pNRF24L01_Implements->RF_csnDis();  
}
/*****************************************************************************
 函 数 名  : void RF_writeRegsF(u8 writeAddr, u8 *writeData, u8 len) 
 功能描述  : 写芯片寄存器
 输入参数  : u8 writeAddr   
             u8 *writeData  
             u8 len         
 输出参数  : 
 返 回 值  : 
 日    期  : 2017年3月2日
 作    者  : ljk
*****************************************************************************/
void RF_writeRegsF(u8 writeAddr, u8 *writeData, u8 len) 
{
	u8 byte_ctr;    
	pNRF24L01_Implements->RF_csnEn();
	pNRF24L01_Implements->readWriteByte(writeAddr);
	for(byte_ctr = 0; byte_ctr < len; byte_ctr++)
			pNRF24L01_Implements->readWriteByte(writeData[byte_ctr]); 
	pNRF24L01_Implements->RF_csnDis(); 
}

/*****************************************************************************
 函 数 名  : void RF_irqTxF(u8 ch, u8 *buf, u8 len) 
 功能描述  : 中断填充发送缓冲区
 输入参数  : u8 ch  
 输出参数  : 
 返 回 值  : 
 日    期  : 2017年3月2日
 作    者  : ljk
*****************************************************************************/
void RF_irqTxF(u8* TxRxaddr,u8 *buf, u8 len) 
{
	u8 *ptr;
	u8 length;
	ptr = buf;
	length = len;
	
	pNRF24L01_Implements->RF_ceEn();
	RF_writeRegF(RF_W_REGISTER + RF_STATUS, 0xFF);  
	RF_writeRegF(RF_FLUSH_TX, 0xFF);
	RF_writeRegF(RF_FLUSH_RX, 0xFF);
	
	RF_writeRegsF(RF_W_REGISTER + RF_TX_ADDR, TxRxaddr, 5);
	RF_writeRegsF(RF_W_REGISTER + RF_RX_ADDR_P0, TxRxaddr, 5);
	
	RF_writeRegsF(RF_W_TX_PAYLOAD, ptr, length);//fill send buffer
	pNRF24L01_Implements->RF_ceDis();
}
//配对通道初始化
void NRF24L01_Init_PAIR(u8 ch)
{
	u8 sta;                                     
	sta = RF_readRegF(RF_STATUS);     
	RF_writeRegF(RF_W_REGISTER + RF_STATUS, sta);  
	RF_writeRegF(RF_FLUSH_TX, 0xFF); 
	RF_writeRegF(RF_FLUSH_RX, 0xFF); 
	
	pNRF24L01_Implements->RF_ceEn();
	
	RF_writeRegsF(RF_W_REGISTER + RF_TX_ADDR,    pNRF24L01_Implements->RX_ADDRESS0, pNRF24L01_Implements->TX_ADR_WIDTH);	
	RF_writeRegsF(RF_W_REGISTER + RF_RX_ADDR_P0, pNRF24L01_Implements->RX_ADDRESS0, pNRF24L01_Implements->RX_ADR_WIDTH);
	
	RF_writeRegF(RF_W_REGISTER + RF_EN_AA, 0x01); //使能通道的自动应答 
	RF_writeRegF(RF_W_REGISTER + RF_EN_RXADDR, 0x01);//使能通道的接收地址
	RF_writeRegF(RF_W_REGISTER + RF_SETUP_RETR, 0x2a); //old:0x10 //等待750us 0x2a重新传10次
	RF_writeRegF(RF_W_REGISTER + RF_RF_CH, ch);//2460Mhz
	RF_writeRegF(RF_W_REGISTER + RF_RF_SETUP, 0x0F); //2Mbps 0dBm 低噪声增益开启
	RF_writeRegF(RF_W_REGISTER + RF_DYNPD, 0x01);//使能动态载荷 
	RF_writeRegF(RF_W_REGISTER + RF_FEATURE, 0x06);//功能寄存器
	//RF_writeRegF(RF_W_REGISTER + RF_CONFIG, 0x5A);//old:0x5a//CRC一个字节
	RF_writeRegF(RF_W_REGISTER + RF_CONFIG, 0x1A);//old:0x5a//CRC一个字节
	pNRF24L01_Implements->RF_ceDis();
	printf("PAIRch:%d\r\n",ch);
	pairing_flag = 1;
}

/***************************************************************************
函数名称：void NRF24L01_Init_TX(uchar pipe)
函数功能：初始化NRF24L01
函数备注：在调用NRF24L01作为发射时，都要先调用该函数对NRF24L01进行初始化
***************************************************************************/
void NRF24L01_Init_TX(u8 ch)//输入通道值 0-125
{    
	//宋明坤的设置目地兼容以前的
	u8 sta;                                     
	sta = RF_readRegF(RF_STATUS);     
	RF_writeRegF(RF_W_REGISTER + RF_STATUS, sta);  
	RF_writeRegF(RF_FLUSH_TX, 0xFF); 
	RF_writeRegF(RF_FLUSH_RX, 0xFF); 
	
	//RF_writeRegsF(RF_W_REGISTER + RF_TX_ADDR,  rfTxAddrCh0, TX_ADR_WIDTH);	
	//RF_writeRegsF(RF_W_REGISTER + RF_RX_ADDR_P0, rfRxAddrCh0, RX_ADR_WIDTH);

	pNRF24L01_Implements->RF_ceEn();
	RF_writeRegF(RF_W_REGISTER + RF_EN_AA, 0x3F);     
	RF_writeRegF(RF_W_REGISTER + RF_EN_RXADDR, 0x3F);  
	RF_writeRegF(RF_W_REGISTER + RF_SETUP_RETR, 0x15); //old:0x1
	RF_writeRegF(RF_W_REGISTER + RF_RF_CH, ch); 
	RF_writeRegF(RF_W_REGISTER + RF_RF_SETUP, 0x06);//1mbps
	RF_writeRegF(RF_W_REGISTER + RF_DYNPD, 0x3F);
	RF_writeRegF(RF_W_REGISTER + RF_FEATURE, 0x06);
	RF_writeRegF(RF_W_REGISTER + RF_CONFIG, 0x5A);  //old:0x5a
	pNRF24L01_Implements->RF_ceDis();
	pairing_flag = 0;
}

/***************************************************************************
函数名称：void nRF24L01_TxPacket(unsigned char * tx_buf)
函数功能：发送 tx_buf中数据
函数备注：
***************************************************************************/
static u8 nRF24L01_TxPacket(u8 pipe,u8 * tx_buf,u8 len)
{
	pNRF24L01_Implements->RF_ceEn();
	RF_writeRegF(RF_W_REGISTER + RF_STATUS, 0xFF);  
	RF_writeRegF(RF_FLUSH_TX, 0xFF);	
	RF_writeRegF(RF_FLUSH_RX, 0xFF);	
	RF_writeRegsF(RF_W_TX_PAYLOAD, tx_buf, len);
	pNRF24L01_Implements->RF_ceDis();//进入接收模式
	return 1;
}

static void nRF24L01_sendData(void* tx_buf,u8 len)
{
	nRF24L01_TxPacket(0,(u8*)tx_buf,len);
}

//中断数据回调
void rf_irq_call(void)
{
	u8 sta = RF_readRegF(RF_STATUS);
	RF_writeRegF(RF_W_REGISTER + RF_STATUS, sta);//清除RF中断标志位
	if(sta & RF_RX_OK){
		RF_readRegsF(RF_R_RX_PAYLOAD, pNRF24L01_Implements->FIFO, RF_readRegF(RF_R_RX_PL_WID));
		RF_writeRegF(RF_FLUSH_RX, 0xFF);
		receiveCompltetState = TRUE;
	}
	if(sta & RF_MAX_TX)//重新发送次数达到最大值中断 
	{
		RF_writeRegF(RF_FLUSH_TX, 0xFF);
	}
}

static u8 NRF24L01_GetFIFO_Length(void)
{
	return pNRF24L01_Implements->FIFO_length;
}
static u8* RF24L01_GetFIFO_BUF(void)
{
	return pNRF24L01_Implements->FIFO;
}
static bool NRF24L01_Get_CompleteState(void)
{
	return receiveCompltetState;
}
static void NRF24L01_ToSendDataReady(void)
{
	receiveCompltetState = FALSE;
}

static u8 NRF24L01_Checkchip(void)
{
	u8 i;
	u8 buf[5] = {0xA5, 0xA5, 0xA5, 0xA5, 0xA5};
	RF_writeRegsF(RF_W_REGISTER+RF_TX_ADDR, buf, 5);
	for(i = 0; i < 5; i++)
			buf[i] = 0;
	RF_readRegsF(RF_TX_ADDR, buf, 5); 
	for(i = 0; i < 5; i++)
			if(buf[i] != 0xA5)
					break;                                 
	if(i != 5)
			return 0;
	return 1;
}

//API
nrf24l01_t nrf24l01 = 
{
	.Get_CompleteState = NRF24L01_Get_CompleteState,
	.ToSendDataReady   = NRF24L01_ToSendDataReady,
	.irqTxF            = RF_irqTxF,
	.setChannel 		   = NRF24L01_Init_TX,
	.setPairChannel    = NRF24L01_Init_PAIR,
	.SendData          = nRF24L01_sendData,
	.Get_FIFO_Length   = NRF24L01_GetFIFO_Length,
	.Get_FIFO_BUF      = RF24L01_GetFIFO_BUF,
	.Checkchip         = NRF24L01_Checkchip,
};

/**
  * @brief init_nrf24l01
  * @param  None
  * @retval : None
**/
void init_nrf24l01(void)
{
	pNRF24L01_Implements = &NRF24l01_Interface;
	pNRF24L01_Implements->Init();
	pNRF24L01_Implements->irq_register(rf_irq_call);
}
