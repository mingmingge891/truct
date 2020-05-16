#ifndef _hstp_LLC_H
#define _hstp_LLC_H

#define	UP_HEAD					0x81
#define	DOWN_HEAD				0x7E
#define	DATA_ERR        -0x03
#define	CRC_ERR	       -0x02
#define	SUC							0x00
#define	REV_USER_DATA		7
#define	SEND_ERR				-1

/*���豸MAC��ַ�ֽڳ����Ҳ��ܳ���255*/
#define MAC_LEN        12

typedef __packed struct {			
//  hstp_uint8 head;					/*֡ͷ*/
//	hstp_uint8 length;				/*һ�����ݳ���*/
//	hstp_uint8 MAC[MAC_LEN];	/*���豸MAC*/
//	hstp_uint8 Type;					/*������*/
//	hstp_uint8 Command;				/*������*/
//	hstp_uint8 Status1; 			/*״̬��1*/
//	hstp_uint8 Status2; 			/*״̬��2*/
	
		hstp_uint32 TYPE_MAC_CRC;		/*���豸MAC TYPE CRC*/
}__Data_LLC_t;								/*����֡ͷ�ṹ�ṹ*/

typedef struct {
	__Data_LLC_t *p_Source_Data;
   int P_Result;
	 int event;
}__User_Data_t;         		/*�û�����ͷ�ṹ*/

hstp_CPU_EXT __User_Data_t* Data_Analyse(hstp_uint8 *data,hstp_uint8 length);
#endif
