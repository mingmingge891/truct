/**
  ******************************************************************************
  * @file    MAIN.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    24-1-2019
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.twc.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2019 HansenTech</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dl_system.h"
#include "dl_serial.h"
#include "dl_time.h"
#include "string.h"

static void call(uint8_t* d,uint16_t l)
{
	serial.print(serial.uart1,"%s",d);
}
int jh=-10000000;
void pcll(void* p)
{
	serial.print(serial.uart1,"%d\r\n",jh++);
}
/**
  * @brief  主函数入口
  * @param  None
  * @retval : None
**/
int main(void)
{  
	
	/*系统初始化*/
	SystemInitialize();
	
	/*串口*/
	serial.config(serial.uart1,115200);
	//serial.register_callback(HS_UART1,call,1,0);
	serial.print(serial.uart1,"hello world\r\n");
	
	timer.start(1,0,pcll,100);
	
	while(1)
	{
		serial.print(serial.uart1,"hello world\r\n");
	}
	
}
