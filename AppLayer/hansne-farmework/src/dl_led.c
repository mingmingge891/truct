/**
  ******************************************************************************
  * @file    dl_key.C
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    8-3-2020
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
#include "dl_system.h"
#include "dl_led.h"
#include "dl_delay.h"
#include "dl_time.h"
#include "dl_serial.h"
#include "stdarg.h"
#include <stdio.h>

//定时器
timer_t *ledTime = NULL;
#define  RGB_TIME_EVENT                              0/**/
#define  LED_PULSE1_TIME_EVENT                       1/**/
#define  LED_PULSE2_TIME_EVENT                       2/**/
#define  LED_PULSE3_TIME_EVENT                       3/**/
#define  LED_PULSE4_TIME_EVENT                       4/**/
#define  LED_PULSE5_TIME_EVENT                       5/**/

/*LED抽象函数结构体变量*/
LED_Interface_t* pLED_Implements;
//三基色颜色分量
uint8_t red8,green8,blue8;
//呼吸速度单位ms
uint8_t BreathingRate = 5;
//呼吸亮度峰值默认255这个值配合呼吸速度可以改变呼吸速度
uint8_t BreathingPeakLevel = 255;
//呼吸灯呼吸次数
uint8_t BreathingCount =0;
//呼吸灯暗的时间
uint16_t BreathingBTime =0;
//呼吸灯亮的时间
uint16_t BreathingLTime =0;
u8 IsAllWalawaysBreak = 0;

typedef struct{
	u8 PulseFlag;
	u16 negative_time;
	u16 positive_time;
	u8 timer_event;
	u32 num;
	bool state;
	bool gorun;
	bool InfiniteFlag;
	void (*Interface)(bool,void *);
	void *valuePtr;
}led_ctl_t;
led_ctl_t led_ctl[5];

//脉冲操作
static void PulseOperate(void* p)
{
	led_ctl_t *pLed = (led_ctl_t*)p;
	
	if(pLed->InfiniteFlag == TRUE){
		pLed->gorun = TRUE;
	}
	if(pLed->gorun == TRUE){
		if(pLed->Interface != NULL)
		{
			pLed->Interface(pLed->PulseFlag > 0 ? FALSE:TRUE,pLed->valuePtr);
			pLed->PulseFlag = !pLed->PulseFlag;
			if(pLed->state == FALSE){
				ledTime->start(ledTime->instance,p,PulseOperate,pLed->negative_time,pLed->timer_event);
				pLed->state = TRUE;
				pLed->num--;
				if(pLed->num == 0){
					pLed->gorun = TRUE;
					ledTime->stop(ledTime->instance,pLed->timer_event);
					return;
				}
			}
			else{
				ledTime->start(ledTime->instance,p,PulseOperate,pLed->positive_time,pLed->timer_event);
				pLed->state = FALSE;
			}
		}
	}
	else{
		pLed->gorun = TRUE;
		pLed->Interface(FALSE,pLed->valuePtr);
		ledTime->stop(ledTime->instance,pLed->timer_event);
	}
}

