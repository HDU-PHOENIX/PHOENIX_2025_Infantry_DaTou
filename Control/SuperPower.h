#ifndef __SUPERPOWER_H__
#define __SUPERPOWER_H__

#include "main.h"
#include "can.h"
#include "judge.h"
#include "string.h"
#include "Remote.h"
#include "Car_Mode.h"


#define    SUPERPOWER_SWITCH_OFFSET_ISON      ((uint16_t)0x01<<0)
#define    SUPERPOWER_SWITCH_OFFSET_ALIVE     ((uint16_t)0x01<<1)
#define    SUPERPOWER_SWITCH_OFFSET_EMPTY     ((uint16_t)0x01<<2)
#define    SUPERPOWER_SWITCH_OFFSET_FULL      ((uint16_t)0x01<<3)
#define    SUPERPOWER_SWITCH_OFFSET_OVPBUS    ((uint16_t)0x01<<4)
#define    SUPERPOWER_SWITCH_OFFSET_UVPBUS    ((uint16_t)0x01<<5)
#define    SUPERPOWER_SWITCH_OFFSET_OVPSRC    ((uint16_t)0x01<<6)
#define    SUPERPOWER_SWITCH_OFFSET_UVPSRC    ((uint16_t)0x01<<7)

//检测超电状态
#define    IF_SUPERPOWER_SWITCH_OFFSET_ISON      ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_ISON) != 0)   //超电是否开启
#define    IF_SUPERPOWER_SWITCH_OFFSET_ALIVE     ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_ALIVE) != 0)  //超电是否存活
#define    IF_SUPERPOWER_SWITCH_OFFSET_EMPTY     ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_EMPTY) != 0)  //超电没电
#define    IF_SUPERPOWER_SWITCH_OFFSET_FULL      ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_FULL) != 0)   //超电满电
#define    IF_SUPERPOWER_SWITCH_OFFSET_OVPBUS    ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_OVPBUS) != 0) //超电异常状态0
#define    IF_SUPERPOWER_SWITCH_OFFSET_UVPBUS    ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_UVPBUS) != 0) //超电异常状态1
#define    IF_SUPERPOWER_SWITCH_OFFSET_OVPSRC    ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_OVPSRC) != 0) //超电异常状态2
#define    IF_SUPERPOWER_SWITCH_OFFSET_UVPSRC    ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_UVPSRC) != 0) //超电异常状态3

#define SuperPower_Power_Limit 80

#pragma pack(1)

typedef struct
{
    uint8_t Switch;	       //0   是否开启超电’0’关闭‘1’开启
    float Limited_Power;   //1-4 当前底盘功率上限
	                       //5-7 预留空位
}SuperPower_Tx_Message_t;

typedef struct
{
    char Switch;	       //0   状态
    char Stored_Energy;    //1   当前超电剩余电量
    char Temperature;      //2   电容温度
	short Now_power;       //3-4 真实底盘功率
	                       //5-7 预留空位
}SuperPower_Rx_Message_t;

typedef enum
{
    SuperPower_On   = 0x00,//超电会放电
    SuperPower_Off  = 0x01,//超电不放电
}SuperPower_Mode_t;

typedef enum
{
	SuperPower_Stop = 0x00,//不充电也不放电
    SuperPower_Work = 0x01,//开始工作
}SuperPower_Switch_t;

#pragma pack()

void SuperPower_Rx(uint8_t rx_data[8]);
void SuperPower_Tx(void);
void SuperPower_Switch_Control(void);
void SuperPower_KeyBoard_Control(void);
void SuperPower_Init(void);

#endif


