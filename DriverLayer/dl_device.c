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
#include "dl_device.h"
#include "string.h"

/* extern Values -------------------------------------------------------------*/

extern FunctionalModuleDevice_t MobileNet_Device;

/* 功能模块注册加载 静态模式 */
FunctionalModuleRegistered_t FunctionalModuleRegisterArray[] = 
{
	&MobileNet_Device,
};

/* 设备个数 */
const uint8_t DeviceCnt = sizeof( FunctionalModuleRegisterArray ) / sizeof( FunctionalModuleRegisterArray[0] );

/**
  * @brief  打开设备
  * @param  None
  * @retval : None
**/
int open_moudle_device(const char* device_name)
{
	
	for(int i = 0;i < DeviceCnt;i++)
	{
		//对比两个字符串是否相等
		if(strcmp(FunctionalModuleRegisterArray[i].p_ModuleDevice->DeviceName,
			device_name) == 0)
		{	
			FunctionalModuleRegisterArray[i].p_ModuleDevice->Implements->open_moudle();
			return i;
		}
	}
	
	return -1;
}

/**
  * @brief  关闭设备
  * @param  None
  * @retval : None
**/
void close_moudle_device(int fd)
{
	FunctionalModuleRegisterArray[fd].p_ModuleDevice->Implements->close_moudle();
}

/**
  * @brief  读函数
  * @param  None
  * @retval : None
**/
int read_moudle_device(int fd,void *data,u32 count)
{
	if(fd <0)
		return -1;
	
	return FunctionalModuleRegisterArray[fd].p_ModuleDevice->Implements->read(data,count);
}

/**
  * @brief  写函数
  * @param  None
  * @retval : None
**/
int write_moudle_device(int fd,void *data,u32 count)
{
	if(fd <0)
		return -1;
	
	return FunctionalModuleRegisterArray[fd].p_ModuleDevice->Implements->write(data,count);
}

/**
  * @brief  IO 控制函数
  * @param  None
  * @retval : None
**/
int ioctl_moudle_device(int fd,u8 CMD,void* arg)
{
	return FunctionalModuleRegisterArray[fd].p_ModuleDevice->Implements->ioctl(CMD,arg);
}

/**
  * @brief  事件设置函数
  * @param  None
  * @retval : None
**/
int event_set_moudle_device(int fd,void(*callback)(void* d,u32 count))
{
	return FunctionalModuleRegisterArray[fd].p_ModuleDevice->Implements->events(callback);
}

/**
  * @brief  初始化
  * @param  None
  * @retval : None
**/
void FunctionalModuleLayer_Init(void)
{
	int tmpDeviceCnt = DeviceCnt;
	int indexech = 0;
	char*  LastName = NULL;
	
	/* 依次对比设备的名字,设备的名字不能相同 */
	while(tmpDeviceCnt--)
	{
		LastName = FunctionalModuleRegisterArray[indexech].p_ModuleDevice->DeviceName;
		for(int i = 0 ;i < DeviceCnt;i++)
		{
			if(indexech  == i)
			{
				continue;
			}
			if(strcmp(FunctionalModuleRegisterArray[i].p_ModuleDevice->DeviceName,LastName) == 0){
				while(1){
					//err  设备的名字重复了
				}
			}
		}
		indexech++;
	}
	
}
