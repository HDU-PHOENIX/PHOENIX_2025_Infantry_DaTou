#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "main.h"
#include "dvc_dji_gm6020.h"
#include "dvc_dji_m3508.h"
#include "math.h"
#include "dvc_dr16.h"
#include "alg_pid.h"
#include "Car_Mode.h"
#include "stdbool.h"
#include "dvc_judge.h"
#include "dvc_supercap.h"


#define Gimbal_Yaw_ZERO 2288//云台正方向和底盘正方向重合时候的编码值
#define Gimbal_Yaw_HALF 6384 //云台正方向和底盘负方向重合时候的编码值

#define Transmission_Ratio 19.0f //电机减速比
#define Wheel_Radius 0.07656f //轮子半径m
#define Chassis_Radius 0.290f //底盘半径m

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
void Chassis_fPidCalc(void);
float Find_Angle(void);
void  Chassis_KeyBoard_Control(void);


#endif
