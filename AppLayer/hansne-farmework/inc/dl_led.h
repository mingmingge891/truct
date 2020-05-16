/**
  ******************************************************************************
  * @file    dl_led.h
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
#ifndef __DL_LED_H_
#define __DL_LED_H_


#define INFINITE   0


/*定义LED操作必要的抽象函数接口或者参数，
实现需要用户在用户层实现*/
typedef struct
{
	void (*Init)(void);       							          /*! Initialize the LED */
	__IO uint16_t *r_value;
	__IO uint16_t *g_value;
	__IO uint16_t *b_value;
}LED_Interface_t;

/*LED操作API
*/
typedef struct
{
	//CtlInterface 为控制的接口需要实现 参数返回TRUE,FALSE,CtlCallback为控制的回调函数可以为NULL，脉冲的时间间隔最小为1ms
	//positive_time 为正 negative_time 为互 也可以理解为高低电平negative_time可以为0 就代表只执行positive_time 但positive_time
	//不能为0 num为执行的次数
	void (*Pulse1Start)(void (*CtlInterface)(bool,void*),void*,u16 positive_time,u16 negative_time,u16 num);
	void (*Pulse2Start)(void (*CtlInterface)(bool,void*),void*,u16 positive_time,u16 negative_time,u16 num);
	void (*Pulse3Start)(void (*CtlInterface)(bool,void*),void*,u16 positive_time,u16 negative_time,u16 num);
	void (*Pulse4Start)(void (*CtlInterface)(bool,void*),void*,u16 positive_time,u16 negative_time,u16 num);
	void (*Pulse5Start)(void (*CtlInterface)(bool,void*),void*,u16 positive_time,u16 negative_time,u16 num);
	void (*Pulse1Stop)(void);
	void (*Pulse2Stop)(void);
	void (*Pulse3Stop)(void);
	void (*Pulse4Stop)(void);
	void (*Pulse5Stop)(void);
	//RGB 颜色设置 isBreathing 表示是否呼吸以及呼吸的速度第8未为选择，底7位为速度
	//B_Level 为亮度 Par_BreathingCount为呼吸的次数255为一直呼吸 L高8位为熄灭的时
	//间B底8位为亮的时间，特殊只说明  呼吸速度峰值来的时间如，0x88,0x7F ->1916ms 
	//25s 要13次呼吸，返回值本次呼吸用的时间 常亮返回0
	uint32_t (*rgb_setValue10)(u8 r,u8 g,u8 b,uint8_t isBreathing,uint8_t B_Level,
	uint8_t Par_BreathingCount,uint32_t L_B);
	uint32_t (*rgb_setValue)(uint32_t rgbValue,uint8_t isBreathing,uint8_t B_Level,uint8_t Par_BreathingCount,uint32_t L_B);
}led_t;
extern led_t led;
extern LED_Interface_t LED_Interface;
void init_sysled(void);

#endif
