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
#include "dl_system.h"
#include "dl_time.h"
#include "string.h"

timer_t* time1;
timer_t* time2;
timer_t* time3;
static void as(void* p)
{
	int a = 0;
	a = 4*5;
}
static void as2(void* p)
{
	int a = 0;
	a = 4*5;
	//time3->stop(time3->instance,4);
}

static void as1(void* p)
{
	int a = 0;
	a = 4*5;
	time1->stop(time1->instance);
}

/**
  * @brief  主函数入口
  * @param  None
  * @retval : None
**/
int main(void)
{  
	
	/*系统初始化*/
	SystemInitialize();
	

	//每个定时器有8个子定时器 0-7
	time1 = timerNew(1);
	time2 = timerNew(1);
	time3 = sys_timerNew(1);
	
	time1->start(time1->instance,NULL,as1,500);
	time2->start(time2->instance,NULL,as,1000);
	time3->start(time3->instance,NULL,as2,400);
	 
	while(1);
	
	
}
