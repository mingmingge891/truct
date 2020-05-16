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
#include "dl_system.h"
#include "dl_time.h"
#include "dl_serial.h"
#include "iterator.h"
#include "string.h"

u8 test[5]={1,2,3,4,5};
//主函数
int main(void)
{
	//初始化系统
	SystemInitialize();
	serial.config(serial.uart2,115200);
	serial.print(serial.uart2,"iterator demo test ...\n");
	AggregateInterface *ag = AggregateNew();
	ag->setContainer(ag,(u8*)"hello wolrd\r\n",strlen("hello wolrd\r\n"));
	IteratorInterface *it = ag->iterator(ag);
	while(it->hasNext(it))
	{
		serial.print(serial.uart2,"%c\n,%d",*(uint8_t*)it->next(it),it->cursor);
	}
	while(1){
		//serial.print(serial.uart2,".");
	}
}
