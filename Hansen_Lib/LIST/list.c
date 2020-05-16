#include "list.h"  
#include "malloc.h"
#include <stdlib.h>  
#include "stdio.h"
#include "string.h"  
/** 
 * 功能：初始化链表 
 * 返回值：如果成功，则返回链表的地址，如果失败返回NULL 
 */  
Link_List* Link_List_Init(void)  
{  
	Link_List* pList = NULL;  
	pList = (Link_List*)my_malloc(sizeof(Link_List));  
	if(pList == NULL)  
	{  
		return NULL;
	}  
	pList->length = 0;  
	pList->head = NULL;
	pList->trail = NULL;
	return pList;
}  
  
/** 
 * 功能：随机插入链表 
 * 参数： 
 *      pList：链表地址 
 *      pData：插入的数据节点 
 *      index：要插入的位置，如果为0，则默认从链表的开始处插入，如果为-1，则默认从链表的最后插入 
 * 返回值：成功返回0，失败返回-1 
 */  
int Link_List_Insert(Link_List* pList,void* pData,long index)  
{  
    long i = 0;  
    if(pList == NULL)  
        return -1;  
    if(index < -1 || (index > pList->length && index != -1))  
    {  
        return -1;  
    }  
    //判断是否是首次插入  
    if(pList->length == 0)  
    {  
        Link_Node* pNode = (Link_Node*)my_malloc(sizeof(Link_Node));  
        if(pNode == NULL)  
            return -1;  
        pNode->data = pData;  
        pNode->priorNode = NULL;  
        pNode->nextNode = NULL;  
        pList->head = pNode;
        pList->trail = pNode;  
        pList->length++;  
        return 0;  
    }  
    else
    {  
        if(-1 == index)//从末尾处插入  
        {  
            //创建节点  
            Link_Node* pNode = (Link_Node*)my_malloc(sizeof(Link_Node));  
            if(pNode == NULL)  
                return -1;  
            pNode->data = pData;  
            pNode->nextNode = NULL;
            pNode->priorNode = pList->trail;  
            //将节点加到末尾处  
            pList->trail->nextNode = pNode;
            pList->trail = pNode;  
            pList->length++;  
        }  
        else if(0 == index) //从开始处插入  
        {  
            //创建节点  
            Link_Node* pNode = (Link_Node*)my_malloc(sizeof(Link_Node));  
            if(pNode == NULL)  
                return -1;  
            pNode->data = pData;  
            pNode->nextNode = pList->head;  
            pNode->priorNode = NULL;  
            //将节点加载到头部  
            pList->head->priorNode = pNode;  
            pList->head = pNode;  
            pList->length++;  
            return 0;  
        }  
        else//指定位置插入  
        {  
            //后期可以使用快速查找算法优化  
            Link_Node* pNode = pList->head;  
            i=0;  
            while(pNode != NULL)  
            {  
                if(index == i)//查找到要插入的位置节点  
                {  
                    //创建节点  
                    Link_Node* pCurrentNode = (Link_Node*)my_malloc(sizeof(Link_Node));  
                    if(pCurrentNode == NULL)  
                        return -1;  
                    pCurrentNode->nextNode = pNode;  
                    pCurrentNode->priorNode = pNode->priorNode;  
										//pCurrentNode->data = pData;
                    pNode->priorNode->nextNode = pCurrentNode;  
                    pNode->priorNode = pCurrentNode;  
                    pList->length++;  
                    return 0;  
                }  
                pNode = pNode->nextNode;  
                i++;
            }  
        }  
    }  
    return 0;  
}  
  
/** 
 * 功能：从某个位置取出元素 
 * 参数： 
 *      pList：链表地址 
 *      index：位置 
 * 返回值：返回数据体指针 
 */  
void* Link_List_GetAt(Link_List* pList,unsigned long index)  
{  
    int i = 0;  
    Link_Node* pNode = NULL;  
    if(pList == NULL)  
    {  
        return NULL;  
    }  
    i = 0;  
    pNode = pList->head;  
    while(pNode != NULL)  
    {  
        if(i == index)  
        {  
            return pNode->data;  
        }  
        pNode = pNode->nextNode;  
        i++;  
    }  
    return NULL;  
}  

//任意数据类型对比
int isEqual(unsigned char *Sdata,unsigned char *Ddata,int length)
{		
	for(int i=0;i<length;i++)
	{
		if(Sdata[i] != Ddata[i])
		{
			return 0;
		}
	}
	return 1;
}
//通过对象查询
int Link_List_object_SearchAt(Link_List* pList,object_t* object)
{
	Link_Node* pNode = NULL;
	if(pList == NULL)  
	{  
			return -1;  
	}  
	int i=0;
	pNode = pList->head; 
	while(pNode != NULL)
	{   
		if(isEqual((unsigned char*)pNode->data,(unsigned char*)object->data,object->length))//找到节点
		{
			return i;
		}
		pNode = pNode->nextNode;
		i++;
	}  
	return -1;
}

