#ifndef __SUPERPOWER_H__
#define __SUPERPOWER_H__

extern SuperCap_Tx_Message_t SuperCap_Tx_Message;
extern SuperCap_Rx_Message_t SuperCap_Rx_Message;
extern RC_t RC;
extern SuperCap_Mode_t SuperCap_Mode;
extern SuperCap_Switch_t SuperCap_Switch;
extern Car_Mode_t Car_Mode;

#define    SUPERPOWER_SWITCH_OFFSET_ISON      ((uint16_t)0x01<<0)
#define    SUPERPOWER_SWITCH_OFFSET_ALIVE     ((uint16_t)0x01<<1)
#define    SUPERPOWER_SWITCH_OFFSET_EMPTY     ((uint16_t)0x01<<2)
#define    SUPERPOWER_SWITCH_OFFSET_FULL      ((uint16_t)0x01<<3)
#define    SUPERPOWER_SWITCH_OFFSET_OVPBUS    ((uint16_t)0x01<<4)
#define    SUPERPOWER_SWITCH_OFFSET_UVPBUS    ((uint16_t)0x01<<5)
#define    SUPERPOWER_SWITCH_OFFSET_OVPSRC    ((uint16_t)0x01<<6)
#define    SUPERPOWER_SWITCH_OFFSET_UVPSRC    ((uint16_t)0x01<<7)

//��ⳬ��״̬
#define    IF_SUPERPOWER_SWITCH_OFFSET_ISON      ( (SuperCap_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_ISON) != 0)   //�����Ƿ���
#define    IF_SUPERPOWER_SWITCH_OFFSET_ALIVE     ( (SuperCap_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_ALIVE) != 0)  //�����Ƿ���
#define    IF_SUPERPOWER_SWITCH_OFFSET_EMPTY     ( (SuperCap_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_EMPTY) != 0)  //����û��
#define    IF_SUPERPOWER_SWITCH_OFFSET_FULL      ( (SuperCap_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_FULL) != 0)   //��������
#define    IF_SUPERPOWER_SWITCH_OFFSET_OVPBUS    ( (SuperCap_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_OVPBUS) != 0) //�����쳣״̬0
#define    IF_SUPERPOWER_SWITCH_OFFSET_UVPBUS    ( (SuperCap_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_UVPBUS) != 0) //�����쳣״̬1
#define    IF_SUPERPOWER_SWITCH_OFFSET_OVPSRC    ( (SuperCap_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_OVPSRC) != 0) //�����쳣״̬2
#define    IF_SUPERPOWER_SWITCH_OFFSET_UVPSRC    ( (SuperCap_Rx_Message.Switch & SUPERPOWER_SWITCH_OFFSET_UVPSRC) != 0) //�����쳣״̬3

#define SuperCap_Power_Limit 80

#pragma pack(1)

typedef struct
{
    uint8_t Switch;	       //0   �Ƿ������硯0���رա�1������
    float Limited_Power;   //1-4 ��ǰ���̹�������
	                       //5-7 Ԥ����λ
}SuperCap_Tx_Message_t;

typedef struct
{
    char Switch;	       //0   ״̬
    char Stored_Energy;    //1   ��ǰ����ʣ�����
    char Temperature;      //2   �����¶�
	short Now_power;       //3-4 ��ʵ���̹���
	                       //5-7 Ԥ����λ
}SuperCap_Rx_Message_t;

typedef enum
{
    SuperCap_On   = 0x00,//�����ŵ�
    SuperCap_Off  = 0x01,//���粻�ŵ�
}SuperCap_Mode_t;

typedef enum
{
	SuperCap_Stop = 0x00,//�����Ҳ���ŵ�
    SuperCap_Work = 0x01,//��ʼ����
}SuperCap_Switch_t;

#pragma pack()

void SuperCap_Rx(uint8_t rx_data[8]);
void SuperCap_Tx(void);
void SuperCap_Switch_Control(void);
void SuperCap_KeyBoard_Control(void);
void SuperCap_Init(void);

#endif


