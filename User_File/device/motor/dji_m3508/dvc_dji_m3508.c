/**
 * @file dvc_dji_m3508.c
 * @brief dji_m3508������ƺ���
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @date 2025-4-13
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */
#include "dvc_dji_m3508.h"

#include "main.h"
#include "can.h"
#include "stm32f4xx.h"
#include "alg_pid.h"
#include "stdint.h"
#include "stm32f4xx_hal_can.h"
/**
 * @brief ����M3508���ܷ������ĺ���
 * @param StdId ���ID
 * @param rx_data CANͨ���յ�������
 */
void Get_M3508_Chassis_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//����ָ�������������Ϣ
    {
        case 0x201://�������ı�ʶ��
        {
            M3508_Chassis[0].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//���ջ�е�Ƕȣ�16bit��
            M3508_Chassis[0].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//����ת�٣�16bit��
            M3508_Chassis[0].torque_current = ((rx_data[4] << 8) | rx_data[5]);//����ʵ��ת��
            M3508_Chassis[0].temp           =   rx_data[6];//���յ���¶ȣ�8bit��
            break;
        }
        case 0x202://�������ı�ʶ��
        {
            M3508_Chassis[1].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//���ջ�е�Ƕȣ�16bit��
            M3508_Chassis[1].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//����ת�٣�16bit��
            M3508_Chassis[1].torque_current = ((rx_data[4] << 8) | rx_data[5]);//����ʵ��ת��
            M3508_Chassis[1].temp           =   rx_data[6];//���յ���¶ȣ�8bit��
            break;
        }
        case 0x203://�������ı�ʶ��
        {
            M3508_Chassis[2].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//���ջ�е�Ƕȣ�16bit��
            M3508_Chassis[2].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//����ת�٣�16bit��
            M3508_Chassis[2].torque_current = ((rx_data[4] << 8) | rx_data[5]);//����ʵ��ת��
            M3508_Chassis[2].temp           =   rx_data[6];//���յ���¶ȣ�8bit��
            break;
        } 
        case 0x204://�������ı�ʶ��
        {
            M3508_Chassis[3].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//���ջ�е�Ƕȣ�16bit��
            M3508_Chassis[3].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//����ת�٣�16bit��
            M3508_Chassis[3].torque_current = ((rx_data[4] << 8) | rx_data[5]);//����ʵ��ת��
            M3508_Chassis[3].temp           =   rx_data[6];//���յ���¶ȣ�8bit��
            break;
        }
    }
}

/**
 * @brief ����M3508���͵������Ŀ��ƺ���
 * @param hcan CANͨ��
 * @param M3508_Chassis ���̵��
 */
void Set_M3508_Chassis_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Chassis[4])
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x200;//��ʶ�������ֲ�P6��
    tx_header.IDE   = CAN_ID_STD;//��׼ID
    tx_header.RTR   = CAN_RTR_DATA;//����֡
    tx_header.DLC   = 8;//�ֽڳ���

    tx_data[0] = ((int16_t)M3508_Chassis[0].PID.output>>8)&0xff;
    tx_data[1] = ((int16_t)M3508_Chassis[0].PID.output)&0xff;

    tx_data[2] = ((int16_t)M3508_Chassis[1].PID.output>>8)&0xff;
    tx_data[3] = ((int16_t)M3508_Chassis[1].PID.output)&0xff;

    tx_data[4] = ((int16_t)M3508_Chassis[2].PID.output>>8)&0xff;
    tx_data[5] = ((int16_t)M3508_Chassis[2].PID.output)&0xff;

    tx_data[6] = ((int16_t)M3508_Chassis[3].PID.output>>8)&0xff;
    tx_data[7] = ((int16_t)M3508_Chassis[3].PID.output)&0xff;

    HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}

/**
 * @brief ���M3508���ܷ������ĺ���
 * @param StdId ���ID
 * @param rx_data CANͨ���յ�������
 */
void Get_M3508_Shoot_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//����ָ�������������Ϣ
    {
        case 0x204://�������ı�ʶ��
        {
            M3508_Shoot[0].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//���ջ�е�Ƕȣ�16bit��
            M3508_Shoot[0].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//����ת�٣�16bit��
            M3508_Shoot[0].torque_current = ((rx_data[4] << 8) | rx_data[5]);//����ʵ��ת��
            M3508_Shoot[0].temp           =   rx_data[6];//���յ���¶ȣ�8bit��
            break;
        }
        case 0x203://�������ı�ʶ��
        {
            M3508_Shoot[1].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//���ջ�е�Ƕȣ�16bit��
            M3508_Shoot[1].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//����ת�٣�16bit��
            M3508_Shoot[1].torque_current = ((rx_data[4] << 8) | rx_data[5]);//����ʵ��ת��
            M3508_Shoot[1].temp           =   rx_data[6];//���յ���¶ȣ�8bit��
            break;
        }
    }
}


/**
 * @brief ���M3508���͵������Ŀ��ƺ���
 * @param hcan CANͨ��
 * @param M3508_Shoot ������
 */
void Set_M3508_Shoot_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Shoot[2])
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x200;//��ʶ�������ֲ�P6��
    tx_header.IDE   = CAN_ID_STD;//��׼ID
    tx_header.RTR   = CAN_RTR_DATA;//����֡
    tx_header.DLC   = 8;//�ֽڳ���

    tx_data[6] = ((int16_t)M3508_Shoot[0].PID.output>>8)&0xff;
    tx_data[7] = ((int16_t)M3508_Shoot[0].PID.output)&0xff;

    tx_data[4] = ((int16_t)M3508_Shoot[1].PID.output>>8)&0xff;
    tx_data[5] = ((int16_t)M3508_Shoot[1].PID.output)&0xff;

    HAL_CAN_AddTxMessage(&hcan2, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}
