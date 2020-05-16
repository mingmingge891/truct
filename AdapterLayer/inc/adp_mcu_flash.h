/**
  ******************************************************************************
  * @file    
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
#ifndef __ADP_MCU_FLASH_H_
#define __ADP_MCU_FLASH_H_


int ReadFlashNBtye(uint32_t ReadAddress, uint8_t *ReadBuf, uint16_t ReadNum);
void WriteFlashOneWord(uint32_t WriteAddress,uint8_t* Buf,uint16_t size);
#endif
