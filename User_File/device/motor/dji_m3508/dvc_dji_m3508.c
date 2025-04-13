/**
 * @file dvc_dji_m3508.c
 * @brief dji_m3508电机控制函数
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
 * @brief 底盘M3508接受反馈报文函数
 * @param StdId 电机ID
 * @param rx_data CAN通道收到的数据
 */
void Get_M3508_Chassis_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//接收指定电机反馈的信息
    {
        case 0x201://反馈报文标识符
        {
            M3508_Chassis[0].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//接收机械角度（16bit）
            M3508_Chassis[0].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//接收转速（16bit）
            M3508_Chassis[0].torque_current = ((rx_data[4] << 8) | rx_data[5]);//接收实际转矩
            M3508_Chassis[0].temp           =   rx_data[6];//接收电机温度（8bit）
            break;
        }
        case 0x202://反馈报文标识符
        {
            M3508_Chassis[1].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//接收机械角度（16bit）
            M3508_Chassis[1].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//接收转速（16bit）
            M3508_Chassis[1].torque_current = ((rx_data[4] << 8) | rx_data[5]);//接收实际转矩
            M3508_Chassis[1].temp           =   rx_data[6];//接收电机温度（8bit）
            break;
        }
        case 0x203://反馈报文标识符
        {
            M3508_Chassis[2].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//接收机械角度（16bit）
            M3508_Chassis[2].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//接收转速（16bit）
            M3508_Chassis[2].torque_current = ((rx_data[4] << 8) | rx_data[5]);//接收实际转矩
            M3508_Chassis[2].temp           =   rx_data[6];//接收电机温度（8bit）
            break;
        } 
        case 0x204://反馈报文标识符
        {
            M3508_Chassis[3].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//接收机械角度（16bit）
            M3508_Chassis[3].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//接收转速（16bit）
            M3508_Chassis[3].torque_current = ((rx_data[4] << 8) | rx_data[5]);//接收实际转矩
            M3508_Chassis[3].temp           =   rx_data[6];//接收电机温度（8bit）
            break;
        }
    }
}

/**
 * @brief 底盘M3508发送电流报文控制函数
 * @param hcan CAN通道
 * @param M3508_Chassis 底盘电机
 */
void Set_M3508_Chassis_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Chassis[4])
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x200;//标识符（见手册P6）
    tx_header.IDE   = CAN_ID_STD;//标准ID
    tx_header.RTR   = CAN_RTR_DATA;//数据帧
    tx_header.DLC   = 8;//字节长度

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
 * @brief 射击M3508接受反馈报文函数
 * @param StdId 电机ID
 * @param rx_data CAN通道收到的数据
 */
void Get_M3508_Shoot_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//接收指定电机反馈的信息
    {
        case 0x204://反馈报文标识符
        {
            M3508_Shoot[0].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//接收机械角度（16bit）
            M3508_Shoot[0].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//接收转速（16bit）
            M3508_Shoot[0].torque_current = ((rx_data[4] << 8) | rx_data[5]);//接收实际转矩
            M3508_Shoot[0].temp           =   rx_data[6];//接收电机温度（8bit）
            break;
        }
        case 0x203://反馈报文标识符
        {
            M3508_Shoot[1].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//接收机械角度（16bit）
            M3508_Shoot[1].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//接收转速（16bit）
            M3508_Shoot[1].torque_current = ((rx_data[4] << 8) | rx_data[5]);//接收实际转矩
            M3508_Shoot[1].temp           =   rx_data[6];//接收电机温度（8bit）
            break;
        }
    }
}


/**
 * @brief 射击M3508发送电流报文控制函数
 * @param hcan CAN通道
 * @param M3508_Shoot 射击电机
 */
void Set_M3508_Shoot_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Shoot[2])
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x200;//标识符（见手册P6）
    tx_header.IDE   = CAN_ID_STD;//标准ID
    tx_header.RTR   = CAN_RTR_DATA;//数据帧
    tx_header.DLC   = 8;//字节长度

    tx_data[6] = ((int16_t)M3508_Shoot[0].PID.output>>8)&0xff;
    tx_data[7] = ((int16_t)M3508_Shoot[0].PID.output)&0xff;

    tx_data[4] = ((int16_t)M3508_Shoot[1].PID.output>>8)&0xff;
    tx_data[5] = ((int16_t)M3508_Shoot[1].PID.output)&0xff;

    HAL_CAN_AddTxMessage(&hcan2, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}
