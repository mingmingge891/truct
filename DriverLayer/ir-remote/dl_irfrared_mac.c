/**
  ******************************************************************************
  * @file 
  * @author  mh.j
  * @version v2.0
  * @date    2016.3.17
  * @brief   ���ʷ��ʿ��Ʋ�
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
  * @brief  ��ȡS_MAC
  * @param  None
  * @retval : None
*/
void GET_D_MAC(void){
	memcpy(MAC.S_MAC,pHSTP_Property->Get_UID(),S_MAC_LEN);
}

/**
  * @brief  This function ��hstp����FIFO
  * @param  out-> -1��ָ����Ч��-2:����Ч�����жϣ�>0:�յ���Ч����
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
������:		Send_Data
��������:	��������
�����ӿ�:	userH-��������ͷ�ṹָ��
		data-����������ָ��
����:	���ͳɹ�����SUC,ʧ�ܷ���ERR
************************************************/
hstp_int16 hstp_Send_Data(__Data_LLC_t *user_SDH)
{
	hstp_uint8 err = 0;
	
	/*�ṹ�帳ֵ������*/
	memcpy(MAC.Buffer,(hstp_uint8*)user_SDH, sizeof(__Data_LLC_t));
	
	/*����ĳ�����ָ��ʵ�ʷ����ֽ���*/
	MAC.length = sizeof(__Data_LLC_t);
	
	/*��ʼ��������*/
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
