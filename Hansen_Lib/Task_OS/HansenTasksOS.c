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

TaskOs_callBack_t *task_call;//����ص�����ָ��
uint8_t Timer_tasksCnt;//ϵͳ�δ�ʱ�Ӽ�����
uint32_t Tick_Cnt = 0;//ϵͳ�ĵδ�ʱ��
Task_t OsTask;//����ص�ʱ�ϴ��Ĳ���

/**
  * @brief  �������ʱ��Ҫִ�еĻص�������ַ����
  * @param  this_priority ��ǰ�������ȼ�
  * @retval : 1 suc 0fail
**/
static void SetTaskStop_Cb(uint8_t this_priority ,void(*ptr)(void))
{
	
	task_call[this_priority].SuspendCb = ptr;
	
}
/**
  * @brief  ����ָ�ʱ��Ҫִ�еĻص�������ַ����
  * @param  this_priority ��ǰ�������ȼ�
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
  * @brief  �������
  * @param  priority ���ȼ�
  * @retval : None
**/
void Timer_Task_OS_Suspend(uint8_t priority)
{
	if(priority > (Timer_tasksCnt-1))
	{
		return;
	}
	
	/*ִ�е�ǰ�������ص��¼�*/
	
	if(task_call[priority].SuspendCb != NULL)
	{
		task_call[priority].SuspendCb();
	}
	
	task_call[priority].IsSuspend = 1;
}

/**
  * @brief �ָ����������
  * @param  priority ���ȼ�
  * @retval : None
**/
void Timer_Task_OS_Resume(uint8_t priority)
{
	if(priority > (Timer_tasksCnt-1))
	{
		return;
	}
	
	/*ִ�е�ǰ����ָ��ص��¼�*/
	
	if(task_call[priority].ResumeCb != NULL)
	{
		task_call[priority].ResumeCb();
	}
	
	task_call[priority].IsSuspend = 0;
}
/**
  * @brief  ��������ʱ����
  * @param  priority  timer_period
  * @retval : 0 fail 1 succ
**/
uint8_t Timer_Task_OS_UpdataPeriod(uint8_t priority,uint32_t timer_period)
{
	if(priority > (Timer_tasksCnt-1))
	{
			return 0;
	}
	/*timer_period = NULL ������ٶ�ִ��TASK ����ʱ,��Ƭ����ָ���ٶ�*/
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
  * @param  priority 0��� ,ptr:���� ,timer_period ʱ������ ΪNULL=����ٶ�ִ��������ʱ
  * @retval : 0 fail 1 succ
**/
uint8_t Timer_Task_OS_Create(uint8_t priority,void (*task)(void*),uint32_t timer_period)
{
	if(priority > (Timer_tasksCnt-1))
	{
			return 0;
	}
	/*timer_period = NULL ������ٶ�ִ��TASK ����ʱ,��Ƭ����ָ���ٶ�*/
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
  * @param  task_cnt ������� ���255��
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
	
	/*��ʼ��*/
	for(int i=0;i < Timer_tasksCnt;i++)
	{
		task_call[i].NextTick = 0;
		task_call[i].time_out = 0;
		task_call[i].task_callBack = NULL;
		task_call[i].no_timer = 0;
		task_call[i].OS_Ready = 0;
		task_call[i].InitFlag = 1;
		task_call[i].IsSuspend =0;//Ĭ�ϲ���������
	}
	
	/*��������ֹͣ����Ļص�������ַ���õĺ���ʵ����*/
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
  * @brief  ��ѵִ������ ���������ʱ
  * @param  None
  * @retval : None
**/
void Timer_Task_OS_Run(void)
{
	for(;;)
	{
		for(int i = 0; i < Timer_tasksCnt;i++)
		{
			/*����Ҫ��ʱ ֱ��������ʱ�ǵ�Ƭ����ָ����ʱ*/
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
				/*ʱ�䵽�� ���������*/
				if(( task_call[i].NextTick <= GetTimingTick() ) && ( task_call[i].OS_Ready ))
				{
					OsTask.SysTimerCnt = Tick_Cnt;
					OsTask.this_priority = i;//�õ���ǰ���ȼ���
					if(task_call[i].InitFlag == 1)
					{
						OsTask.InitFlag = 1;
						/*��һ�̵�ʱ����ڵ�ǰʱ��+��ʱʱ��*/
						task_call[i].NextTick = (Tick_Cnt+task_call[i].time_out);
						
						/*�жϵ�ǰ�����Ƿ����*/
						if(!task_call[i].IsSuspend){
							task_call[i].task_callBack(&OsTask);
						}
						task_call[i].InitFlag = 0;
					}
					else
					{
						OsTask.InitFlag = 0;
						/*��һ�̵�ʱ����ڵ�ǰʱ��+��ʱʱ��*/
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
