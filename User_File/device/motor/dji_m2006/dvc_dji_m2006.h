#ifndef __DVC_DJI_M2006_H__
#define __DVC_DJI_M2006_H__

#define MOTOR_2006_CIRCLE_ANGLE 36*8192

extern Moto_M2006_t M2006_Rammer;

typedef struct
{
    uint16_t can_id;//电机ID
    uint16_t rotor_angle;//电机角度
    uint16_t last_angle;
    uint32_t total_angle;
    int16_t  rotor_speed;//电机速度
    int16_t  torque_current;//电机扭矩
    int16_t Set_Speed;//设定速度
    uint16_t Set_Angle;//设定角度
    PID_struct_t Angle_PID;//PID各种参数
    PID_struct_t Speed_PID;
}Moto_M2006_t;

void Get_M2006_Motor_Message(uint32_t StdId,uint8_t rx_data[8]);
void Set_M2006_Motor_Voltage(CAN_HandleTypeDef* hcan,Moto_M2006_t M2006_Rammer);
void Get_Total_Angle(Moto_M2006_t *p);
int ABS(int x);

#endif
