/**
  ******************************************************************************
  * @file    dl_irfrared.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V0.1.0
  * @date    22-3-2020
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.wisdudu.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghui technology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2016 WIS-DUDU</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dl_irfrared.h"
#include "string.h"

#define IR_DATA_MAX_NUM    4
#define IR_FIFO_LENGTH     64
#define LOOP_TIME_OUT(x) 	 \
{pHSTP_Property->delay_us(100);if(x > 1000)\
{pHSTP_Property->irq_on(index);return;}}


typedef struct{
    bool irqId[5];
}ir_Flag_t;

User_callBackCB_t CallBack;
hstp_MAC_T IR_MAC_Data;
__User_Data_t *rev_DH = NULL;
/*抽象接口函数定义*/
Dl_Irfrared_Interface_t* pHSTP_Property;

static hstp_uint16 count = 0,endcount = 0;
hstp_uint16 ir_time_count = 0;
hstp_uint8 IR_flag;
hstp_uint8 IRCOM[IR_DATA_MAX_NUM];
ir_Flag_t ir_Flag;
hstp_uint16 ir_time_count2 = 0;
static void Event_Handler(uint8_t event);

/**
  * @brief 发送中断38KHZ振荡器 13us定时器中断
  * @param
  * @retval: None
*/
void IR_38KHZ_oscillators(void)
{
	count++;
	static u8 OP;
	if(IR_flag == 1){
		OP = ~OP;
		pHSTP_Property->sendPin(OP);
	}
	else{
		pHSTP_Property->sendPin(1);
	}
}
/**
  * @brief 接收中断
  * @param
  * @retval: None
*/
void ir_nec_decoding_irq(u8 index)
{
	hstp_uint8 j,k;
	
	/*首先关闭中断*/
	pHSTP_Property->irq_off(index);
	
	/*9ms红外前导码的启始码，先延时7ms判断是否是高电平
	如果是说明不是红外线进的中断是干扰进的*/
	pHSTP_Property->delay_ms(7);
	if(pHSTP_Property->ReadPin(index))
	{
		pHSTP_Property->irq_on(index);
		return;
	}
	//等9ms前导码结束，等待结果码到来，结果码一出现就退出循环。
	ir_time_count = 0;
	while(!pHSTP_Property->ReadPin(index))
	{
		LOOP_TIME_OUT(ir_time_count++);
	}
	/*计算一下高电平的时间 时间必须是4.5ms*/
	ir_time_count = 0;
	while(pHSTP_Property->ReadPin(index))
	{
		pHSTP_Property->delay_us(10);
		ir_time_count++;
		if(ir_time_count > 1000)
		{
			pHSTP_Property->irq_on(index);
			return;
		}
	}
	if(ir_time_count < 350)
	{
		pHSTP_Property->irq_on(index);
		return;
	}
	//这里开始读数据
	for(j=0;j<IR_DATA_MAX_NUM;j++)
	{
		for(k=1;k<=8;k++)
		{ 
				/*等待0.565ms到来，一到来就退出循环*/
				ir_time_count = 0;
				while(pHSTP_Property->ReadPin(index))
				{
					LOOP_TIME_OUT(ir_time_count++);
				}
				
				/*等待变为高电平，一变为高就退出循环*/
				ir_time_count = 0;
				while(!pHSTP_Property->ReadPin(index))
				{
					LOOP_TIME_OUT(ir_time_count++);
				}
				ir_time_count = 0;
				/*现在开始计算高电平的时长，1.685ms是逻辑1，0.565是逻辑0*/
				while(pHSTP_Property->ReadPin(index))
				{
					pHSTP_Property->delay_us(10);
					ir_time_count++;
					if(ir_time_count > 5000)
					{
	//					j = IR_DATA_MAX_NUM+10;
	//					k = 8+10;
						break;
					}
				}
				/*LSB，所以要右移动*/
				IRCOM[j]=IRCOM[j] >> 1;
				
				/*超过1ms说明是逻辑1*/
				if(ir_time_count > 100)
				{
					IRCOM[j] = IRCOM[j] | 0x80;
				}
				ir_time_count = 0x00;
		 }
	}
	//回调事件 index+1 目的是最小为一个事件
	Event_Handler(index+1);
	pHSTP_Property->irq_on(index);
}
/**
  * @brief 
  * @param
  * @retval: None
*/
hstp_uint8 IR_Drv_Read_FIFO(hstp_uint8* pData)
{
	hstp_uint8 rLen;
  hstp_uint8 i;
	rLen = IR_DATA_MAX_NUM;
	if(rLen > IR_FIFO_LENGTH){
      rLen = IR_FIFO_LENGTH;
	}
  for(i=0;i<rLen;i++){
		pData[i] = IRCOM[i];
	}
	return rLen;
}
/**
  * @brief 
  * @param
  * @retval: None
*/
void Send_IRdata(hstp_uint8 x_irdata)
{
	unsigned char i; 
	unsigned char irdata;
	
	irdata = x_irdata;
	for(i=0;i<8;i++)
	{
		count = 0;
		endcount = 42;
		IR_flag = 1;
		while(count < endcount);

		if(irdata&0x01)
		{
			endcount = 128;	  //1.685ms / 13.15us = 128
		}
		else endcount = 43;	  //0.565ms / 13.15us = 43

		count = 0;
		IR_flag = 0;
		while(count < endcount);

		irdata = irdata >> 1;
	}
}

