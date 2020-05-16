/**
  ******************************************************************************
  * @file    iterator.h
  * @author  HANSEN.JIANG     emil:951868692@qq.com
  * @version V1.0.0
  * @date    04-11-2020
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
#ifndef __ITERATOR_H_
#define __ITERATOR_H_
#include "list.h"  
typedef struct _IteratorInterface IteratorInterface;
typedef struct _AggregateInterface AggregateInterface;

//抽象迭代器接口
struct _IteratorInterface
{
	void*    (*next)(IteratorInterface *it);
  bool     (*hasNext)(IteratorInterface *it);
	Link_List *container;
	int cursor;
};

//抽象聚合接口
struct _AggregateInterface{
    IteratorInterface* (*iterator)(AggregateInterface *ag);
		void     (*addListDataContainer)(AggregateInterface *ag,void *data,uint16_t length);
		void     (*setContainer)(AggregateInterface *ag,uint8_t *container,uint16_t length);
	  void     (*freeContainer)(AggregateInterface *ag);
		Link_List *AggregateList;
		void     *thisContainer;
};

//具体聚合
AggregateInterface* AggregateNew(void);

#endif
