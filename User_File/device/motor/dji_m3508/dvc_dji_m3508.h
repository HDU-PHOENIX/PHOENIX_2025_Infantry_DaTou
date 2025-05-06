#ifndef __M3508_H__
#define __M3508_H__

extern Moto_M3508_t M3508_Chassis[4],M3508_Shoot[2];

typedef struct
{
    uint16_t can_id;//电机ID
    uint16_t rotor_angle;//电机角度
    int16_t  rotor_speed;//电机速度
    int16_t  torque_current;//电机扭矩
    uint8_t  temp;//温度
    int16_t Set_Speed;//设定速度
    uint16_t Set_Angle;//设定角度
    PID_struct_t PID;//PID各种参数
}Moto_M3508_t;

void Get_M3508_Chassis_Message(uint32_t StdId,uint8_t rx_data[8]);
void Set_M3508_Chassis_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Chassis[4]);
void Get_M3508_Shoot_Message(uint32_t StdId,uint8_t rx_data[8]);
void Set_M3508_Shoot_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Shoot[2]);

#endif
