#ifndef _hstp_LLC_H
#define _hstp_LLC_H

#define	UP_HEAD					0x81
#define	DOWN_HEAD				0x7E
#define	DATA_ERR        -0x03
#define	CRC_ERR	       -0x02
#define	SUC							0x00
#define	REV_USER_DATA		7
#define	SEND_ERR				-1

/*自设备MAC地址字节长度且不能超过255*/
#define MAC_LEN        12

typedef __packed struct {			
//  hstp_uint8 head;					/*帧头*/
//	hstp_uint8 length;				/*一包数据长度*/
//	hstp_uint8 MAC[MAC_LEN];	/*子设备MAC*/
//	hstp_uint8 Type;					/*类型码*/
//	hstp_uint8 Command;				/*命令字*/
//	hstp_uint8 Status1; 			/*状态子1*/
//	hstp_uint8 Status2; 			/*状态子2*/
	
		hstp_uint32 TYPE_MAC_CRC;		/*子设备MAC TYPE CRC*/
}__Data_LLC_t;								/*数据帧头结构结构*/

typedef struct {
	__Data_LLC_t *p_Source_Data;
   int P_Result;
	 int event;
}__User_Data_t;         		/*用户数据头结构*/

hstp_CPU_EXT __User_Data_t* Data_Analyse(hstp_uint8 *data,hstp_uint8 length);
#endif
