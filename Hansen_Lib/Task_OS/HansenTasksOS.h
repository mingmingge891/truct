/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2018 </center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#ifndef __TASK_OS_H_
#define __TASK_OS_H_

//系统任务ID
#define SYS_TASK_PRO  0

typedef void (*pTaskHandlerFn)(void* p);


/*任务集合信息----------------------------------------------------------------*/
typedef struct
{
	uint8_t TaskPriority;//任务优先级
	
	uint32_t TaskPeriod;//定时周期
		
	pTaskHandlerFn task;//任务
	
}Task_list_t;

typedef struct
{
	uint8_t InitFlag;//初始化标志
	uint32_t SysTimerCnt;//时钟节拍
	uint8_t  this_priority;//当前的任务的优先级
	void (*SetTaskStart_Cb)(uint8_t,void(*)(void));//任务被恢复的时候回调函数设置函数
	void (*SetTaskStop_Cb)(uint8_t,void(*)(void));//任务被挂起的时候回调函数设置函数
}Task_t;

typedef void (*task_callback)(void*);

typedef struct
{
	task_callback task_callBack;//任务回调函数
	uint32_t time_out;//超时
	uint32_t NextTick;//下个一个时间节拍
	uint8_t no_timer;//任务不需要延时标志位
	uint8_t OS_Ready;//os ready 标志位
	uint8_t InitFlag;//首次执行标志位
	uint8_t IsSuspend;//是否将任务挂起
	void (*SuspendCb)(void);//任务挂起回调函数
	void (*ResumeCb)(void);//任务恢复回调函数
}TaskOs_callBack_t;

/**
  * @brief  Task_OS_Create
  * @param  priority 0最高 ptr:任务 timer_period 时间周期 为NULL=最快速度执行任务不延时
  * @retval : 0 fail 1 succ
**/
uint8_t Timer_Task_OS_Create(uint8_t priority,void (*task)(void*),uint32_t timer_period);

/**
  * @brief  Task_OS_Init
  * @param  task_cnt 任务个数 最大255个
  * @retval : 0 fail 1 success
**/
uint8_t Timer_Task_OS_Init(uint8_t task_cnt);

/**
  * @brief  轮训执行任务 任务可以延时
  * @param  None
  * @retval : None
**/
void Timer_Task_OS_Run(void);

/**
  * @brief  更改任务定时周期
  * @param  priority  timer_period
  * @retval : 0 fail 1 succ
**/
uint8_t Timer_Task_OS_UpdataPeriod(uint8_t priority,uint32_t timer_period);

/**
  * @brief  任务挂起
  * @param  priority 优先级
  * @retval : 
**/
void Timer_Task_OS_Suspend(uint8_t priority);

/**
  * @brief 恢复挂起的任务
  * @param  priority 优先级
  * @retval : 
**/
void Timer_Task_OS_Resume(uint8_t priority);

/*创建任务数据*/
extern Task_list_t Task_list[];
extern u32 Tick_Cnt;
extern uint8_t TasksCnt;//任务个数
extern void Sys_Task(void* ptr);//系统任务

#endif
