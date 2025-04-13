/**
 * @file dvc_supercap.c
 * @brief 25��������Э���Լ��������
 * 
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @date 2025-4-13
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */
#include "dvc_supercap.h"

#include "main.h"
#include "can.h"
#include "dvc_judge.h"
#include "string.h"
#include "dvc_dr16.h"
#include "Car_Mode.h"

/**
 * @brief �����ʼ������
 */
void SuperCap_Init(void)
{
    SuperCap_Switch = SuperCap_Stop;
    SuperCap_Mode = SuperCap_Off;
}

/**
 * @brief �������ݴ������ͺ���
 */
void SuperCap_Tx(void)
{
	CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x0C1;//��ʶ��
    tx_header.IDE   = CAN_ID_STD;//��׼ID
    tx_header.RTR   = CAN_RTR_DATA;//����֡
    tx_header.DLC   = 8;//�ֽڳ���

    if(SuperCap_Switch == SuperCap_Stop)
        SuperCap_Tx_Message.Switch=0;
    else if(SuperCap_Switch == SuperCap_Work)
        SuperCap_Tx_Message.Switch=1;
	SuperCap_Tx_Message.Limited_Power = JUDGE_usGetPowerLimit();

	memcpy(tx_data,&SuperCap_Tx_Message,8);
    HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}

/**
 * @brief ����������ݴ���
 * @param rx_data ���յ���ԭʼ����
 */
void SuperCap_Rx(uint8_t rx_data[8])
{
	memcpy(&SuperCap_Rx_Message,rx_data,5);
    SuperCap_Rx_Message.Now_power /= 100;
}

/**
 * @brief ���󿪹س���
 */
bool E_judge=false;
void SuperCap_KeyBoard_Control(void)
{
    if(IF_KEY_PRESSED_E == 1 && E_judge == true)//���E��������
    {
        if(SuperCap_Mode == SuperCap_On)
            SuperCap_Mode = SuperCap_Off;
        else if(SuperCap_Mode == SuperCap_Off)
            SuperCap_Mode = SuperCap_On;
        E_judge = false;
	}
    if(IF_KEY_PRESSED_E == 0)
    {
        E_judge = true;
    }
}

/**
 * @brief ������������
 */
bool C_judge = false;
void SuperCap_Switch_Control(void)
{
    if(SuperCap_Rx_Message.Stored_Energy <= 5)
        SuperCap_Mode = SuperCap_Off;

    if(IF_KEY_PRESSED_C == 1 && C_judge == true)//���C��������
    {
        if(SuperCap_Switch == SuperCap_Stop)
            SuperCap_Switch = SuperCap_Work;
        else if(SuperCap_Switch == SuperCap_Work)
            SuperCap_Switch = SuperCap_Stop;
        C_judge = false;
    }
    if(IF_KEY_PRESSED_C == 0)
    {
        C_judge = true;
    if(IF_SUPERPOWER_SWITCH_OFFSET_ALIVE != 1)
        SuperCap_Switch = SuperCap_Stop;
    }
}
