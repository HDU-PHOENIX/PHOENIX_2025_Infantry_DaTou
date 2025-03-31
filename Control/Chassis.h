#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "main.h"
#include "GM6020.h"
#include "M3508.h"
#include "math.h"
#include "Remote.h"
#include "PID.h"
#include "Car_Mode.h"
#include "stdbool.h"
#include "judge.h"
#include "SuperPower.h"


#define Gimbal_Yaw_ZERO 3260//��̨������͵����������غ�ʱ��ı���ֵ
#define Gimbal_Yaw_HALF 7356 //��̨������͵��̸������غ�ʱ��ı���ֵ

#define Transmission_Ratio 19.0f //������ٱ�
#define Wheel_Radius 0.07656f //���Ӱ뾶m
#define Chassis_Radius 0.290f //���̰뾶m

#define Chassis_Power_Set 30

typedef struct
{
    float vx;
    float vy;
    float vw;
}Chassis_Speed_t;

typedef struct
{
    float x;
    float y;
    float w;
    float t;
}Chassis_Step;

void Chassis_Move(void);
void Chassis_Remote_Control(void);
void Chassis_Remote_Mode(void);
void Chassis_PID_Init_All(void);
void Chassis_PID_Clean_All(void);
void Chassis_Stop(void);
void Chassis_PID_Calc(void);
float Find_Angle(void);
void  Chassis_KeyBoard_Control(void);


#endif
