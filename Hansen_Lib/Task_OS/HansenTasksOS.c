/**
  ******************************************************************************
  * @file    Task_OS.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.0.0
  * @date    29-june-2018
  * @brief   
  ******************************************************************************
  * @attention
  * http://www.tfx.com
	* if the developer want to use the code, and the consequences shall 
	* be borne by himself. 
	* @note
  * it is belongs to the open source code.
  * <h2><center>&copy; COPYRIGHT 2018 WWW.TFC.COM</center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "HansenTasksOs.h"
#include "stdlib.h"
#include "malloc.h"	    

TaskOs_callBack_t *task_call;//任务回调函数指针
uint8_t Timer_tasksCnt;//系统滴答时钟计数器
uint32_t Tick_Cnt = 0;//系统的滴答时钟
Task_t OsTask;//任务回调时上传的参数

/**
  * @brief  任务挂起时需要执行的回调函数地址设置
  * @param  this_priority 当前任务优先级
  * @retval : 1 suc 0fail
**/
static void SetTaskStop_Cb(uint8_t this_priority ,void(*ptr)(void))
{
	
	task_call[this_priority].SuspendCb = ptr;
	
}
/**
  * @brief  任务恢复时需要执行的回调函数地址设置
  * @param  this_priority 当前任务优先级
  * @retval : 1 suc 0 fail
**/
static void SetTaskStart_Cb(uint8_t this_priority ,void(*ptr)(void))
{
	
	if(this_priority > (Timer_tasksCnt-1))
	{
		return;
	}
	
	task_call[this_priority].ResumeCb = ptr;
	
}

/**
  * @brief  任务挂起
  * @param  priority 优先级
  * @retval : None
**/
void Timer_Task_OS_Suspend(uint8_t priority)
{
	if(priority > (Timer_tasksCnt-1))
	{
		return;
	}
	
	/*执行当前任务挂起回调事件*/
	
	if(task_call[priority].SuspendCb != NULL)
	{
		task_call[priority].SuspendCb();
	}
	
	task_call[priority].IsSuspend = 1;
}

/**
  * @brief 恢复挂起的任务
  * @param  priority 优先级
  * @retval : None
**/
void Timer_Task_OS_Resume(uint8_t priority)
{
	if(priority > (Timer_tasksCnt-1))
	{
		return;
	}
	
	/*执行当前任务恢复回调事件*/
	
	if(task_call[priority].ResumeCb != NULL)
	{
		task_call[priority].ResumeCb();
	}
	
	task_call[priority].IsSuspend = 0;
}
/**
  * @brief  更改任务定时周期
  * @param  priority  timer_period
  * @retval : 0 fail 1 succ
**/
uint8_t Timer_Task_OS_UpdataPeriod(uint8_t priority,uint32_t timer_period)
{
	if(priority > (Timer_tasksCnt-1))
	{
			return 0;
	}
	/*timer_period = NULL 以最快速度执行TASK 不延时,单片机的指令速度*/
	if(timer_period == NULL)
	{
		task_call[priority].no_timer = 0x01;
	}
	else
	{
		task_call[priority].no_timer = 0x00;
	}
	task_call[priority].time_out = timer_period;
	task_call[priority].NextTick = Tick_Cnt + timer_period;
	task_call[priority].OS_Ready = 0x01;
	return 1;
}

/**
  * @brief  Task_OS_Create
  * @param  priority 0最高 ,ptr:任务 ,timer_period 时间周期 为NULL=最快速度执行任务不延时
  * @retval : 0 fail 1 succ
**/
uint8_t Timer_Task_OS_Create(uint8_t priority,void (*task)(void*),uint32_t timer_period)
{
	if(priority > (Timer_tasksCnt-1))
	{
			return 0;
	}
	/*timer_period = NULL 以最快速度执行TASK 不延时,单片机的指令速度*/
	if(timer_period == NULL)
	{
		task_call[priority].no_timer = 0x01;
	}
	else
	{
		task_call[priority].no_timer = 0x00;
	}
	task_call[priority].task_callBack = task;
	task_call[priority].time_out = timer_period;
	task_call[priority].NextTick = timer_period;
	task_call[priority].OS_Ready = 0x01;
	return 1;
}

