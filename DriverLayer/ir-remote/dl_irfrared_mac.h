#ifndef _hstp_MAC_H
#define _hstp_MAC_H

/*���ɴ�С*/
#define HSTP_FIFO_LENGTH                        64

/*���豸MAC��ַ�ֽڳ����Ҳ��ܳ���255*/
#define S_MAC_LEN        												12

typedef struct{
  hstp_uint8 length;                           /*���ݰ�����*/
  hstp_uint8 Count;                            /*���ݷ����������ʱ��(����Ϊ��λ)*/
  hstp_uint8 Buffer[HSTP_FIFO_LENGTH];         /*���ݻ�����*/
	hstp_uint8 S_MAC[S_MAC_LEN];                 /*�豸��ID����*/
}hstp_MAC_T;


hstp_CPU_EXT hstp_int8 hstp_Receive_Data(hstp_MAC_T *pMac_Data);
hstp_CPU_EXT hstp_int16 hstp_Send_Data(__Data_LLC_t *user_SDH);
hstp_CPU_EXT void GET_D_MAC(void);
hstp_CPU_EXT hstp_MAC_T MAC;

#endif
