/**
  ******************************************************************************
  * @file    observer.c
  * @author  hansen.jiang
  * @version v1.0.0
  * @date    2020-4-8
  * @brief   
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "dl_system.h"
#include "obServer.h"
#include "string.h"
#include "stdio.h"
#include "malloc.h"


Link_List *subjectList = NULL;

//添加订阅者方法
static void thisAdd(subject* s,obServer* ob)
{
		//这里必须使用内存分配不能用局部变量传过去
		//栈空间会回收处理,当取数据时指针引用的值会
		//变化
	  obServerMap *obMap = (obServerMap*)my_malloc(sizeof(obServerMap));
    obMap->key   = ob->name;
    obMap->value = ob;
    Link_List_Insert(s->obMapList,obMap,-1);
}
//删除订阅者方法
static void thisDel(subject* s,obServer* ob)
{
		object_t object;
		obServerMap obMap;
		obMap.key = ob->name;
		obMap.value = ob;
		object.data = &obMap;
		object.length = sizeof(obServerMap);
    Link_List_object_RemoveAt(s->obMapList,&object);
}
//通知方法
static void thisNotify(subject* s,const char* messages)
{
    obServerMap* kv = NULL;
    for(uint8_t i=0;i < s->obMapList->length; i++)
    {
        kv = (obServerMap*)Link_List_GetAt(s->obMapList,i);
        ((obServer*)kv->value)->upDataProc((obServer*)kv->value,s,messages);
    }
}
//创建一个新的主题订阅者
obServer* obServerNew(const char* obName,obMsgCallFunc obMsgCall)
{
    obServer *ob = (obServer*)my_malloc(sizeof(obServer));
    ob->name = (char*)my_malloc(sizeof(char)*strlen(obName));
		memcpy(ob->name,obName,strlen(obName));
		ob->upDataProc = obMsgCall;
    return ob;
}
//创建一个新的主题发布者
subject* subjectNew(const char* topicName)
{
    subject *s = (subject*)my_malloc(sizeof(subject));
    s->name = (char*)my_malloc(sizeof(char)*strlen(topicName));
    memcpy(s->name,topicName,strlen(topicName));
		s->obMapList = Link_List_Init();
    s->notify = thisNotify;
    s->add = thisAdd;
		s->del = thisDel;
    return s;
}

//创建独立发布订阅逻辑
subject* thisPublishTopic(char *topicName)
{
	obServerMap *obMap = NULL;
	//判断名字是否相同
	for(uint8_t i=0;i<subjectList->length;i++)
	{
		obMap = (obServerMap*)Link_List_GetAt(subjectList,i);
		if(strcmp((char*)obMap->key,topicName)==0){
			return NULL;
		}
	}
	obMap = (obServerMap*)my_malloc(sizeof(obServerMap));
	obMap->key   = topicName;
	obMap->value = subjectNew(topicName);
	Link_List_Insert(subjectList,obMap,-1);
	
	return obMap->value;
}
uint8_t thisSubscribeTopic(char *obName,const char *topicName,obMsgCallFunc obMsgCall)
{
	obServerMap *obMap = NULL;
	for(uint8_t i=0;i<subjectList->length;i++)
	{
		obMap = (obServerMap*)Link_List_GetAt(subjectList,i);
		//找到订阅的主题名字
		if(strcmp((char*)obMap->key,topicName)==0){	
			subject *s = (subject*)obMap->value;
			
			//判断订阅者的名字是否相同
			for(uint8_t i=0;i<s->obMapList->length;i++)
			{
				obServerMap *obMap = (obServerMap*)Link_List_GetAt(s->obMapList,i);
				if(strcmp((char*)obMap->key,obName)==0){
					return 0;
				}
			}
			s->add(s,obServerNew(obName,obMsgCall));
			return 1;
		}
	}
	return 0;
}
//单例模式
ObserverPattern_t* ObserverSingletonNew(void)
{
	static ObserverPattern_t ObserverPattern;
	ObserverPattern.PublishTopic   = thisPublishTopic;
	ObserverPattern.SubscribeTopic = thisSubscribeTopic;
	if(subjectList==NULL)
	{
		subjectList = Link_List_Init();
	}	
	return &ObserverPattern;
}

#if 0
//主函数 测试
int main(void)
{
	//初始化系统
	SystemInitialize();
	serial.config(serial.uart2,115200);
	//发布主题
	subject  *s = subjectNew("button");
	//订阅者
	obServer *obA = obServerNew("mainA",upDataproc);
	obServer *obB = obServerNew("mainB",upDataproc);
	obServer *obC = obServerNew("mainC",upDataproc);
	//添加订阅者
	s->add(s,obA);
	s->add(s,obB);
	s->add(s,obC);
	//删除订阅者
	s->del(s,obB);
	//通知
	s->notify(s,"{hello world}");
	while(1);
}
#endif
