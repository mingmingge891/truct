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

//ϵͳ����ID
#define SYS_TASK_PRO  0

typedef void (*pTaskHandlerFn)(void* p);


/*���񼯺���Ϣ----------------------------------------------------------------*/
typedef struct
{
	uint8_t TaskPriority;//�������ȼ�
	
	uint32_t TaskPeriod;//��ʱ����
		
	pTaskHandlerFn task;//����
	
}Task_list_t;

typedef struct
{
	uint8_t InitFlag;//��ʼ����־
	uint32_t SysTimerCnt;//ʱ�ӽ���
	uint8_t  this_priority;//��ǰ����������ȼ�
	void (*SetTaskStart_Cb)(uint8_t,void(*)(void));//���񱻻ָ���ʱ��ص��������ú���
	void (*SetTaskStop_Cb)(uint8_t,void(*)(void));//���񱻹����ʱ��ص��������ú���
}Task_t;

typedef void (*task_callback)(void*);

typedef struct
{
	task_callback task_callBack;//����ص�����
	uint32_t time_out;//��ʱ
	uint32_t NextTick;//�¸�һ��ʱ�����
	uint8_t no_timer;//������Ҫ��ʱ��־λ
	uint8_t OS_Ready;//os ready ��־λ
	uint8_t InitFlag;//�״�ִ�б�־λ
	uint8_t IsSuspend;//�Ƿ��������
	void (*SuspendCb)(void);//�������ص�����
	void (*ResumeCb)(void);//����ָ��ص�����
}TaskOs_callBack_t;

/**
  * @brief  Task_OS_Create
  * @param  priority 0��� ptr:���� timer_period ʱ������ ΪNULL=����ٶ�ִ��������ʱ
  * @retval : 0 fail 1 succ
**/
uint8_t Timer_Task_OS_Create(uint8_t priority,void (*task)(void*),uint32_t timer_period);

/**
  * @brief  Task_OS_Init
  * @param  task_cnt ������� ���255��
  * @retval : 0 fail 1 success
**/
uint8_t Timer_Task_OS_Init(uint8_t task_cnt);

/**
  * @brief  ��ѵִ������ ���������ʱ
  * @param  None
  * @retval : None
**/
void Timer_Task_OS_Run(void);

/**
  * @brief  ��������ʱ����
  * @param  priority  timer_period
  * @retval : 0 fail 1 succ
**/
uint8_t Timer_Task_OS_UpdataPeriod(uint8_t priority,uint32_t timer_period);

/**
  * @brief  �������
  * @param  priority ���ȼ�
  * @retval : 
**/
void Timer_Task_OS_Suspend(uint8_t priority);

/**
  * @brief �ָ����������
  * @param  priority ���ȼ�
  * @retval : 
**/
void Timer_Task_OS_Resume(uint8_t priority);

/*������������*/
extern Task_list_t Task_list[];
extern u32 Tick_Cnt;
extern uint8_t TasksCnt;//�������
extern void Sys_Task(void* ptr);//ϵͳ����

#endif
