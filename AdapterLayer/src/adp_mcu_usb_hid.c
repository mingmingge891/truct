/**
  ******************************************************************************
  * File Name          : adp_mcu_usb_hid.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "adp_mcu_usb_hid.h"
#include "adp_mcu_delay.h"
#include "usb_conf.h"
#include "usb_mem.h"
#include "usb_lib.h"
#include "usb_regs.h"
#include "usb_hw.h"
#include "string.h"
#include "stdarg.h"
#include <stdio.h>
/**
* @brief This Value handles System Callback for USB HID
*/
ADP_USB_HID_t Callback;
extern u8 Transi_Buffer[22];
extern u8 Receive_Buffer[22];
extern u8 USB_ReceiveFlg;

/**
  * @brief  USB_HID_Transmit_FS
  *         Data send over USB IN endpoint are sent over CDC interface 
  *         through this function.           
  *         @note
  *         
  *                 
  * @param  Buf: Buffer of data to be send
  * @param  Len: Number of data to be send (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
void ADP_USB_HID_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
	memcpy(Transi_Buffer,Buf,22);
	UserToPMABufferCopy(Transi_Buffer, ENDP2_TXADDR, 22);
	SetEPTxValid(ENDP2);
}
void ADP_USB_HID_Transmit_String(const char* format,...)
{
	
	#define IFS_PRINTF_BUF_LEN 64
	static u8 prt_buf[IFS_PRINTF_BUF_LEN];
	u8* pBuf = prt_buf;
	va_list ap;
	uint32_t size = 0;
	
	va_start(ap, format);
	size = vsnprintf((char *)pBuf, IFS_PRINTF_BUF_LEN, format, ap);
	va_end(ap);

	pBuf[size] = 0;

	static int IrStuLen = 0;
	int len = 0;
	
	len = strlen((char*)pBuf);
	IrStuLen = len;
	while(len > 0)
	{
		 memcpy(Transi_Buffer,pBuf+(IrStuLen-len),22);
		 UserToPMABufferCopy(Transi_Buffer, ENDP2_TXADDR, 22);
		 SetEPTxValid(ENDP2);
		 len-=22;
		 ADP_delay_ms(30);
	}
}

/**
  * @brief  USB_VCP_SetDataCb
  *         Data send over USB IN endpoint are sent over CDC interface 
  *         through this function.           
  *         @note
  *         
  *         
  * @param  Buf: Buffer of data to be send
  * @param  Len: Number of data to be send (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
void ADP_USB_HID_SetDataCb(void (*arg)(uint8_t* Buf, uint32_t Len))
{
	Callback.DataCb = arg;
}

/**
  * @brief  USB_HID_Initialize
  *         Usb Initialize          
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
void ADP_USB_HID_Initialize(void)
{
	
	/*设置USB的时钟 USB HID时钟不能超过48MHZ 所以 72MHZ 1.5分频*/
	/*如果是48MZHA 就选RCC_USBCLKSource_PLLCLK_Div1*/
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
	
	/*USB 连接控制线*/
	USB_Disconnect_Config();
	
	/*USB初始化*/
	USB_Init();
	
}

/**
  * @brief  USB_HID_ReceiveHandler
  *             
  *         @note
  *         
  *                 
  * @param  
  * @param  
  * @retval 
  */
void ADP_USB_HID_ReceiveHandler(void)
{
	if(USB_ReceiveFlg == TRUE)
	{
		USB_ReceiveFlg  = FALSE;
		Callback.DataCb(Receive_Buffer,22);
	}
	
}

