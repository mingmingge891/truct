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

/*�����ָ��*/
SM_IIC_Abstract_t *pSMIIC_Abstract;

/**
  * @brief   IIC ��ʼ��
  * @param    None
  * @retval : None
**/
void SM_IIC_Initialize(void)
{					     
	/*ʵ�������󻯽ṹ��*/
	pSMIIC_Abstract = &Implement_SMIIC_Abstract;

	/*��صĳ�ʼ��*/
	pSMIIC_Abstract->Init();
	
}
/**
  * @brief    ����IIC��ʼ�ź�
  * @param    None
  * @retval : None
**/
void IIC_Start(void)
{
	pSMIIC_Abstract->SDA_OUT();//sda�����
	pSMIIC_Abstract->IIC_SDA_SET(1);
	pSMIIC_Abstract->IIC_SCL_SET(1);
	pSMIIC_Abstract->delay_us(D_START_STOP_1);
	pSMIIC_Abstract->IIC_SDA_SET(0);//START:when CLK is high,DATA change form high to low 
	pSMIIC_Abstract->delay_us(D_START_STOP_1);
	pSMIIC_Abstract->IIC_SCL_SET(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
/**
  * @brief    ����IICֹͣ�ź�
  * @param    None
  * @retval : None
**/
void IIC_Stop(void)
{
	pSMIIC_Abstract->SDA_OUT();//sda�����
	pSMIIC_Abstract->IIC_SCL_SET(0);
	pSMIIC_Abstract->IIC_SDA_SET(0);//STOP:when CLK is high DATA change form low to high
	pSMIIC_Abstract->delay_us(D_START_STOP_1);
	pSMIIC_Abstract->IIC_SCL_SET(1); 
	pSMIIC_Abstract->IIC_SDA_SET(1);//����I2C���߽����ź�
	pSMIIC_Abstract->delay_us(D_START_STOP_1);	
}
/**
  * @brief    
		�ȴ�Ӧ���źŵ���
		����ֵ��1������Ӧ��ʧ��
						0������Ӧ��ɹ�
  * @param    None
  * @retval : None
**/
smiic_uint8 IIC_Wait_Ack(void)
{
	smiic_uint16 ucErrTime=0;
	pSMIIC_Abstract->SDA_IN();//SDA����Ϊ����
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
	pSMIIC_Abstract->IIC_SCL_SET(0);//ʱ�����0 	   
	return 0;  
} 
/**
  * @brief    ����ACKӦ��
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
  * @brief    ������ACKӦ��		
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
		IIC����һ���ֽ�
		���شӻ�����Ӧ��
		1����Ӧ��
		0����Ӧ��	
  * @param    None
  * @retval : None
**/		  
void IIC_Send_Byte(smiic_uint8 txd)
{                        
	smiic_uint8 t;   
	pSMIIC_Abstract->SDA_OUT(); 	    
	pSMIIC_Abstract->IIC_SCL_SET(0);//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{              
		//IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			pSMIIC_Abstract->IIC_SDA_SET(1);
		else
			pSMIIC_Abstract->IIC_SDA_SET(0);
		txd<<=1; 	  
		pSMIIC_Abstract->delay_us(D_SEND_BYTE);   //��TEA5767��������ʱ���Ǳ����
		pSMIIC_Abstract->IIC_SCL_SET(1);
		pSMIIC_Abstract->delay_us(D_SEND_BYTE); 
		pSMIIC_Abstract->IIC_SCL_SET(0);	
		pSMIIC_Abstract->delay_us(D_SEND_BYTE);
	}	 
} 
/**
  * @brief    ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
  * @param    None
  * @retval : None
**/
smiic_uint8 IIC_Read_Byte(unsigned char ack)
{
	smiic_uint8 i,receive=0;

	pSMIIC_Abstract->SDA_IN();//SDA����Ϊ����
	
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
		IIC_NAck();//����nACK
	else
		IIC_Ack(); //����ACK 
	return receive;
}

