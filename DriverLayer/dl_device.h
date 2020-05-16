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

#ifndef __DL_DEVICE_H_
#define __DL_DEVICE_H_
#include "adapter.h"

/**
  * @brief  打开设备
  * @param  None
  * @retval : None
**/
int open_moudle_device(const char* device_name);

/**
  * @brief  关闭设备
  * @param  None
  * @retval : None
**/
void close_moudle_device(int fd);

/**
  * @brief  读函数
  * @param  None
  * @retval : None
**/
int read_moudle_device(int fd,void *data,u32 count);

/**
  * @brief  写函数
  * @param  None
  * @retval : None
**/
int write_moudle_device(int fd,void *data,u32 count);

/**
  * @brief  IO 控制函数
  * @param  None
  * @retval : None
**/
int ioctl_moudle_device(int fd,u8 CMD,void* arg);

/**
  * @brief  事件设置函数
  * @param  None
  * @retval : None
**/
int event_set_moudle_device(int fd,void(*callback)(void* d,u32 count));

/**
  * @brief  功能模块接口
  * @param  None
  * @retval : None
**/
typedef struct{
	/**
  * @brief  打开模块
  * @param  None
  * @retval : int
	**/
	int (*open_moudle)(void);
	/**
  * @brief  关闭模块
  * @param  None
  * @retval : None
	**/
	void (*close_moudle)(void);
	/**
  * @brief  读取数据
  * @param  None
  * @retval : None
	**/
	int (*read)(void* d,u32 count);
	/**
  * @brief  写入数据
  * @param  None
  * @retval : None
	**/
	int (*write)(void* d,u32 count);
	/**
  * @brief  IO控制
  * @param  None
  * @retval : None
	**/
	int (*ioctl)(u8 CMD,void* arg);
	
	/**
  * @brief  事件
  * @param  None
  * @retval : None
	**/
	int (*events)(void(*)(void* d,u32 count));
	
}FunctionalModuleInterface_t;

/**
  * @brief  功能模块设备
  * @param  None
  * @retval : None
**/
typedef struct{
	
	char* DeviceName;
	
	FunctionalModuleInterface_t *Implements;
	
}FunctionalModuleDevice_t;

/**
  * @brief  功能模块注册
  * @param  None
  * @retval : None
**/
typedef struct{
	
	FunctionalModuleDevice_t *p_ModuleDevice;
	
}FunctionalModuleRegistered_t;

/**
  * @brief  功能模块初始化
  * @param  None
  * @retval : None
**/
void FunctionalModuleLayer_Init(void);

#endif