//通过对象删除
void Link_List_object_RemoveAt(Link_List* pList,object_t* object)
{
	Link_Node* pNode = NULL;
	if(pList == NULL)  
	{  
			return;  
	}  
	pNode = pList->head;
	while(pNode != NULL)  
	{   
		if(isEqual((unsigned char*)pNode->data,(unsigned char*)object->data,object->length))//找到节点
		{
			if(pNode->priorNode == NULL) //表示链表头部节点  
			{  
					Link_Node* head = pList->head;  
					pList->head = head->nextNode;  
					pList->head->priorNode = NULL;
					my_free(head);//释放节点  
					pList->length--;
			}  
			else if(pNode->nextNode == NULL)//表示链表尾部节点  
			{  
					Link_Node* trial = pList->trail;  
					pList->trail = trial->priorNode;  
					pList->trail->nextNode = NULL;  
					my_free(trial);//释放节点  
					pList->length--;  
			}  
			else  
			{  
					//当前节点的上一个节点的下一个节点指向当前节点的下一个节点  
					pNode->priorNode->nextNode = pNode->nextNode;  
					//当前节点的下一个节点的上一个节点指向当前节点的上一个节点  
					pNode->nextNode->priorNode = pNode->priorNode;  
					my_free(pNode);  
					pList->length--;  
			}  
			return;  
		}
		pNode = pNode->nextNode;
	}  
}

/** 
 * 功能：通过索引删除元素，删除元素只是将data域置为NULL，并不会释放data指针，由调用者释放 
 * 参数： 
 *      pList：链表地址 
 *      index：位置 
 */  
void Link_List_RemoveAt(Link_List* pList,unsigned long index)  
{  
    int i = 0;  
    Link_Node* pNode = NULL;  
    if(pList == NULL)  
    {  
        return;  
    }  
    i = 0;  
    pNode = pList->head;  
    while(pNode != NULL)
    {  
        if(i == index)
        {  
						if(pList->length == 0x01)//如果只有一个节点删除就清空链表
						{
							Link_List_Clear(pList);
							return;
						}
            //找到节点，开始删除
            if(pNode->priorNode == NULL) //表示链表头部节点  
            {  
                Link_Node* head = pList->head;
                pList->head = head->nextNode;
                pList->head->priorNode = NULL;
                my_free(head);//释放节点
                pList->length--;
            }  
            else if(pNode->nextNode == NULL)//表示链表尾部节点  
            {  
                Link_Node* trial = pList->trail;  
                pList->trail = trial->priorNode;  
                pList->trail->nextNode = NULL;
                my_free(trial);//释放节点
                pList->length--;
            }  
            else  
            {  
                //当前节点的上一个节点的下一个节点指向当前节点的下一个节点  
                pNode->priorNode->nextNode = pNode->nextNode;  
                //当前节点的下一个节点的上一个节点指向当前节点的上一个节点  
                pNode->nextNode->priorNode = pNode->priorNode;  
                my_free(pNode);  
                pList->length--;  
            }  
            return;  
        }  
        pNode = pNode->nextNode;  
        i++;  
    }  
}  
  
/** 
 * 功能：清空链表 
 * 参数： 
 *  pList：链表地址 
 */  
void Link_List_Clear(Link_List* pList)  
{  
    //将数据域置为空  
    Link_Node* pNode = NULL;  
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
}  
  
/** 
 * 功能：释放链表空间 
 * 参数： 
 *  pList：链表地址 
 */  
void Link_List_my_free(Link_List* pList)  
{  
	Link_Node* pNode = NULL;  
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
	my_free(pList);
	pList = NULL;
}  

#if 0

#include "stm32f10x.h"
 typedef struct _packed{
	char a;
	int b;
	unsigned char c;
	float d;
}_test_1;
 
_test_1  test_1[5]; 

/** 
 * 功能：Link_List测试 
 */  
int *p = NULL; 

void Link_List_Test()  
{  
	Link_List* list = Link_List_Init();  
	
	object_t ob;
	
	for(unsigned char i=0;i<5;i++)
	{
		test_1[i].a = 1 +i;
		test_1[i].b =-1 +i;
		test_1[i].c = 4 +i;
		test_1[i].d = 1.2 +i;
		Link_List_Insert(list,&test_1[i],-1);
	}
	for(u8 i=0;i<list->length;i++)
	{
		_test_1* p = (_test_1*)Link_List_GetAt(list,i);
		printf("%d,%d,%d,%f,len=%d\r\n",p->a,p->b,p->c,p->d,list->length);
	}
	ob.data = &test_1[3];
	ob.length = sizeof(_test_1);
	printf("->%d\r\n",ob.length);
	//Link_List_object_RemoveAt(list,&ob);
	//Link_List_RemoveAt(list,2);
	
	printf("--->%d--------------\r\n",Link_List_object_SearchAt(list,&ob));
	for(u8 i=0;i<list->length;i++)
	{
		_test_1* p = (_test_1*)Link_List_GetAt(list,i);
		printf("%d,%d,%d,%f,len=%d\r\n",p->a,p->b,p->c,p->d,list->length);
	}
	while(1);
	printf("sjkd");
    int i = 0;  
     
    Link_List* list = Link_List_Init();  
    //动态添加1000个值  
    for(i = 0;i < 1;i++)  
    {  
        p = (int*) my_malloc(sizeof(int));  
        *p = i;  
        Link_List_Insert(list,p,-1);
    }  
    //取出第500个元素  
    p = (int*)Link_List_GetAt(list,0);  
		printf("%d\r\n",*p);
    //删除第500个元素，删除之后记得释放  
    Link_List_RemoveAt(list,0);
		printf("ok\r\n");
    my_free(p);  
    //重新获取第500个元素  
    p = (int*)Link_List_GetAt(list,499);  
    //开始释放空间  
    for(i = 0;i<list->length;i++)  
    {  
        p = (int*)Link_List_GetAt(list,i);  
        my_free(p);  
    }  
    Link_List_Clear(list);  
    Link_List_my_free(list);  
while(1);

}
#endif
