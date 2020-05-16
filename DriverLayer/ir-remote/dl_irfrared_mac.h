#ifndef _hstp_MAC_H
#define _hstp_MAC_H

/*换成大小*/
#define HSTP_FIFO_LENGTH                        64

/*自设备MAC地址字节长度且不能超过255*/
#define S_MAC_LEN        												12

typedef struct{
  hstp_uint8 length;                           /*数据包长度*/
  hstp_uint8 Count;                            /*数据发射包数或者时长(以秒为单位)*/
  hstp_uint8 Buffer[HSTP_FIFO_LENGTH];         /*数据缓冲区*/
	hstp_uint8 S_MAC[S_MAC_LEN];                 /*设备的ID号码*/
}hstp_MAC_T;


hstp_CPU_EXT hstp_int8 hstp_Receive_Data(hstp_MAC_T *pMac_Data);
hstp_CPU_EXT hstp_int16 hstp_Send_Data(__Data_LLC_t *user_SDH);
hstp_CPU_EXT void GET_D_MAC(void);
hstp_CPU_EXT hstp_MAC_T MAC;

#endif
