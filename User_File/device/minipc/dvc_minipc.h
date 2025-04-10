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
char start;                     //0 帧头 's’
char type;                      //1 消息类型 0xA0
char find_bool;                 //2 是否找到敌方
float yaw;                      //3-6 偏航角
float pitch;                    //7-10 俯仰角
uint8_t buff[20];               //11-30 预留空位
char end;                       //31 帧尾 'e’
}Computer_Rx_Message_t;

typedef struct
{
char start;                     //0 帧头 's’
char type;                      //1 消息类型 0xB0
float yaw;                      //2-5 偏航角
float pitch;                    //6-9 俯仰角
uint8_t enemy_team_color;       //10 敌方颜色 0红，1蓝
uint8_t mode;                   //11 模式 0自瞄 1打符
uint8_t rune_flag;              //12 符模式 0不可激活，1小符，2大符
uint8_t buff[18];               //......  13-30 预留空位
char end;                       //31 帧尾 'e’
}Computer_Tx_Message_t;

#pragma pack()

void Computer_Init(void);
void Computer_Tx(void);

#endif
