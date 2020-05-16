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
#include "string.h"
#include "stdio.h"
#include "malloc.h"
#include "iterator.h"
#include "list.h"

//获取下一个元素
static void* thisNext(IteratorInterface *it)
{	
	if(it->container->length ==0)
	{
		it->cursor = 0;
		return NULL;
	}
	return Link_List_GetAt(it->container,++it->cursor);
}
//判断是否还有下一个元素
static bool thisHasNext(IteratorInterface *it)
{
	if(it->container->length == (it->cursor+1)){
		it->cursor = -1;
		return FALSE;
	}
	return TRUE;
}
//删除元素
static void thisFreeContainer(AggregateInterface *ag)
{
	for(uint8_t i = 0;i< ag->AggregateList->length;i++)
	{  
			ag->thisContainer = (u8*)Link_List_GetAt(ag->AggregateList,i);  
			my_free(ag->thisContainer);
	}  
	Link_List_Clear(ag->AggregateList);
}

//聚合添加数据
static void thisAddListDataContainer(AggregateInterface *ag,void *data,uint16_t length)
{
	ag->thisContainer = my_malloc(sizeof(length));
	memcpy(ag->thisContainer,data,length);
	Link_List_Insert(ag->AggregateList,ag->thisContainer,-1);
}
//设置聚合容器
static void thisSetContainer(AggregateInterface *ag,uint8_t *container,uint16_t length)
{
	for(uint8_t i=0;i<length;i++)
	{
		ag->thisContainer = (uint8_t*)my_malloc(sizeof(uint8_t));
		*(uint8_t*)ag->thisContainer = *container++;
		Link_List_Insert(ag->AggregateList,ag->thisContainer,-1);
	}
}
//具体迭代器
static IteratorInterface* thisIterator(AggregateInterface *ag)
{
	IteratorInterface *it =(IteratorInterface*) my_malloc(sizeof(IteratorInterface));
	it->next                 = thisNext;
	it->hasNext              = thisHasNext;
	it->cursor               = -1;
	it->container            = ag->AggregateList;
	return it;
}
//具体聚合
AggregateInterface* AggregateNew(void)
{
	AggregateInterface *ag   = (AggregateInterface*)my_malloc(sizeof(AggregateInterface));
	ag->addListDataContainer = thisAddListDataContainer;
	ag->setContainer         = thisSetContainer;
	ag->iterator             = thisIterator;
	ag->AggregateList        = Link_List_Init();
	ag->freeContainer        = thisFreeContainer;
	return ag;
}