//脉冲设置
static void LED_PulseStart(u8 index,u8 TimerEvent,void (*CtlInterface)(bool,void*),void * v,u16 positive_time,u16 negative_time,u16 num)
{
	//初始化变量
	led_ctl[index].gorun = TRUE;
	led_ctl[index].state = TRUE;
	//得到控制接口
	led_ctl[index].Interface = CtlInterface;
	//反转时间
	led_ctl[index].negative_time = negative_time;
	//得到正向时间
	led_ctl[index].positive_time = positive_time;
	//得到定时器事件
	led_ctl[index].timer_event = TimerEvent;
	//得到值回调
	led_ctl[index].valuePtr = v;
	//得到执行的次数
	led_ctl[index].num = num;
	led_ctl[index].InfiniteFlag = (num >0 ? TRUE : FALSE);
	//启动一个定时器
	ledTime->start(ledTime->instance,&led_ctl[index],PulseOperate,led_ctl[index].positive_time,TimerEvent);
}
//脉冲启动
static void LED_Pulse1Start(void (*CtlInterface)(bool,void* v),void* ptr,u16 positive_time,u16 negative_time,u16 num)
{LED_PulseStart(0,LED_PULSE1_TIME_EVENT,CtlInterface,ptr,positive_time,negative_time,num);
}
static void LED_Pulse2Start(void (*CtlInterface)(bool,void* v),void* ptr,u16 positive_time,u16 negative_time,u16 num)
{LED_PulseStart(1,LED_PULSE2_TIME_EVENT,CtlInterface,ptr,positive_time,negative_time,num);
}
static void LED_Pulse3Start(void (*CtlInterface)(bool,void* v),void* ptr,u16 positive_time,u16 negative_time,u16 num)
{LED_PulseStart(2,LED_PULSE3_TIME_EVENT,CtlInterface,ptr,positive_time,negative_time,num);
}
static void LED_Pulse4Start(void (*CtlInterface)(bool,void* v),void* ptr,u16 positive_time,u16 negative_time,u16 num)
{LED_PulseStart(3,LED_PULSE4_TIME_EVENT,CtlInterface,ptr,positive_time,negative_time,num);
}
static void LED_Pulse5Start(void (*CtlInterface)(bool,void* v),void* ptr,u16 positive_time,u16 negative_time,u16 num)
{LED_PulseStart(4,LED_PULSE5_TIME_EVENT,CtlInterface,ptr,positive_time,negative_time,num);
}
//脉冲停止
static void LED_Pulse1Stop(void)
{
	if(led_ctl[0].Interface !=NULL)
		led_ctl[0].Interface(FALSE,NULL);
	ledTime->stop(ledTime->instance,LED_PULSE1_TIME_EVENT);
}
static void LED_Pulse2Stop(void)
{
	if(led_ctl[1].Interface !=NULL)
		led_ctl[1].Interface(FALSE,NULL);
	ledTime->stop(ledTime->instance,LED_PULSE2_TIME_EVENT);
	
}
static void LED_Pulse3Stop(void)
{
	if(led_ctl[2].Interface !=NULL)
		led_ctl[2].Interface(FALSE,NULL);
	ledTime->stop(ledTime->instance,LED_PULSE3_TIME_EVENT);
	
}
static void LED_Pulse4Stop(void)
{
	if(led_ctl[3].Interface !=NULL)
		led_ctl[3].Interface(FALSE,NULL);
	ledTime->stop(ledTime->instance,LED_PULSE4_TIME_EVENT);
	
}
static void LED_Pulse5Stop(void)
{
	if(led_ctl[4].Interface !=NULL)
		led_ctl[4].Interface(FALSE,NULL);
	ledTime->stop(ledTime->instance,LED_PULSE5_TIME_EVENT);
	
}

/**
  * @brief  This function rgb led呼吸灯
  * @param  None
  * @retval : None
**/
static void BreathingHandler(void* p)
{
		static uint16_t B_Level = 0;
		static uint8_t Br_Flag = 1;
		static uint8_t EffectFlag = 0;
		if(EffectFlag)
		{
			EffectFlag = 0;
			ledTime->start(ledTime->instance,NULL,BreathingHandler,BreathingRate,RGB_TIME_EVENT);//恢复到正常延时模式
		}
		if(Br_Flag)B_Level++;
		else B_Level--;
		if(B_Level > BreathingPeakLevel)
		{
			 Br_Flag = 0;
			 ledTime->start(ledTime->instance,NULL,BreathingHandler,BreathingLTime,RGB_TIME_EVENT);//高亮的时间500ms固定值
			 EffectFlag = 1;
		}
		if(B_Level == 0)
		{
			Br_Flag = 1;
			#if 0
				if(BreathingOutTime > 0){
						ledTime->start(ledTime->instance,NULL,BreathingHandler,BreathingOutTime*100,RGB_TIME_EVENT);//暗亮的时间手动设置
				}
			#else
				if(BreathingCount > 0){
						ledTime->start(ledTime->instance,NULL,BreathingHandler,BreathingBTime,RGB_TIME_EVENT);//暗亮的时间固定时间
				}
			#endif
			EffectFlag = 1;
			if(IsAllWalawaysBreak == 0)//是否一直呼吸
			{
				if(BreathingCount == 0)
				{
					/*停止呼吸灯*/
					ledTime->stop(ledTime->instance,RGB_TIME_EVENT);
				}
				else
				{
						BreathingCount--;
				}
			}
		}
		*(u16*)pLED_Implements->r_value = (900*B_Level/255)*(red8)>>8;
		*(u16*)pLED_Implements->g_value = (900*B_Level/255)*(green8)>>8;
		*(u16*)pLED_Implements->b_value = (900*B_Level/255)*(blue8)>>8;
}
/**
  * @brief  This function rgb rgb 灯亮度设置
  * @param  None
  * @retval : None
**/
void rgb_setBrightLevel(uint8_t B_Level)
{
		*(u16*)pLED_Implements->r_value = (900*B_Level/255)*(red8)>>8;
		*(u16*)pLED_Implements->g_value = (900*B_Level/255)*(green8)>>8;
		*(u16*)pLED_Implements->b_value = (900*B_Level/255)*(blue8)>>8;	
}

