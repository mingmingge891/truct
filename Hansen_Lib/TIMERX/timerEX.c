/**
  ******************************************************************************
  * @file    timerEX.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    24-September-2018
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

/* Includes ------------------------------------------------------------------*/
#include "timerEX.h"
#include "string.h"

/* Values---------------------------------------------------------------------*/
/*HARDWARE_TIMERX_NUM: ֧�ֵ�Ӳ����ʱ���ĸ��� 2��0��������ʱ��1����Ƕ�׵Ķ�ʱ��
��TIME2_EVENT_NUM�������Ӷ�ʱ���ĸ���*/
timerEx_callback_t timerEx_call[HARDWARE_TIMERX_NUM][2][TIME_EVENT_NUM];

virtual_timerExInterface_t virtual_timerEximplements[HARDWARE_TIMERX_NUM];

virtual_nest_timerExInterface_t virtual_nest_timerEximplements[HARDWARE_TIMERX_NUM];

virtual_timerExInterface_t* virtual_timer0Handler = NULL;
virtual_timerExInterface_t* virtual_timer1Handler = NULL;

/*��ʱ���������ṹ�����*/
TIMER_PROP_T* pTimerProperty;

/*�ڲ�static ����*/
static void TIM0_CallBack(void);
static void TIM1_CallBack(void);

/* �ⲿ�ӿ� */
TIMER_OUTSIDE_DATA_CALLBACK_T* pData_callback_timerIrq;
TIMER_OUTSIDE_DATA_CALLBACK_T TiemrData_Interface = 
{
	.HS_Timer0_DataCallback = TIM0_CallBack,
	.HS_Timer1_DataCallback = TIM1_CallBack,
};

/* Funtion---------------------------------------------------------------------*/
static virtual_nest_timerExInterface_t* osal_create_nestTimerEx(uint8_t h_id,uint8_t parents_id,uint8_t timerExCnt);

/**
  * @brief  This function
  * @param  None
  * @retval : 0 fail 1 succ
**/
static uint8_t osal_start_timerEx(uint8_t h_id,uint8_t timerEx_id,void* p_arg,void (*ptr)(void*),uint32_t timer_period)
{
	if(timerEx_id > (TIME_EVENT_NUM-1))
	{
		return 0;
	}
	if(h_id == 0)
	{
		pTimerProperty->Timer_CMD(0,1);
	}
	if(h_id == 1)
	{
		pTimerProperty->Timer_CMD(1,1);
	}
	timerEx_call[h_id][0][timerEx_id].t_callBack = ptr;
	timerEx_call[h_id][0][timerEx_id].time_out = timer_period;
	timerEx_call[h_id][0][timerEx_id].time_count = 0;
	timerEx_call[h_id][0][timerEx_id].prg = p_arg;
	return 1;
}

/**
  * @brief  This function
  * @param  None
  * @retval : 0 fail 1 succ
**/
static uint8_t nest_start_timerEx(uint8_t h_id,uint8_t timerEx_id,void* p_arg,void (*ptr)(void*),uint32_t timer_period)
{
	if(timerEx_id > (TIME_EVENT_NUM-1))
	{
		return 0;
	}
	if(h_id == 0)
	{
		pTimerProperty->Timer_CMD(0,1);
	}
	if(h_id == 1)
	{
		pTimerProperty->Timer_CMD(1,1);
	}
	timerEx_call[h_id][1][timerEx_id].t_callBack = ptr;
	timerEx_call[h_id][1][timerEx_id].time_out = timer_period;
	timerEx_call[h_id][1][timerEx_id].time_count = 0;
	timerEx_call[h_id][1][timerEx_id].prg = p_arg;
	return 1;
}


/**
  * @brief  This function
  * @param  None
  * @retval : 0 fail 1 succ
**/
static uint8_t osal_stop_timerEx(uint8_t h_id,uint8_t timerEx_id)
{
	if(timerEx_id > (TIME_EVENT_NUM-1))
	{
			return 0;
	}
	timerEx_call[h_id][0][timerEx_id].time_out = 0;
	timerEx_call[h_id][0][timerEx_id].time_count = 0;
	return 1;
}

