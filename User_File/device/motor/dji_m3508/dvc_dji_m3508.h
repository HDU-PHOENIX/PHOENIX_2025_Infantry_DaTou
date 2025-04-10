#ifndef __M3508_H__
#define __M3508_H__

#include "main.h"
#include "can.h"
#include "stm32f4xx.h"
#include "alg_pid.h"
#include "stdint.h"
#include "stm32f4xx_hal_can.h"

typedef struct
{
    uint16_t can_id;//���ID
    uint16_t rotor_angle;//����Ƕ�
    int16_t  rotor_speed;//����ٶ�
    int16_t  torque_current;//���Ť��
    uint8_t  temp;//�¶�
    int16_t Set_Speed;//�趨�ٶ�
    uint16_t Set_Angle;//�趨�Ƕ�
    PID_struct_t PID;//PID���ֲ���
}Moto_M3508_t;

void Get_M3508_Chassis_Message(uint32_t StdId,uint8_t rx_data[8]);
void Set_M3508_Chassis_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Chassis[4]);
void Get_M3508_Shoot_Message(uint32_t StdId,uint8_t rx_data[8]);
void Set_M3508_Shoot_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Shoot[2]);

#endif