/**
  * @brief  This function rgb 灯颜色设置
	*特殊只说明  呼吸速度峰值来的时间如
	*0x88,0x7F ->1916ms  25s 要13次呼吸
  * @param  None
  * @retval : 返回本次呼吸用的时间 常亮返回0
**/
static uint32_t rgb_setValue(uint32_t rgbValue,uint8_t isBreathing,uint8_t B_Level,
	uint8_t Par_BreathingCount,uint32_t L_B) 
{
		if(Par_BreathingCount == 255)
		{
			IsAllWalawaysBreak = 1;
		}
		/*得到颜色分量*/
		red8 		= rgbValue>>16;
		green8 	= rgbValue>>8;
		blue8 	= rgbValue;
		if(isBreathing&0x80)
		{
				/*得到到呼吸速度以及亮度峰值和呼吸次数*/
				BreathingRate = isBreathing&0x7F;
				BreathingPeakLevel = B_Level;
				#if 0
					BreathingCount = (Par_BreathingCount&0x3F) - 1;
					/*得到呼吸黑暗的时间,1、2、3s*/
					BreathingBTime = (Par_BreathingCount&0xC0)>>6;
				#else
					if(Par_BreathingCount > 0){
						BreathingCount = Par_BreathingCount - 1;
					}
					/*亮在高位，暗的低位 0xFFFF0000*/
					BreathingLTime = L_B>>16;
					BreathingBTime = L_B&0xFFFF;
				#endif
				ledTime->start(ledTime->instance,NULL,BreathingHandler,BreathingRate,RGB_TIME_EVENT);
				return	Par_BreathingCount*(BreathingLTime+BreathingBTime+(2*BreathingRate*BreathingPeakLevel));
		}
		/*非呼吸灯亮度赋值到pwm上*/
		rgb_setBrightLevel(B_Level);
		//BreathingCount = 0;
		ledTime->stop(ledTime->instance,RGB_TIME_EVENT);
		return 0;
}
/**
  * @brief  This function rgb 灯颜色设置三个分量单独设置
	*特殊只说明  呼吸速度峰值来的时间
	*0x88,0x7F ->1916ms  25s 要13次呼吸
  * @param  None
  * @retval : 返回本次呼吸用的时间 常亮返回0
**/
static uint32_t rgb_setValue10(u8 r,u8 g,u8 b,uint8_t isBreathing,uint8_t B_Level,
	uint8_t Par_BreathingCount,uint32_t L_B)
{
	u32 rgb_value = (r<<16) + (g<<8) + b;
	return rgb_setValue(rgb_value,isBreathing,B_Level,Par_BreathingCount,L_B);
}

//API接口
led_t led = 
{
	.Pulse1Start 				= LED_Pulse1Start,
	.Pulse2Start 				= LED_Pulse2Start,
	.Pulse3Start 				= LED_Pulse3Start,
	.Pulse4Start 				= LED_Pulse4Start,
	.Pulse5Start 				= LED_Pulse5Start,
	.Pulse1Stop  				= LED_Pulse1Stop,
	.Pulse2Stop  				= LED_Pulse2Stop,
	.Pulse3Stop  				= LED_Pulse3Stop,
	.Pulse4Stop  				= LED_Pulse4Stop,
	.Pulse5Stop  				= LED_Pulse5Stop,
	.rgb_setValue10 		= rgb_setValue10,
	.rgb_setValue 			= rgb_setValue,
};
/**
  * @brief  初始化系统LED
  * @param 	void
  * @retval : None
**/
void init_sysled(void)
{
	ledTime = timerNew(6);
	/*得到对象*/
	pLED_Implements = &LED_Interface;
	pLED_Implements->Init();
}

