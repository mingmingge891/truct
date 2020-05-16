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
  * @brief  ���豸
  * @param  None
  * @retval : None
**/
int open_moudle_device(const char* device_name);

/**
  * @brief  �ر��豸
  * @param  None
  * @retval : None
**/
void close_moudle_device(int fd);

/**
  * @brief  ������
  * @param  None
  * @retval : None
**/
int read_moudle_device(int fd,void *data,u32 count);

/**
  * @brief  д����
  * @param  None
  * @retval : None
**/
int write_moudle_device(int fd,void *data,u32 count);

/**
  * @brief  IO ���ƺ���
  * @param  None
  * @retval : None
**/
int ioctl_moudle_device(int fd,u8 CMD,void* arg);

/**
  * @brief  �¼����ú���
  * @param  None
  * @retval : None
**/
int event_set_moudle_device(int fd,void(*callback)(void* d,u32 count));

/**
  * @brief  ����ģ��ӿ�
  * @param  None
  * @retval : None
**/
typedef struct{
	/**
  * @brief  ��ģ��
  * @param  None
  * @retval : int
	**/
	int (*open_moudle)(void);
	/**
  * @brief  �ر�ģ��
  * @param  None
  * @retval : None
	**/
	void (*close_moudle)(void);
	/**
  * @brief  ��ȡ����
  * @param  None
  * @retval : None
	**/
	int (*read)(void* d,u32 count);
	/**
  * @brief  д������
  * @param  None
  * @retval : None
	**/
	int (*write)(void* d,u32 count);
	/**
  * @brief  IO����
  * @param  None
  * @retval : None
	**/
	int (*ioctl)(u8 CMD,void* arg);
	
	/**
  * @brief  �¼�
  * @param  None
  * @retval : None
	**/
	int (*events)(void(*)(void* d,u32 count));
	
}FunctionalModuleInterface_t;

/**
  * @brief  ����ģ���豸
  * @param  None
  * @retval : None
**/
typedef struct{
	
	char* DeviceName;
	
	FunctionalModuleInterface_t *Implements;
	
}FunctionalModuleDevice_t;

/**
  * @brief  ����ģ��ע��
  * @param  None
  * @retval : None
**/
typedef struct{
	
	FunctionalModuleDevice_t *p_ModuleDevice;
	
}FunctionalModuleRegistered_t;

/**
  * @brief  ����ģ���ʼ��
  * @param  None
  * @retval : None
**/
void FunctionalModuleLayer_Init(void);

#endif
