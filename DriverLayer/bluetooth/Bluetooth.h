/**
  ******************************************************************************
  * @file    bluetooth.h
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

#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_
#include "adapter.h"

#define   SUPPORT_DEVICE_NUM                    2/*支持的设备个数*/

/**/
typedef enum{SPP=0,BLE}TransderModeType;
typedef enum{SPP_MASTER=0,BLE_MASTER,IBEACON,SLAVER}WorkModeType;
typedef enum{DEVICE1=0,DEVICE2}BluetoothDeviceType;
typedef enum{OPEN=0,CLOSE}BluetoothPowerCtlType;

/*蓝牙功能函数接口*/
typedef struct
{
	uint8_t (*SetName)(const char* str);/*设置蓝牙的名字*/
	char* (*SearchSlaveDevice)(void);/*搜索从机设备*/
	uint8_t (*Connect)(int8_t*);/*蓝牙连接*/
	void (*Disconnect)(void);/*蓝牙断开连接*/
	uint8_t (*DB_Set)(int8_t TPL);/*蓝牙发射功率设置*/
	void (*WorkModeSet)(WorkModeType Mode);/*蓝牙的工作模式选择 主机 还是 从机*/
	char* (*GetMac)(void);/*查询蓝牙的MAC地址*/
	void (*SetBroadcastSpeed)(uint8_t Speed);/*设置蓝牙的广播速度*/
	void (*OpenDevice)(BluetoothDeviceType);/*打开设备*/
	void (*CloseDevice)(BluetoothDeviceType);/*关闭设备*/
	void (*Reboot)(void);/*重启*/
	void (*SetCallback)(void (*ptr)(uint8_t* D,uint16_t L));/*数据回调函数*/
	void (*HexTranmission)(uint8_t *data,uint16_t length);
}BluetoothInterface_t;
extern BluetoothInterface_t BluetoothImplements;

typedef struct
{
	uint8_t (*SetName)(const char* str);/*设置蓝牙的名字*/
	char* (*SearchSlaveDevice)(void);/*搜索从机设备*/
	uint8_t (*Connect)(int8_t*);/*蓝牙连接*/
	void (*Disconnect)(void);/*蓝牙断开连接*/
	uint8_t (*DB_Set)(int8_t TPL);/*蓝牙发射功率设置*/
	void (*WorkModeSet)(WorkModeType Mode);/*蓝牙的工作模式选择 主机 还是 从机*/
	char* (*GetMac)(void);/*查询蓝牙的MAC地址*/
	void (*SetBroadcastSpeed)(uint8_t Speed);/*设置蓝牙的广播速度*/
	void (*SetCallback)(void (*ptr)(uint8_t* D,uint16_t L));/*数据回调函数*/
	void (*HexTranmission)(uint8_t *data,uint16_t length);
	void (*StringTranmission)(char* str);
	void (*Reboot)(void);/*重启*/
}Bluetooth_t;

/*蓝牙操作函数 API*/
typedef struct
{
	Bluetooth_t* (*Open)(BluetoothDeviceType);
	void (*Close)(BluetoothDeviceType);
}BluetoothDevice_t;
extern BluetoothDevice_t BluetoothDevice;

Bluetooth_t* BluetoothOpen(BluetoothDeviceType Device);
void BluetoothClose(BluetoothDeviceType Device);

#endif
