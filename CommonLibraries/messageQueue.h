/**
  ******************************************************************************
  * @file    messageQueue.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    21-3-2019
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
#ifndef __MESSAGE_QUEUE_H_
#define __MESSAGE_QUEUE_H_
#include "adapter.h"


//定义节点，双向链表  消息队列
typedef struct _MessageQueue_Node{  
    void* data;//数据域  
    struct _MessageQueue_Node* priorNode;//指向上一个节点的指针  
    struct _MessageQueue_Node* nextNode;//指向下一个节点的指针  
}MessageQueue_Node;

//定义链表结构  
typedef struct _Link_List{
    MessageQueue_Node *head;//链表的头指针
    MessageQueue_Node *trail;//链表的尾部指针  
    unsigned long length;//链表的当前长度  
}Message_List;

/**
  * @brief 数据添加
  * @param  None
  * @retval : None
**/
int	MessageQueueAdd(Message_List* pList,void *pData,u16 length);

/**
  * @brief  数据获取依次从队列中获取数据
  * @param  None
  * @retval : 返回数据的指针
**/
void* MessageQueueGet(Message_List* pList);

/**
  * @brief  消息队列创建可以用动态的方式创建也可以用静态的方式创建
  * @param  None
  * @retval : None
**/
Message_List* MessageQueueCreate(void);

/*API 操作结构体*/
typedef struct
{
	int (*Add)(Message_List* pList,void *pData,u16 length);
	Message_List* (*Create)(void);
	void* (*Get)(Message_List* pList);
	
}MessageQueue_t;

extern MessageQueue_t MessageQueue;

#endif

