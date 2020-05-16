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

/* ����ģ��ע����� ��̬ģʽ */
FunctionalModuleRegistered_t FunctionalModuleRegisterArray[] = 
{
	&MobileNet_Device,
};

/* �豸���� */
const uint8_t DeviceCnt = sizeof( FunctionalModuleRegisterArray ) / sizeof( FunctionalModuleRegisterArray[0] );

/**
  * @brief  ���豸
  * @param  None
  * @retval : None
**/
int open_moudle_device(const char* device_name)
{
	
	for(int i = 0;i < DeviceCnt;i++)
	{
		//�Ա������ַ����Ƿ����
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
  * @brief  �ر��豸
  * @param  None
  * @retval : None
**/
void close_moudle_device(int fd)
{
	FunctionalModuleRegisterArray[fd].p_ModuleDevice->Implements->close_moudle();
}

/**
  * @brief  ������
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
  * @brief  д����
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
  * @brief  IO ���ƺ���
  * @param  None
  * @retval : None
**/
int ioctl_moudle_device(int fd,u8 CMD,void* arg)
{
	return FunctionalModuleRegisterArray[fd].p_ModuleDevice->Implements->ioctl(CMD,arg);
}

/**
  * @brief  �¼����ú���
  * @param  None
  * @retval : None
**/
int event_set_moudle_device(int fd,void(*callback)(void* d,u32 count))
{
	return FunctionalModuleRegisterArray[fd].p_ModuleDevice->Implements->events(callback);
}

/**
  * @brief  ��ʼ��
  * @param  None
  * @retval : None
**/
void FunctionalModuleLayer_Init(void)
{
	int tmpDeviceCnt = DeviceCnt;
	int indexech = 0;
	char*  LastName = NULL;
	
	/* ���ζԱ��豸������,�豸�����ֲ�����ͬ */
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
					//err  �豸�������ظ���
				}
			}
		}
		indexech++;
	}
	
}
