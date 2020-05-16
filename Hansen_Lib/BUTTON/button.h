#ifndef __BUTTON_H_
#define __BUTTON_H_
/* enum  ------------------------------------------------------------------------*/
/*�����¼�*/
typedef enum 
{
	KEY_DOWN = 0,KEY_UP,KEY_HOME
}KEY_EVENT;
/*�������*/
typedef enum 
{
	KEY1 = 0,KEY2,KEY3,KEY4
}KEY_NUMBER;
/* struct  ----------------------------------------------------------------------*/
typedef struct
{
	KEY_EVENT event;
	KEY_NUMBER index;
}key_message_t;

/* point funtion  ----------------------------------------------------------------*/
typedef void (*callback_keyMesaage)(key_message_t*);
#endif
