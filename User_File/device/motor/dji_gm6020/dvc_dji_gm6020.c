#include "dvc_dji_gm6020.h"

extern Moto_GM6020_t GM6020_Yaw,GM6020_Pitch;
HAL_StatusTypeDef can_1;
HAL_StatusTypeDef can_2;

/**
 * @file GM6020.c
 * @brief GM6020接受反馈报文函数
 * @param StdId 电机ID
 * @param rx_data CAN通道收到的数据
 * @author HWX
 * @date 2024/10/20
 */
void Get_GM6020_Motor_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//接收指定电机反馈的信息
    {
        case 0x207://反馈报文标识符
        {
            GM6020_Yaw.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//接收机械角度（16bit）
            GM6020_Yaw.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//接收转速（16bit）
            GM6020_Yaw.torque_current = ((rx_data[4] << 8) | rx_data[5]);//接收实际转矩
            GM6020_Yaw.temp           =   rx_data[6];//接收电机温度（8bit）
            break;
        }
        case 0x205://反馈报文标识符
        {
            GM6020_Pitch.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//接收机械角度（16bit）
            GM6020_Pitch.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//接收转速（16bit）
            GM6020_Pitch.torque_current = ((rx_data[4] << 8) | rx_data[5]);//接收实际转矩
            GM6020_Pitch.temp           =   rx_data[6];//接收电机温度（8bit）
            break;
        }
    }
}

/**
 * @file GM6020.c
 * @brief 发送电压控制报文
 * @param hcan CAN通道
 * @param GM6020_Pitch Pitch轴电机
 * @param GM6020_Yaw Yaw轴电机
 * @author HWX
 * @date 2024/10/20
 */
void Set_GM6020_Gimbal_Voltage(CAN_HandleTypeDef* hcan,Moto_GM6020_t GM6020_Yaw,Moto_GM6020_t GM6020_Pitch)
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             CAN1_tx_data[8] = {0},CAN2_tx_data[8] = {0};
    
    tx_header.StdId = 0X1FF;//标识符（见手册P6）
    tx_header.IDE   = CAN_ID_STD;//标准ID
    tx_header.RTR   = CAN_RTR_DATA;//数据帧
    tx_header.DLC   = 8;//字节长度
    CAN1_tx_data[4] = ((int16_t)GM6020_Yaw.Speed_PID.output>>8)&0xff;
    CAN1_tx_data[5] = ((int16_t)GM6020_Yaw.Speed_PID.output)&0xff;
    can_1=HAL_CAN_AddTxMessage(&hcan1, &tx_header, CAN1_tx_data,(uint32_t*)CAN_TX_MAILBOX0);

    CAN2_tx_data[0] = ((int16_t)GM6020_Pitch.Speed_PID.output>>8)&0xff;
    CAN2_tx_data[1] = ((int16_t)GM6020_Pitch.Speed_PID.output)&0xff;
    can_2=HAL_CAN_AddTxMessage(&hcan2, &tx_header, CAN2_tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}
