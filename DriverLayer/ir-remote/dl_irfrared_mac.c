/**
  ******************************************************************************
  * @file 
  * @author  mh.j
  * @version v2.0
  * @date    2016.3.17
  * @brief   介质访问控制层
  *       
  *         
  ******************************************************************************
  * @copy
***/ 
#include "dl_irfrared.h"
#include "string.h"

hstp_MAC_T MAC;
extern hstp_uint8 IR_Drv_Read_FIFO(hstp_uint8* pData);
extern Dl_Irfrared_Interface_t* pHSTP_Property;
extern hstp_int8  hstp_Drv_Send_MacData(hstp_MAC_T *pMac_Data);

/**
  * @brief  获取S_MAC
  * @param  None
  * @retval : None
*/
void GET_D_MAC(void){
	memcpy(MAC.S_MAC,pHSTP_Property->Get_UID(),S_MAC_LEN);
}

/**
  * @brief  This function 读hstp——FIFO
  * @param  out-> -1：指针无效；-2:非有效数据中断；>0:收到有效数据
  * @retval : None
*/
hstp_int8 hstp_Receive_Data(hstp_MAC_T *pMac_Data)
{
	hstp_int16 status = -1;
	if(pMac_Data != NULL)
	{
		pMac_Data->length = IR_Drv_Read_FIFO(pMac_Data->Buffer);
		status = pMac_Data->length;
	}
	return (status);
}
/************************************************
函数名:		Send_Data
函数功能:	发送数据
函数接口:	userH-发送数据头结构指针
		data-发送数据体指针
返回:	发送成功返回SUC,失败返回ERR
************************************************/
hstp_int16 hstp_Send_Data(__Data_LLC_t *user_SDH)
{
	hstp_uint8 err = 0;
	
	/*结构体赋值到数组*/
	memcpy(MAC.Buffer,(hstp_uint8*)user_SDH, sizeof(__Data_LLC_t));
	
	/*这里的长度是指的实际发送字节数*/
	MAC.length = sizeof(__Data_LLC_t);
	
	/*开始发送数据*/
	while(hstp_Drv_Send_MacData(&MAC) < 0x00 && err < 0x03)
	{
		pHSTP_Property->delay_ms(100);
		err++;
	}
	if(err == 0x03)
	{
		return SEND_ERR;
	}
	return SUC;
}
