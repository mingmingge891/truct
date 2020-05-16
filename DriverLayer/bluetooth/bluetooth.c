/**
  ******************************************************************************
  * @file    bluetooth.c
  * @author  HANSEN     emil:951868692@qq.com
  * @version V1.2.0
  * @date    23-05-2019
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
#include "bluetooth.h"
#include "string.h"

static uint8_t BluetoothSetName(const char* str);
static char* BluetoothSearchSlaveDevice(void);
static uint8_t BluetoothConnect(int8_t* mac);
static void BluetoothDisconnect(void);
static uint8_t BluetoothDB_Set(int8_t TPL);
static void BluetoothWorkModeSet(WorkModeType Mode);
static char* BluetoothGetMac(void);
static void BluetoothSetBroadcastSpeed(uint8_t Speed);
static void BluetoothSetCallback(void (*ptr)(uint8_t* D,uint16_t L));
static void BluetoothSendString(int8* str);
static void BluetoothSendHex(uint8_t* D ,uint16_t Len);
static void BluetoothReboot(void);
/* Values --------------------------------------------------------------------*/

BluetoothDevice_t BluetoothDevice = {
	.Open = BluetoothOpen,
	.Close = BluetoothClose,
};
Bluetooth_t Bluetooth[SUPPORT_DEVICE_NUM] = 
{
	
	{
		.SetName = BluetoothSetName,
		.SearchSlaveDevice = BluetoothSearchSlaveDevice,
		.Connect = BluetoothConnect,
		.Disconnect = BluetoothDisconnect,
		.DB_Set = BluetoothDB_Set,
		.WorkModeSet = BluetoothWorkModeSet,
		.GetMac = BluetoothGetMac,
		.SetBroadcastSpeed = BluetoothSetBroadcastSpeed,
		.SetCallback = BluetoothSetCallback,
		.HexTranmission =BluetoothSendHex,
		.StringTranmission = BluetoothSendString,
		.Reboot             =BluetoothReboot,
	},
	{
		.SetName = BluetoothSetName,
		.SearchSlaveDevice = BluetoothSearchSlaveDevice,
		.Connect = BluetoothConnect,
		.Disconnect = BluetoothDisconnect,
		.DB_Set = BluetoothDB_Set,
		.WorkModeSet = BluetoothWorkModeSet,
		.GetMac = BluetoothGetMac,
		.SetBroadcastSpeed = BluetoothSetBroadcastSpeed,
		.SetCallback = BluetoothSetCallback,
		.HexTranmission =BluetoothSendHex,
		.StringTranmission = BluetoothSendString,
		.Reboot             =BluetoothReboot,
	},
};

BluetoothInterface_t *pBluetoothInterface = NULL;
/**
  * @brief  BluetoothSetName
  *              
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static uint8_t BluetoothSetName(const char* str)
{
	return pBluetoothInterface->SetName(str);
}
/**
  * @brief  BluetoothSearchSlaveDevice
  *         
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static char* BluetoothSearchSlaveDevice(void)
{
	return pBluetoothInterface->SearchSlaveDevice();
}
/**
  * @brief  BluetoothConnect
  *         
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static uint8_t BluetoothConnect(int8_t* mac)
{
	return pBluetoothInterface->Connect(mac);
}
/**
  * @brief  BluetoothDisconnect
  *         
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */	
static void BluetoothDisconnect(void)
{
	pBluetoothInterface->Disconnect();
}
/**
  * @brief  BluetoothDB_Set
  *       	
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static uint8_t BluetoothDB_Set(int8_t TPL)
{
	return pBluetoothInterface->DB_Set(TPL);
}
/**
  * @brief  BluetoothWorkModeSet
  *             
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static void BluetoothWorkModeSet(WorkModeType Mode)
{
	pBluetoothInterface->WorkModeSet(Mode);
}
/**
  * @brief  BluetoothWorkReboot
  *             
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static void BluetoothReboot(void)
{
	pBluetoothInterface->Reboot();
}
/**
  * @brief  BluetoothGetMac
  *         
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static char* BluetoothGetMac(void)
{
	return pBluetoothInterface->GetMac();
}
/**
  * @brief  BluetoothSetBroadcastSpeed
  *       	
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static void BluetoothSetBroadcastSpeed(uint8_t Speed)
{
	pBluetoothInterface->SetBroadcastSpeed(Speed);
}

/**
  * @brief  BluetoothSetCallback
  *       	
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static void BluetoothSetCallback(void (*ptr)(uint8_t* D,uint16_t L))
{
	pBluetoothInterface->SetCallback(ptr);
}

/**
  * @brief  BluetoothSetCallback
  *       	
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
static void BluetoothSendString(int8* str)
{
	pBluetoothInterface->HexTranmission((uint8_t*)str,strlen(str));
}
void BluetoothSendHex(uint8_t* D ,uint16_t Len)
{
	pBluetoothInterface->HexTranmission(D,Len);
}
/**
  * @brief  BluetoothOpen
  *       	
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
Bluetooth_t* BluetoothOpen(BluetoothDeviceType Device)
{
	/*实例化*/
	pBluetoothInterface = &BluetoothImplements;
	
	int Id = -1;
	if(Device == DEVICE1){
		Id = 0;
	}
	else if(Device == DEVICE2){
		Id = 1;
	}else
	{
		return NULL;
	}
	
	if(Id < SUPPORT_DEVICE_NUM){
		pBluetoothInterface->OpenDevice(Device);
		return &Bluetooth[Id];
	}
	return NULL;
}
void BluetoothClose(BluetoothDeviceType Device)
{
	int Id = -1;
	if(Device == DEVICE1){
		Id = 0;
	}
	else if(Device == DEVICE2){
		Id = 1;
	}
	else {
		return ;
	}
	
	if(Id < SUPPORT_DEVICE_NUM){
		pBluetoothInterface->CloseDevice(Device);
	}
}