/**
  * @brief  Task_OS_Init
  * @param  task_cnt 任务个数 最大255个
  * @retval : 0 fail 1 success
**/
uint8_t Timer_Task_OS_Init(uint8_t task_cnt)
{
	if(task_cnt >255)
	{
		return 0;
	}
	
	Timer_tasksCnt = task_cnt;
	
	task_call = (TaskOs_callBack_t *)my_malloc( sizeof( TaskOs_callBack_t) * Timer_tasksCnt);
	
	if(task_call == NULL)
	{
		return 0;
	}
	
	/*初始化*/
	for(int i=0;i < Timer_tasksCnt;i++)
	{
		task_call[i].NextTick = 0;
		task_call[i].time_out = 0;
		task_call[i].task_callBack = NULL;
		task_call[i].no_timer = 0;
		task_call[i].OS_Ready = 0;
		task_call[i].InitFlag = 1;
		task_call[i].IsSuspend =0;//默认不挂起任务
	}
	
	/*启动任务停止任务的回调函数地址设置的函数实例化*/
	OsTask.SetTaskStart_Cb = SetTaskStart_Cb;
	OsTask.SetTaskStop_Cb = SetTaskStop_Cb;
		
	return 1;
	
}

/**
  * @brief  
  * @param  None
  * @retval : None
**/
static uint32_t GetTimingTick(void)
{
	if(Tick_Cnt > 4294960000ul)
	{
		for(int i=0;i < Timer_tasksCnt;i++)
		{
			task_call[i].NextTick = 0;
			task_call[i].time_out = 0;
			task_call[i].task_callBack = NULL;
			task_call[i].no_timer = 0;
			task_call[i].OS_Ready = 0;
		}
		Tick_Cnt = 0;
	}
	return  Tick_Cnt;
}

/**
  * @brief  轮训执行任务 任务可以延时
  * @param  None
  * @retval : None
**/
void Timer_Task_OS_Run(void)
{
	for(;;)
	{
		for(int i = 0; i < Timer_tasksCnt;i++)
		{
			/*无需要延时 直接运行延时是单片机的指令延时*/
			if(task_call[i].no_timer == 0x01)
			{
				OsTask.SysTimerCnt = Tick_Cnt;
				OsTask.this_priority = i;
				if(task_call[i].InitFlag == 1)
				{
					OsTask.InitFlag = 1;
					if(!task_call[i].IsSuspend){
						task_call[i].task_callBack(&OsTask);
					}
					task_call[i].InitFlag = 0;
				}
				else
				{
					OsTask.InitFlag = 0;
					if(!task_call[i].IsSuspend){
						task_call[i].task_callBack(&OsTask);
					}
				}
			}
			else
			{
				/*时间到达 且任务就绪*/
				if(( task_call[i].NextTick <= GetTimingTick() ) && ( task_call[i].OS_Ready ))
				{
					OsTask.SysTimerCnt = Tick_Cnt;
					OsTask.this_priority = i;//得到当前优先级号
					if(task_call[i].InitFlag == 1)
					{
						OsTask.InitFlag = 1;
						/*下一刻的时间等于当前时间+定时时间*/
						task_call[i].NextTick = (Tick_Cnt+task_call[i].time_out);
						
						/*判断当前任务是否挂起*/
						if(!task_call[i].IsSuspend){
							task_call[i].task_callBack(&OsTask);
						}
						task_call[i].InitFlag = 0;
					}
					else
					{
						OsTask.InitFlag = 0;
						/*下一刻的时间等于当前时间+定时时间*/
						task_call[i].NextTick = (Tick_Cnt+task_call[i].time_out);
						if(!task_call[i].IsSuspend){
							task_call[i].task_callBack(&OsTask);
						}
					}
				}
			}
		}
	}
}
