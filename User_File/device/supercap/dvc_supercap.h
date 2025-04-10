#ifndef __SUPERPOWER_H__
#define __SUPERPOWER_H__

#include "main.h"
#include "can.h"
#include "dvc_judge.h"
#include "string.h"
#include "dvc_dr16.h"
#include "Car_Mode.h"


#define    SUPERPOWER_SWITCH_OFFSET_ISON      ((uint16_t)0x01<<0)
#define    SUPERPOWER_SWITCH_OFFSET_ALIVE     ((uint16_t)0x01<<1)
#define    SUPERPOWER_SWITCH_OFFSET_EMPTY     ((uint16_t)0x01<<2)
#define    SUPERPOWER_SWITCH_OFFSET_FULL      ((uint16_t)0x01<<3)
#define    SUPERPOWER_SWITCH_OFFSET_OVPBUS    ((uint16_t)0x01<<4)
#define    SUPERPOWER_SWITCH_OFFSET_UVPBUS    ((uint16_t)0x01<<5)
#define    SUPERPOWER_SWITCH_OFFSET_OVPSRC    ((uint16_t)0x01<<6)
#define    SUPERPOWER_SWITCH_OFFSET_UVPSRC    ((uint16_t)0x01<<7)

//��ⳬ��״̬
#define    IF_SUPERPOWER_SWITCH_OFFSET_ISON      ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_ISON) != 0)   //�����Ƿ���
#define    IF_SUPERPOWER_SWITCH_OFFSET_ALIVE     ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_ALIVE) != 0)  //�����Ƿ���
#define    IF_SUPERPOWER_SWITCH_OFFSET_EMPTY     ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_EMPTY) != 0)  //����û��
#define    IF_SUPERPOWER_SWITCH_OFFSET_FULL      ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_FULL) != 0)   //��������
#define    IF_SUPERPOWER_SWITCH_OFFSET_OVPBUS    ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_OVPBUS) != 0) //�����쳣״̬0
#define    IF_SUPERPOWER_SWITCH_OFFSET_UVPBUS    ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_UVPBUS) != 0) //�����쳣״̬1
#define    IF_SUPERPOWER_SWITCH_OFFSET_OVPSRC    ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_OVPSRC) != 0) //�����쳣״̬2
#define    IF_SUPERPOWER_SWITCH_OFFSET_UVPSRC    ( (SuperPower_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_UVPSRC) != 0) //�����쳣״̬3

#define SuperPower_Power_Limit 80

#pragma pack(1)

typedef struct
{
    uint8_t Switch;	       //0   �Ƿ������硯0���رա�1������
    float Limited_Power;   //1-4 ��ǰ���̹�������
	                       //5-7 Ԥ����λ
}SuperPower_Tx_Message_t;

typedef struct
{
    char Switch;	       //0   ״̬
    char Stored_Energy;    //1   ��ǰ����ʣ�����
    char Temperature;      //2   �����¶�
	short Now_power;       //3-4 ��ʵ���̹���
	                       //5-7 Ԥ����λ
}SuperPower_Rx_Message_t;

typedef enum
{
    SuperPower_On   = 0x00,//�����ŵ�
    SuperPower_Off  = 0x01,//���粻�ŵ�
}SuperPower_Mode_t;

typedef enum
{
	SuperPower_Stop = 0x00,//�����Ҳ���ŵ�
    SuperPower_Work = 0x01,//��ʼ����
}SuperPower_Switch_t;

#pragma pack()

void SuperPower_Rx(uint8_t rx_data[8]);
void SuperPower_Tx(void);
void SuperPower_Switch_Control(void);
void SuperPower_KeyBoard_Control(void);
void SuperPower_Init(void);

#endif