/**
  * @brief 
  * @param
  * @retval: None
*/
hstp_int8  hstp_Drv_Send_MacData(hstp_MAC_T *pMac_Data)
{
	unsigned char i = 0;
	pHSTP_Property->pulse_open();
	//发送9ms的引导码
	endcount = 684;	 //9ms / 13.15us = 684
	count = 0;
	IR_flag = 1; //发送标志(发送38K载波)
	while(count < endcount);

	//发送4.5ms的引导码的结束码
	count = 0;
	endcount = 342;	 //4.5ms / 13.15us = 342 
	IR_flag = 0; //发送标志（发送0）
	while(count < endcount);
	
	for(i = 0;i < pMac_Data->length;i++)
	{
		Send_IRdata(pMac_Data->Buffer[i]);
	}	
	
	//最后发送0.56ms的38KHz红外载波（即编码中0.56ms的低电平）
	count = 0;
	endcount = 42;	//0.56ms / 13.15us = 42 (42 / 2 = 21，即发送0.56ms的载波中包括21个载波周期)
	IR_flag = 1; //发送标志
	while(count < endcount);
	IR_flag = 0;
	pHSTP_Property->pulse_close();
	return 1;
}
/**
  * @brief 
  * @param
  * @retval: None
*/
void dl_SendCustom_Data(hstp_uint8 *Data,u8 len)
{
	unsigned char i = 0;
	pHSTP_Property->pulse_open();
	//发送9ms的引导码
	endcount = 684;	 //9ms / 13.15us = 684
	count = 0;
	IR_flag = 1; //发送标志(发送38K载波)
	while(count < endcount);

	//发送4.5ms的引导码的结束码
	count = 0;
	endcount = 342;	 //4.5ms / 13.15us = 342 
	IR_flag = 0; //发送标志（发送0）
	while(count < endcount);
	
	for(i = 0;i < len;i++)
	{
		Send_IRdata(Data[i]);
	}	
	
	//最后发送0.56ms的38KHz红外载波（即编码中0.56ms的低电平）
	count = 0;
	endcount = 42;	//0.56ms / 13.15us = 42 (42 / 2 = 21，即发送0.56ms的载波中包括21个载波周期)
	IR_flag = 1; //发送标志
	while(count < endcount);
	IR_flag = 0;
	pHSTP_Property->pulse_close();
}
/**
  * @brief  This function 数据处理流程
  * @param  None
  * @retval : None
  */
static void Event_Handler(uint8_t event)
{	
	if((hstp_Receive_Data(&IR_MAC_Data) > 0x00)&& (event > 0))
	{
		/*协议分析解析*/
		rev_DH = Data_Analyse(IR_MAC_Data.Buffer,IR_MAC_Data.length);
		
		/*成功收到用户数据*/
		if(rev_DH->P_Result == REV_USER_DATA)
		{
			/*回调函数处理*/
			if(CallBack.User_ReceiveData != NULL)
			{
				 rev_DH->event = event;
				 CallBack.User_ReceiveData(rev_DH);
			}
		}
	}
}
static void ir_setCallback(void (*ptr)(__User_Data_t*))
{
	/*函数指针赋值*/
	CallBack.User_ReceiveData = ptr;
}
static void irRemotePollingHandler(u8 irq_id)
{
	if(ir_Flag.irqId[irq_id] == TRUE){
		ir_Flag.irqId[irq_id] = FALSE;
		ir_nec_decoding_irq(irq_id);
	}
}
static void irfrared_irq_handeler(u8 irq_id)
{
	ir_remote.rev_irq_id = irq_id;
	if(ir_remote.IsPollingHandler == TRUE){
		ir_Flag.irqId[irq_id] = TRUE;
	}else{
		ir_nec_decoding_irq(irq_id);
	}
}
//API
ir_remote_t ir_remote = 
{
	.SendCustom_Data        = dl_SendCustom_Data,
	.SendLLC_Data           = hstp_Send_Data,
	.setRxCallback          = ir_setCallback,
	.RemotePollingHandler   = irRemotePollingHandler,
	.IsPollingHandler       = FALSE,
};
/**
  * @brief  This function HSTP初始化
  * @param  None
  * @retval : None
  */
void IrRemote_Init(void)
{	
	/*获取实际接口地址*/
	pHSTP_Property = &Implement_IR_Interface;
	//注册中断
	if(pHSTP_Property->irq_rx_register != NULL){
		pHSTP_Property->irq_rx_register(irfrared_irq_handeler);
	}
	if(pHSTP_Property->irq38khz_tx_register != NULL){
		pHSTP_Property->irq38khz_tx_register(IR_38KHZ_oscillators);
	}
	/*初始化硬件抽象层*/
	pHSTP_Property->Init();
	/*得到MACID*/
	GET_D_MAC();
}
