/**
  ******************************************************************************
  * @file    dl_time.c
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
#include "dl_time.h"
#include "list.h"  
#include "dl_time.h"
#include "dl_time_interface.h"
#include "stdarg.h"
#include "malloc.h"
#include "string.h"

/* Values---------------------------------------------------------------------*/


/*定时器抽象函数结构体变量*/
TIMER_Hal_Interface_t* pTimerHal = NULL;
//链表又来保持定时器实例
Link_List **TimeInstancelist;

/**
  * @brief  This function
  * @param  None
  * @retval : 0 fail 1 succ
**/
static uint8_t osal_start_timerEx(timerEx_Instance_t* instance,uint8_t timerEx_id,
	void* p_arg,void (*ptr)(void*),uint32_t timer_period)
{
	if(timerEx_id > (instance->timerEx_Cnt-1))
	{
		return 0;
	}
	instance->timerEx_call[timerEx_id].t_callBack = ptr;
	instance->timerEx_call[timerEx_id].time_out = timer_period;
	instance->timerEx_call[timerEx_id].time_count = 0;
	instance->timerEx_call[timerEx_id].prg = p_arg;
	return 1;
}
/**
  * @brief  This function
  * @param  None
  * @retval : 0 fail 1 succ
**/
static uint8_t osal_stop_timerEx(timerEx_Instance_t* instance,uint8_t timerEx_id)
{
	if(timerEx_id > (instance->timerEx_Cnt-1))
	{
			return 0;
	}
	instance->timerEx_call[timerEx_id].time_out = 0;
	instance->timerEx_call[timerEx_id].time_count = 0;
	return 1;
}

uint8_t InstaceTimeStart(timerEx_Instance_t* instance,void* p_arg,void (*ptr)(void*),uint32_t args,...)
{
	va_list ap;
	va_start(ap, args);
	uint32_t timer_period = args;
	uint8_t timerEx_id;
	if(instance->timerEx_Cnt > 1)
	{
		timerEx_id = va_arg(ap,int);
	}
	if(timerEx_id > 8){
		timerEx_id = 0;
	}
	va_end(ap);
	return osal_start_timerEx(instance,timerEx_id,p_arg,ptr,timer_period);

}
void InstaceTimeStop(void *args,...)
{
	va_list ap;
	va_start(ap, args);
	timerEx_Instance_t *instance = (timerEx_Instance_t*)args;
	int timerEx_id = va_arg(ap,int);
	if(timerEx_id > 8){
		osal_stop_timerEx(instance,0);
	}
	do
	{
		if(timerEx_id < 8)
		{
			osal_stop_timerEx(instance,timerEx_id);
		}
		else{
			//当va_arg娶不到值时一定会大于8
			//因为void* 下一个地址一定是个32位的(ARM32)
			//寄存器地址
			break;
		}
		timerEx_id = va_arg(ap,int);
	}while(1);
	va_end(ap);
}


//实例化
timer_t *AlltimerNew(u8 HalId,u8 time_cnt)
{
	if(HalId > pTimerHal->Hal_time_num)
	{
		return NULL;
	}
	if(time_cnt > 8 || time_cnt ==0){
		return NULL;
	}
	timer_t *p = (timer_t*)my_malloc(sizeof(timer_t));
	p->start = InstaceTimeStart;
	p->stop  = InstaceTimeStop;
	p->instance = (timerEx_Instance_t*)my_malloc(sizeof(timerEx_Instance_t));
	p->instance->timerEx_call = (timerEx_callback_t*)my_malloc(time_cnt*sizeof(timerEx_callback_t));
	p->instance->timerEx_Cnt = time_cnt;
	//把实例添加到链表中
	Link_List_Insert(TimeInstancelist[HalId-1],p->instance,-1);
	pTimerHal->Timer_CMD(HalId,1);
	return p;
}
/**
  * @brief  循环执行
  * @param  id 硬件定时的编号
  * @retval : None
**/
void virtualTimerRun(uint8_t id)
{
	//得到链表对象
	timerEx_Instance_t* p = NULL;
	for(uint8_t i =0; i< TimeInstancelist[id]->length;i++)
	{
		p =	(timerEx_Instance_t*)Link_List_GetAt(TimeInstancelist[id],i);
		if(p == NULL)return;
		/*定时器复用*/
		for(uint8_t i = 0; i < p->timerEx_Cnt;i++)
		{
			/*当计数到最大值的时候 先清零计数器 当再次计数就会跳过0 
			防止与time_out相等*/
			if(p->timerEx_call[i].time_count == 0xFFFFFFFFul)
				p->timerEx_call[i].time_count = 0;
			
			/*判断计数值是否与定时值相等如果相等执行相应的回调函数*/
			if(++(p->timerEx_call[i].time_count) == p->timerEx_call[i].time_out)
			{
				p->timerEx_call[i].time_count = 0;
				p->timerEx_call[i].t_callBack(p->timerEx_call[i].prg);
			}
		}
	}
}
/**
  * @brief  This function 1ms 中断函数处理
  * @param  None
  * @retval : None
**/
static void TIM_CallBack(u8 id)
{
	if(id >=1)
	{
		virtualTimerRun(id-1);
	}
}

/**
  * @brief  This function 初始化系统定时器
  * @param 	void
  * @retval : None
**/
void init_timerEX(void)
{
	/*得到对象*/
	pTimerHal = &Implement_Timer_HAL_Interface;
	TimeInstancelist = (Link_List**)my_malloc(sizeof(Link_List*) * pTimerHal->Hal_time_num);
	if(pTimerHal->Init != NULL)
	{
		//初始化链表
		for(u8 i=0;i<pTimerHal->Hal_time_num;i++)
		{
			TimeInstancelist[i] = Link_List_Init();
			pTimerHal->Init(i+1,1);
		}
	}
	//注册硬件中断回调函数
	if(pTimerHal->RegisterTimerIrq != NULL)
	{
		pTimerHal->RegisterTimerIrq(TIM_CallBack);
	}
}
