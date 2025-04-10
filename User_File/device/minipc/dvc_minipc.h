#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include "main.h"
#include "usb_device.h"
#include "dvc_dji_gm6020.h"
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include "Gimbal.h"

#pragma pack(1)

typedef struct
{
char start;                     //0 ֡ͷ 's��
char type;                      //1 ��Ϣ���� 0xA0
char find_bool;                 //2 �Ƿ��ҵ��з�
float yaw;                      //3-6 ƫ����
float pitch;                    //7-10 ������
uint8_t buff[20];               //11-30 Ԥ����λ
char end;                       //31 ֡β 'e��
}Computer_Rx_Message_t;

typedef struct
{
char start;                     //0 ֡ͷ 's��
char type;                      //1 ��Ϣ���� 0xB0
float yaw;                      //2-5 ƫ����
float pitch;                    //6-9 ������
uint8_t enemy_team_color;       //10 �з���ɫ 0�죬1��
uint8_t mode;                   //11 ģʽ 0���� 1���
uint8_t rune_flag;              //12 ��ģʽ 0���ɼ��1С����2���
uint8_t buff[18];               //......  13-30 Ԥ����λ
char end;                       //31 ֡β 'e��
}Computer_Tx_Message_t;

#pragma pack()

void Computer_Init(void);
void Computer_Tx(void);

#endif