/**
  * @brief  This function
  * @param  None
  * @retval : 0 fail 1 succ
**/
static uint8_t nest_stop_timerEx(uint8_t h_id,uint8_t timerEx_id)
{
	if(timerEx_id > (TIME_EVENT_NUM-1))
	{
			return 0;
	}
	timerEx_call[h_id][1][timerEx_id].time_out = 0;
	timerEx_call[h_id][1][timerEx_id].time_count = 0;
	return 1;
}
/**
  * @brief  ѭ��ִ��
  * @param  id Ӳ����ʱ�ı��
  * @retval : None
**/
void virtualTimerRun(uint8_t id)
{
	/*��ʱ������*/
	for(uint8_t i = 0; i < TIME_EVENT_NUM;i++)
	{
		if(timerEx_call[id][0][i].time_count == 0xFFFFFFFFul)
			timerEx_call[id][0][i].time_count = 0;
			
		if(++timerEx_call[id][0][i].time_count == timerEx_call[id][0][i].time_out)
		{
			timerEx_call[id][0][i].time_count = 0;
			
			/*һ��Ƕ��*/
			if( timerEx_call[id][0][i].nest_timerCnt >0 ){
				
				for(uint8_t j = 0;j < timerEx_call[id][0][i].nest_timerCnt; j++)
				{
					if(timerEx_call[id][1][j].time_count == 0xFFFFFFFFul)
						timerEx_call[id][1][j].time_count = 0;
					
					if(++timerEx_call[id][1][j].time_count == timerEx_call[id][1][j].time_out){
						timerEx_call[id][1][j].time_count = 0;
						timerEx_call[id][1][j].t_callBack(timerEx_call[id][1][j].prg);
					}
				}
			}
			else{
				timerEx_call[id][0][i].t_callBack(timerEx_call[id][0][i].prg);
			}
		}
	}
}

/**
  * @brief  ʵ����
  * @param  None
  * @retval : None
**/
void new_timerEX(virtual_timerExInterface_t* p)
{
	p->create_nestTimerEx = osal_create_nestTimerEx;
	
	p->start_timerEx = osal_start_timerEx;
	
	p->stop_timerEx = osal_stop_timerEx;
}

/**
  * @brief  Ƕ�׶�ʱ��ʵ����
  * @param  None
  * @retval : None
**/
void new_nest_timerEX(virtual_nest_timerExInterface_t* p)
{	
	
	p->start_timerEx = nest_start_timerEx;
	
	p->stop_timerEx = nest_stop_timerEx;
	
}

/**
  * @brief ����Ƕ�׶�ʱ��
  * @param  parents_id����ID
	timerExCnt: ����
**/
static virtual_nest_timerExInterface_t* osal_create_nestTimerEx(uint8_t h_id,uint8_t parents_id,uint8_t timerExCnt)
{
	if(parents_id > (TIME_EVENT_NUM-1))
	{
		return NULL;
	}
	
	if(timerExCnt > (TIME_EVENT_NUM-1))
	{
		return NULL;
	}
	
	/*Ƕ�׵ĸ���*/
	timerEx_call[h_id][0][parents_id].nest_timerCnt = timerExCnt;
	
	return &virtual_nest_timerEximplements[h_id];
	
}

/**
  * @brief ����һ����ʱ��
  * @param  hardwareTimerEx_ID Ӳ����ʱ�����
  * @retval : virtual_timerExInterface_t �ṹ��
**/
virtual_timerExInterface_t* create_new_virtual_timerEX(uint8_t hardwareTimerEx_ID)
{
	if(hardwareTimerEx_ID > 2)
	{
		return NULL;
	}
	/*ʵ������ʱ��*/
	new_timerEX(&virtual_timerEximplements[hardwareTimerEx_ID]);
	
	/*Ƕ�׶�ʱ��ʵ����*/
	new_nest_timerEX(&virtual_nest_timerEximplements[hardwareTimerEx_ID]);
	
	/*�õ�Ӳ��ID*/
	virtual_timerEximplements[hardwareTimerEx_ID].this_h_id = hardwareTimerEx_ID;
	
	return &virtual_timerEximplements[hardwareTimerEx_ID];
}

/**
  * @brief  This function 1ms �жϺ�������
  * @param  None
  * @retval : None
**/
static void TIM0_CallBack(void)
{
	virtualTimerRun(0);
}
/**
  * @brief  This function 1ms �жϺ�������
  * @param  None
  * @retval : None
**/
static void TIM1_CallBack(void)
{
	virtualTimerRun(1);
}

/**
  * @brief  This function ��ʼ��ϵͳ��ʱ��
  * @param 	void
  * @retval : None
**/
void init_timerEX(void)
{
	/*�õ�����*/
	pTimerProperty = &Implement_Timer_Property;
	
	/*Ӳ����ʱ���ж����ݻص��ӿ�*/
	pData_callback_timerIrq = &TiemrData_Interface;
	
	/*��ʼ��*/
	pTimerProperty->Init();
	
	/*ʵ����*/
	virtual_timer0Handler = create_new_virtual_timerEX(0);
	virtual_timer1Handler = create_new_virtual_timerEX(1);
	
}

