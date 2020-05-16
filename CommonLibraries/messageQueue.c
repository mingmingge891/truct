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

/* Includes ------------------------------------------------------------------*/
#include "messageQueue.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <stdio.h>

/* macro ---------------------------------------------------------------------*/

/*funtion define -------------------------------------------------------------*/


/*Value ----------------------------------------------------------------------*/

MessageQueue_t MessageQueue = 
{
	.Create = MessageQueueCreate,
	.Add = MessageQueueAdd,
	.Get = MessageQueueGet,
};

/*Implement ------------------------------------------------------------------*/

/** 
 * 功能：释放链表空间单保留List
 * 参数： 
 *  pList：链表地址 
 */  
static void Message_List_my_free(Message_List* pList)  
{  
	MessageQueue_Node* pNode = NULL;  
	if(pList == NULL)  
	{  
		return;
	}
	//从尾部开始释放  
	pNode = pList->trail;  
	while(pNode != NULL)  
	{  
		pList->trail = pNode->priorNode;  
		my_free(pNode);  
		pNode = pList->trail;
	}  
	pList->length = 0;
	pList->head = NULL;
	pList->trail = NULL;
	//my_free(pList);
	//pList = NULL;
} 

/**
  * @brief  删除数据根据节点来删除
  * @param  None
  * @retval : None
**/
static void MessageQueueRemove(Message_List* pList,MessageQueue_Node* pNode)
{
	if(pList->length == 0x01)//如果只有一个节点删除就清空链表
	{
		my_free(pNode->data);
		Message_List_my_free(pList);
		return;
	}
	if(pNode->priorNode == NULL) //表示链表头部节点  
	{  
		MessageQueue_Node* head = pList->head;  
		pList->head = head->nextNode;  
		pList->head->priorNode = NULL;
		my_free(pNode->data);//释放数据
		my_free(head);//释放节点  
		pList->length--;
	}
	else if(pNode->nextNode == NULL)//表示链表尾部节点  
	{  
		MessageQueue_Node* trial = pList->trail;  
		pList->trail = trial->priorNode;  
		pList->trail->nextNode = NULL;  
		my_free(pNode->data);//释放数据
		my_free(trial);//释放节点  
		pList->length--;
	}  
	else  
	{  
		//当前节点的上一个节点的下一个节点指向当前节点的下一个节点  
		pNode->priorNode->nextNode = pNode->nextNode;  
		//当前节点的下一个节点的上一个节点指向当前节点的上一个节点  
		pNode->nextNode->priorNode = pNode->priorNode;  
		my_free(pNode->data);//释放数据
		my_free(pNode);  
		pList->length--;  
	}
}
/**
  * @brief 数据添加
  * @param  None
  * @retval : None
**/
int	MessageQueueAdd(Message_List* pList,void *pData,u16 length)
{
	if(pList == NULL){
		return -1;
	}
	
	//首先分配数据域内存 然后复制到分配的区域
	u8 *pThisData = (u8*)my_malloc(length);
	if(pThisData == NULL){
		return -2;
	}
	
	memcpy(pThisData,(u8*)pData,length);
	
	//判断是否是首次插入
	if(pList->length == 0)
	{  
		MessageQueue_Node* pNode = (MessageQueue_Node*)my_malloc(sizeof(MessageQueue_Node));  
		if(pNode == NULL)  
				return -1;
		pNode->data = pThisData;
		pNode->priorNode = NULL;  
		pNode->nextNode = NULL;
		pList->head = pNode;
		pList->trail = pNode;
		pList->length++;
		return 0;
	}
	else
	{
		//创建节点  
		MessageQueue_Node* pNode = (MessageQueue_Node*)my_malloc(sizeof(MessageQueue_Node));  
		if(pNode == NULL)  
				return -1;  
		pNode->data = pThisData;  
		pNode->nextNode = NULL;
		pNode->priorNode = pList->trail;
		//将节点加到末尾处  
		pList->trail->nextNode = pNode;
		pList->trail = pNode;
		pList->length++;
		return 0;
	}
}
/**
  * @brief  数据获取依次从队列中获取数据
  * @param  None
  * @retval : None
**/
void* MessageQueueGet(Message_List* pList)
{
	static MessageQueue_Node* pNode = NULL;
	
	if(pNode == NULL){
		/*指向此链表的头*/
		pNode = pList->head;
	}
	else{
		/*先删除上次已经使用过的数据*/
		MessageQueueRemove(pList,pNode);
		pNode = pNode->nextNode;
	}
	
	/*如果队列不为空*/
	if(pNode != NULL){
		/*返回数据*/
		return pNode->data;
	}
	
	return NULL;
}
/**
  * @brief  消息队列创建可以用动态的方式创建也可以用静态的方式创建
  * @param  None
  * @retval : None
**/
Message_List* MessageQueueCreate(void)
{
	Message_List* pList = NULL;
	pList = (Message_List*)my_malloc(sizeof(Message_List));  
	if(pList == NULL)
	{  
		return NULL;
	}  
	pList->length = 0;  
	pList->head = NULL;
	pList->trail = NULL;
	return pList;
}
