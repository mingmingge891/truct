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
#include "string.h"
#include "obServer.h"

static void msgCallA(obServer* ob,subject* s,const char* messges)
{
	serial.print(serial.uart2,"this msgCallA su=[%s],ob=[%s],msg=[%s]\n",s->name,ob->name,messges);
}
static void msgCallB(obServer* ob,subject* s,const char* messges)
{
	serial.print(serial.uart2,"this msgCallB su=[%s],ob=[%s],msg=[%s]\n",s->name,ob->name,messges);
}

//主函数
int main(void)
{
	//初始化系统
	SystemInitialize();
	serial.config(serial.uart2,115200);
	serial.print(serial.uart2,"observer demo test ...\n");
	//发布主题
	subject *s = ObserverSingletonNew()->PublishTopic("button");
	//订阅主题
	ObserverSingletonNew()->SubscribeTopic("mianA","button",msgCallA);
	ObserverSingletonNew()->SubscribeTopic("mianB","button",msgCallB);
	ObserverSingletonNew()->SubscribeTopic("mianC","button",msgCallB);
	ObserverSingletonNew()->SubscribeTopic("mianD","button",msgCallB);
	ObserverSingletonNew()->SubscribeTopic("mianE","button",msgCallB);
	ObserverSingletonNew()->SubscribeTopic("mianE","button",msgCallB);
	s->notify(s,"hello world");
	while(1){
		//serial.print(serial.uart2,".");
	}
}
