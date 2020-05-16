/**
  ******************************************************************************
  * @file    sm_iic.c
  * @author  hansen.jiang(minghong.jiang) 951868692@qq.com
  * @version Version 1.0.0
  * @date    2018/12/22
  * @brief   NULL
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "sm_iic.h"

/*抽象的指针*/
SM_IIC_Abstract_t *pSMIIC_Abstract;

/**
  * @brief   IIC 初始化
  * @param    None
  * @retval : None
**/
void SM_IIC_Initialize(void)
{					     
	/*实例化抽象化结构体*/
	pSMIIC_Abstract = &Implement_SMIIC_Abstract;

	/*相关的初始化*/
	pSMIIC_Abstract->Init();
	
}
/**
  * @brief    产生IIC起始信号
  * @param    None
  * @retval : None
**/
void IIC_Start(void)
{
	pSMIIC_Abstract->SDA_OUT();//sda线输出
	pSMIIC_Abstract->IIC_SDA_SET(1);
	pSMIIC_Abstract->IIC_SCL_SET(1);
	pSMIIC_Abstract->delay_us(D_START_STOP_1);
	pSMIIC_Abstract->IIC_SDA_SET(0);//START:when CLK is high,DATA change form high to low 
	pSMIIC_Abstract->delay_us(D_START_STOP_1);
	pSMIIC_Abstract->IIC_SCL_SET(0);//钳住I2C总线，准备发送或接收数据 
}	  
/**
  * @brief    产生IIC停止信号
  * @param    None
  * @retval : None
**/
void IIC_Stop(void)
{
	pSMIIC_Abstract->SDA_OUT();//sda线输出
	pSMIIC_Abstract->IIC_SCL_SET(0);
	pSMIIC_Abstract->IIC_SDA_SET(0);//STOP:when CLK is high DATA change form low to high
	pSMIIC_Abstract->delay_us(D_START_STOP_1);
	pSMIIC_Abstract->IIC_SCL_SET(1); 
	pSMIIC_Abstract->IIC_SDA_SET(1);//发送I2C总线结束信号
	pSMIIC_Abstract->delay_us(D_START_STOP_1);	
}
/**
  * @brief    
		等待应答信号到来
		返回值：1，接收应答失败
						0，接收应答成功
  * @param    None
  * @retval : None
**/
smiic_uint8 IIC_Wait_Ack(void)
{
	smiic_uint16 ucErrTime=0;
	pSMIIC_Abstract->SDA_IN();//SDA设置为输入
	pSMIIC_Abstract->IIC_SDA_SET(1);
	pSMIIC_Abstract->delay_us(D_WAIT_ACK_2);
	pSMIIC_Abstract->IIC_SCL_SET(1);
	pSMIIC_Abstract->delay_us(D_WAIT_ACK_2);
	while(pSMIIC_Abstract->IIC_SDA_READ())
	{
		ucErrTime++;
		if(ucErrTime>300)
		{
			IIC_Stop();
			return 1;
		}
	}
	pSMIIC_Abstract->IIC_SCL_SET(0);//时钟输出0 	   
	return 0;  
} 
/**
  * @brief    产生ACK应答
  * @param    None
  * @retval : None
**/
void IIC_Ack(void)
{
	pSMIIC_Abstract->IIC_SCL_SET(0);
	pSMIIC_Abstract->SDA_OUT();
	pSMIIC_Abstract->IIC_SDA_SET(0);
	pSMIIC_Abstract->delay_us(D_SET_ACK);
	pSMIIC_Abstract->IIC_SCL_SET(1);
	pSMIIC_Abstract->delay_us(D_SET_ACK);
	pSMIIC_Abstract->IIC_SCL_SET(0);
}
/**
  * @brief    不产生ACK应答		
  * @param    None
  * @retval : None
**/ 
void IIC_NAck(void)
{
	pSMIIC_Abstract->IIC_SCL_SET(0);
	pSMIIC_Abstract->SDA_OUT();
	pSMIIC_Abstract->IIC_SDA_SET(1);
	pSMIIC_Abstract->delay_us(D_SET_ACK);
	pSMIIC_Abstract->IIC_SCL_SET(1);
	pSMIIC_Abstract->delay_us(D_SET_ACK);
	pSMIIC_Abstract->IIC_SCL_SET(0);
}	
/**
  * @brief    
		IIC发送一个字节
		返回从机有无应答
		1，有应答
		0，无应答	
  * @param    None
  * @retval : None
**/		  
void IIC_Send_Byte(smiic_uint8 txd)
{                        
	smiic_uint8 t;   
	pSMIIC_Abstract->SDA_OUT(); 	    
	pSMIIC_Abstract->IIC_SCL_SET(0);//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{              
		//IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			pSMIIC_Abstract->IIC_SDA_SET(1);
		else
			pSMIIC_Abstract->IIC_SDA_SET(0);
		txd<<=1; 	  
		pSMIIC_Abstract->delay_us(D_SEND_BYTE);   //对TEA5767这三个延时都是必须的
		pSMIIC_Abstract->IIC_SCL_SET(1);
		pSMIIC_Abstract->delay_us(D_SEND_BYTE); 
		pSMIIC_Abstract->IIC_SCL_SET(0);	
		pSMIIC_Abstract->delay_us(D_SEND_BYTE);
	}	 
} 
/**
  * @brief    读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
  * @param    None
  * @retval : None
**/
smiic_uint8 IIC_Read_Byte(unsigned char ack)
{
	smiic_uint8 i,receive=0;

	pSMIIC_Abstract->SDA_IN();//SDA设置为输入
	
	for(i=0;i<8;i++ )
	{
		receive<<=1;
		pSMIIC_Abstract->IIC_SCL_SET(0); 
		pSMIIC_Abstract->delay_us(D_READ_BYTE);
		pSMIIC_Abstract->IIC_SCL_SET(1);
		pSMIIC_Abstract->delay_us(D_READ_BYTE);

		if(pSMIIC_Abstract->IIC_SDA_READ())receive++;   

	}					 
	if (!ack)
		IIC_NAck();//发送nACK
	else
		IIC_Ack(); //发送ACK 
	return receive;
}

