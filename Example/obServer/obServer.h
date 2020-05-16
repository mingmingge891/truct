/**
  ******************************************************************************
  * @file    observer.h
  * @author  HANSEN.JIANG     emil:951868692@qq.com
  * @version V1.0.0
  * @date    04-08-2020
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.wisdudu.com
	* This code developed by hardware science and technology division 
	* of chongqing fanghuitechnology co., LTD 
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2016 WIS-DUDU</center></h2>
  ******************************************************************************
  */
#ifndef __OB_SERVER_H_
#define __OB_SERVER_H_

#include "list.h"

//map 存储key value
typedef struct 
{
    void *key;
    void *value;
}obServerMap;

//结构体类型定义
typedef struct _subject subject;
typedef struct _obServer obServer;

//主题发布者
struct _subject
{
    char* name;
		//list用来存储 key value
    Link_List *obMapList;
    void (*notify)(subject* s,const char* messges);
    void (*add)(subject* s,obServer* ob);
		void (*del)(subject* s,obServer* ob);
};

//主题订阅者
struct _obServer
{
    char* name;
    void (*upDataProc)(obServer* ob,subject* s,const char* messges);
};
typedef void (*obMsgCallFunc)(obServer* ob,subject* s,const char* messges);

//API
typedef struct
{
	subject* (*PublishTopic)(char *topicName);
	uint8_t  (*SubscribeTopic)(char *obName,const char *topicName,obMsgCallFunc obMsgCall);
}ObserverPattern_t;
ObserverPattern_t* ObserverSingletonNew(void);

#endif
