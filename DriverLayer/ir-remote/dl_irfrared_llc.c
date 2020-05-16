/**
  ******************************************************************************
  * @file 
  * @author  mh.j
  * @version v2.0
  * @date    2016.3.17
  * @brief   �߼���·����
  *       
  *         
  ******************************************************************************
  * @copy
***/ 

#include "dl_irfrared.h"
#include "string.h"
#include <stdio.h>

/*�������ݾ�̬����*/
static hstp_uint8 hstp_RevD[HSTP_FIFO_LENGTH];

/************************************************
������:		Comper_MAC
��������:	�Ƚ�MAC�Ƿ����
�����ӿ�:	data-����
����:	�鿴���ض���
************************************************/
hstp_int16 Comper_MAC(hstp_uint8 *D_MAC) {
	hstp_uint8 i;
	for(i=0;i<MAC_LEN;i++) {
			if(D_MAC[i] != MAC.S_MAC[i])
					return CRC_ERR;
	}
	return SUC;
}

/************************************************
������:		Down_Data_Analyse
��������:	�������ݽ���
�����ӿ�:	data-����
����:	�鿴���ض���
************************************************/
/**
**/
__User_Data_t* Up_Data_Analyse(__Data_LLC_t *Dhead) 
{
	static __User_Data_t User;
	
	/*ָ�������׵�ַ��Ŀ���ýṹ�����
	���ζ��������е�ÿһ��Ԫ��*/	
	User.p_Source_Data = Dhead;
	User.P_Result = REV_USER_DATA;
	
	return &User;
}
/************************************************
������:		Down_Data_Analyse
��������:	�������ݽ���
�����ӿ�:	data-����
����:	�鿴���ض���
************************************************/
__User_Data_t* Down_Data_Analyse(__Data_LLC_t *Dhead) 
{	  	
//	if(Comper_MAC(Dhead->MAC) != SUC){	
//  }
	return NULL;
}
/************************************************
������:		Data_Analyse
��������:	���ݽ���
�����ӿ�:	
����:
************************************************/
__User_Data_t* Data_Analyse(hstp_uint8 *data,hstp_uint8 length) {

	__Data_LLC_t *Dhead;
	
	/*��ֵ���µ������д���*/
	memcpy(hstp_RevD,data,length);
	Dhead = (__Data_LLC_t *)hstp_RevD;
	
	
	
//	/*����֡ͷ����*/
//	if(Dhead->head == UP_HEAD){
//		return Up_Data_Analyse(Dhead);
//	}
//	else if(Dhead->head == DOWN_HEAD)
//	{
//		return Down_Data_Analyse(Dhead);
//	}
	return Up_Data_Analyse(Dhead);
}
