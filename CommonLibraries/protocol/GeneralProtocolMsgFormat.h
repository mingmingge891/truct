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

#ifndef __GENERAL_PROTOCOL_MSG_FORMAT_H_
#define __GENERAL_PROTOCOL_MSG_FORMAT_H_

#include "adapter.h"

#define LOCK1                   'D'
#define LOCK2                   'E'
#define R_CODE                 	0XF1//异或密码
#define RE_SEND_BUFFER          64

//协议1字节对齐
__packed typedef struct{
	u8 	MsgHead[2];//头字节
	u8  MsgLength;//有效数据长度
	u8  MsgClass;//消息类
	u16 MsgCrc16;//校验
	u8  MsgPayLoad[RE_SEND_BUFFER];
}MsgProtocol_t;

//格式话
typedef struct
{
	void (*FmtU8)(u8 d);
	void (*FmtU16)(u16 d);
	void (*FmtU32)(u32 d);
	void (*FmtStr)(char* str);
	void (*FmtHex)(u8* Dt,u16 len);
	void (*FmtHead)(void);
	void (*Fmtend)(void);
	void (*FmtCmd)(u16 cmd);
	void (*FmtCrc16)(void);
	u8*  (*GetBuffer)(void);
	u16  (*GetLength)(void);
	
}ProtocolFormat_t;

//函数
ProtocolFormat_t* ProtocolFormatNew(void);
MsgProtocol_t*  Protocol_Analysis(void* d);
//变量
extern MsgProtocol_t transferProtocol;

#endif
