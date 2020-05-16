/**
  ******************************************************************************
  * @file 
  * @author  mh.j
  * @version v2.0
  * @date    2016.3.17
  * @brief   逻辑链路控制
  *       
  *         
  ******************************************************************************
  * @copy
***/ 

#include "dl_irfrared.h"
#include "string.h"
#include <stdio.h>

/*接收数据静态缓存*/
static hstp_uint8 hstp_RevD[HSTP_FIFO_LENGTH];

/************************************************
函数名:		Comper_MAC
函数功能:	比较MAC是否相等
函数接口:	data-数据
返回:	查看返回定义
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
函数名:		Down_Data_Analyse
函数功能:	上行数据解析
函数接口:	data-数据
返回:	查看返回定义
************************************************/
/**
**/
__User_Data_t* Up_Data_Analyse(__Data_LLC_t *Dhead) 
{
	static __User_Data_t User;
	
	/*指向数组首地址，目的让结构体变量
	依次对齐数组中的每一个元素*/	
	User.p_Source_Data = Dhead;
	User.P_Result = REV_USER_DATA;
	
	return &User;
}
/************************************************
函数名:		Down_Data_Analyse
函数功能:	下行数据解析
函数接口:	data-数据
返回:	查看返回定义
************************************************/
__User_Data_t* Down_Data_Analyse(__Data_LLC_t *Dhead) 
{	  	
//	if(Comper_MAC(Dhead->MAC) != SUC){	
//  }
	return NULL;
}
/************************************************
函数名:		Data_Analyse
函数功能:	数据解析
函数接口:	
返回:
************************************************/
__User_Data_t* Data_Analyse(hstp_uint8 *data,hstp_uint8 length) {

	__Data_LLC_t *Dhead;
	
	/*赋值到新的数组中处理*/
	memcpy(hstp_RevD,data,length);
	Dhead = (__Data_LLC_t *)hstp_RevD;
	
	
	
//	/*数据帧头分析*/
//	if(Dhead->head == UP_HEAD){
//		return Up_Data_Analyse(Dhead);
//	}
//	else if(Dhead->head == DOWN_HEAD)
//	{
//		return Down_Data_Analyse(Dhead);
//	}
	return Up_Data_Analyse(Dhead);
}
