#include "Chassis.h"

extern Car_Mode_t Car_Mode;
extern RC_t RC;
extern Chassis_Speed_t Chassis_Speed;
extern Chassis_Speed_t Temp1_Chassis_Speed,Temp2_Chassis_Speed;
extern Moto_GM6020_t GM6020_Yaw;
extern Moto_M3508_t M3508_Chassis[4];
extern PID_struct_t Follow_PID;
extern float Chassis_Power_Limit,Chassis_Power_Buffer;
extern SuperPower_Mode_t SuperPower_Mode;
extern SuperPower_Switch_t SuperPower_Switch;
extern SuperPower_Rx_Message_t SuperPower_Rx_Message;

float Angle;
float err;

/********************解算部分********************/
void Chassis_Solution(void);
void Chassis_Motor_Solution(void);
float Find_Angle(void);
void Chassis_Speed_XiePo(Chassis_Speed_t* target_speed, Chassis_Speed_t* XiePo_speed);
float Find_Min_Angle(void);

/********************输入控制部分********************/
void Chassis_Remote_Control(void);
void  Chassis_KeyBoard_Control(void);

/********************PID部分********************/
void Chassis_PID_Init_All(void);
void Chassis_PID_Clean_All(void);
void Chassis_PID_Calc(void);

/********************输出控制部分********************/
void Chassis_Move(void);
void Chassis_Stop(void);


/**
 * @file Chassis.c
 * @brief 把云台坐标系解算到底盘的坐标系
 * @author HWX
 * @date 2024/10/20
 */
void Chassis_Solution(void)
{
    Angle = Find_Angle();
    Temp2_Chassis_Speed.vx = Temp1_Chassis_Speed.vx * cos(Angle) + Temp1_Chassis_Speed.vy * sin(Angle);
    Temp2_Chassis_Speed.vy = -Temp1_Chassis_Speed.vx * sin(Angle) + Temp1_Chassis_Speed.vy * cos(Angle);
    Temp2_Chassis_Speed.vw = Temp1_Chassis_Speed.vw;
	if(Chassis_Power_Buffer>0 && Chassis_Power_Limit>0)
	{
		Chassis_Speed_XiePo(&Temp2_Chassis_Speed,&Chassis_Speed);
	}else{
        Chassis_Speed=Temp2_Chassis_Speed;
	}
	if(Car_Mode.Action == FOLLOW)//跟随模式vw不限功率
    {
        Temp2_Chassis_Speed.vw = Temp1_Chassis_Speed.vw;
    }
}

/**
 * @file Chassis.c
 * @brief 把底盘运动解算到电机运动
 * @author HWX
 * @date 2024/10/20
 */
void Chassis_Motor_Solution(void)
{

    M3508_Chassis[0].Set_Speed = (Chassis_Speed.vx*Transmission_Ratio*30.0f)/(3.14f*1.4142f*Wheel_Radius) + (Chassis_Speed.vy*Transmission_Ratio*30.0f)/(3.14f*1.4142f*Wheel_Radius) + (Chassis_Speed.vw*Chassis_Radius*30.0f*Transmission_Ratio)/(3.14f*Wheel_Radius);
    M3508_Chassis[1].Set_Speed = -(Chassis_Speed.vx*Transmission_Ratio*30.0f)/(3.14f*1.4142f*Wheel_Radius) + (Chassis_Speed.vy*Transmission_Ratio*30.0f)/(3.14f*1.4142f*Wheel_Radius) + (Chassis_Speed.vw*Chassis_Radius*30.0f*Transmission_Ratio)/(3.14f*Wheel_Radius);
    M3508_Chassis[2].Set_Speed = -(Chassis_Speed.vx*Transmission_Ratio*30.0f)/(3.14f*1.4142f*Wheel_Radius) - (Chassis_Speed.vy*Transmission_Ratio*30.0f)/(3.14f*1.4142f*Wheel_Radius) + (Chassis_Speed.vw*Chassis_Radius*30.0f*Transmission_Ratio)/(3.14f*Wheel_Radius);
    M3508_Chassis[3].Set_Speed = (Chassis_Speed.vx*Transmission_Ratio*30.0f)/(3.14f*1.4142f*Wheel_Radius) - (Chassis_Speed.vy*Transmission_Ratio*30.0f)/(3.14f*1.4142f*Wheel_Radius) + (Chassis_Speed.vw*Chassis_Radius*30.0f*Transmission_Ratio)/(3.14f*Wheel_Radius);
}


/**
 * @file Chassis.c
 * @brief 底盘开始运动
 * @author HWX
 * @date 2024/10/20
 */
void Chassis_Move(void)
{

    Chassis_Solution();

    Chassis_Motor_Solution();

	Chassis_PID_Calc();

    Set_M3508_Chassis_Voltage(&hcan1,M3508_Chassis);
}

/**
 * @file Chassis.c
 * @brief 通过清空PID使输出为0
 * @author HWX
 * @date 2024/10/20
 */
void Chassis_Stop(void)
{
    Chassis_PID_Calc();
    Set_M3508_Chassis_Voltage(&hcan1,M3508_Chassis);
}

/**
 * @file Chassis.c
 * @brief 找出底盘和云台+x轴的角度差
 * @retval 角度差（-PI~PI）
 * @author HWX
 * @date 2024/10/20
 */
float Find_Angle(void)
{
	float Angle,Zero;
	Angle = GM6020_Yaw.rotor_angle;
	Zero = Gimbal_Yaw_ZERO;

	if(Angle - Zero > 4096)
	{
		Zero+=8190;
	}
	else if(Angle - Zero < -4096)
	{
		Zero-=8190;
	}
    
	err = Angle-Zero;
	
	float temp1 = err * 2 * 3.1415926f / 8192;
    if(temp1 > 3.141593f)
        temp1 = 3.141593f;
    else if(temp1 < -3.141593f)
        temp1 = -3.141593f;
    return temp1;
}

float Find_Min_Angle(void) 
{
	float Angle = GM6020_Yaw.rotor_angle;
    float temp1=Angle-Gimbal_Yaw_ZERO;
    float temp2=Angle-Gimbal_Yaw_HALF;
	float mintemp1;
    if(temp1>4096)
        temp1-=8192;
    else if(temp1<-4096)
        temp1+=8192;
    if(temp2>4096)
        temp2-=8192;
    else if(temp2<-4096)
        temp2+=8192;
	mintemp1 = (fabs(temp1) < fabs(temp2) ? temp1 : temp2);
    return mintemp1;
}


/**
 * @file Chassis.c
 * @brief 遥控器控制底盘
 * @author HWX
 * @date 2024/10/20
 */
void Chassis_Remote_Control(void)
{
    switch (Car_Mode.Action)
    {
    case GYROSCOPE:
        Temp1_Chassis_Speed.vx = (float)RC.ch3/132;
        Temp1_Chassis_Speed.vy = (float)RC.ch2/132;
        Temp1_Chassis_Speed.vw =12.56f;
        break;
    case NORMAL:
        Temp1_Chassis_Speed.vx = (float)RC.ch3/132;
        Temp1_Chassis_Speed.vy = (float)RC.ch2/132;
        Temp1_Chassis_Speed.vw = 0;
        break;
    case FOLLOW:
        Temp1_Chassis_Speed.vx = (float)RC.ch3/132;
        Temp1_Chassis_Speed.vy = (float)RC.ch2/132;
		PID_Calc_Angle(&Follow_PID,0.0f,Find_Min_Angle(),8192,0);
        Temp1_Chassis_Speed.vw = Follow_PID.output*0.0007666f;
    default:
        break;
    }
}

/**
 * @file Chassis.c
 * @brief 计算电机PID
 * @author HWX
 * @date 2024/10/20
 */
void Chassis_PID_Calc(void)
{
    PID_Calc_Speed(&M3508_Chassis[0].PID,M3508_Chassis[0].Set_Speed,M3508_Chassis[0].rotor_speed);
    PID_Calc_Speed(&M3508_Chassis[1].PID,M3508_Chassis[1].Set_Speed,M3508_Chassis[1].rotor_speed);
    PID_Calc_Speed(&M3508_Chassis[2].PID,M3508_Chassis[2].Set_Speed,M3508_Chassis[2].rotor_speed);
    PID_Calc_Speed(&M3508_Chassis[3].PID,M3508_Chassis[3].Set_Speed,M3508_Chassis[3].rotor_speed);
}

/**
 * @file Chassis.c
 * @brief 底盘电机PID初始化
 * @author HWX
 * @date 2024/10/20
 */
void Chassis_PID_Init_All(void)
{
    PID_init(&Follow_PID,5,0,300,0,16308);
    PID_init(&(M3508_Chassis[0].PID),10,1.5,0,2000,8192);
    PID_init(&(M3508_Chassis[1].PID),10,1.5,0,2000,8192);
    PID_init(&(M3508_Chassis[2].PID),10,1.5,0,2000,8192);
    PID_init(&(M3508_Chassis[3].PID),10,1.5,0,2000,8192);
}

/**
 * @file Chassis.c
 * @brief 底盘电机PID清空
 * @author HWX
 * @date 2024/10/20
 */
void Chassis_PID_Clean_All(void)
{
    PID_init(&Follow_PID,0,0,0,0,0);
    PID_init(&(M3508_Chassis[0].PID),0,0,0,0,0);
    PID_init(&(M3508_Chassis[1].PID),0,0,0,0,0);
    PID_init(&(M3508_Chassis[2].PID),0,0,0,0,0);
    PID_init(&(M3508_Chassis[3].PID),0,0,0,0,0);
}

/**
 * @file Chassis.c
 * @brief 键盘控制底盘
 * @author HWX
 * @date 2025/2/10
 */
void  Chassis_KeyBoard_Control(void)
{
    switch (Car_Mode.Action)
    {
    case GYROSCOPE:
        if(IF_KEY_PRESSED_W == 1)
            Temp1_Chassis_Speed.vx = 2.0f;
        if(IF_KEY_PRESSED_S == 1)
            Temp1_Chassis_Speed.vx = -2.0f;
        if(IF_KEY_PRESSED_W == 0 && IF_KEY_PRESSED_S == 0)
            Temp1_Chassis_Speed.vx = 0.0f;
        if(IF_KEY_PRESSED_A == 1)
            Temp1_Chassis_Speed.vy = -2.0f;
        if(IF_KEY_PRESSED_D == 1)
            Temp1_Chassis_Speed.vy = 2.0f;
        if(IF_KEY_PRESSED_A == 0 && IF_KEY_PRESSED_D == 0)
            Temp1_Chassis_Speed.vy = 0.0f;
        Temp1_Chassis_Speed.vw = 12.56f;
        break;
    case NORMAL:
        if(IF_KEY_PRESSED_W == 1)
            Temp1_Chassis_Speed.vx = 2.5f;
        if(IF_KEY_PRESSED_S == 1)
            Temp1_Chassis_Speed.vx = -2.5f;
        if(IF_KEY_PRESSED_W == 0 && IF_KEY_PRESSED_S == 0)
            Temp1_Chassis_Speed.vx = 0.0f;
        if(IF_KEY_PRESSED_A == 1)
            Temp1_Chassis_Speed.vy = -2.5f;
        if(IF_KEY_PRESSED_D == 1)
            Temp1_Chassis_Speed.vy = 2.5f;
        if(IF_KEY_PRESSED_A == 0 && IF_KEY_PRESSED_D == 0)
            Temp1_Chassis_Speed.vy = 0.0f;
        Temp1_Chassis_Speed.vw = 0;
        break;
    case FOLLOW:
        if(IF_KEY_PRESSED_W == 1)
            Temp1_Chassis_Speed.vx = 2.5f;
        if(IF_KEY_PRESSED_S == 1)
            Temp1_Chassis_Speed.vx = -2.5f;
        if(IF_KEY_PRESSED_W == 0 && IF_KEY_PRESSED_S == 0)
            Temp1_Chassis_Speed.vx = 0.0f;
        if(IF_KEY_PRESSED_A == 1)
            Temp1_Chassis_Speed.vy = -2.5f;
        if(IF_KEY_PRESSED_D == 1)
            Temp1_Chassis_Speed.vy = 2.5f;
        if(IF_KEY_PRESSED_A == 0 && IF_KEY_PRESSED_D == 0)
            Temp1_Chassis_Speed.vy = 0.0f;
		PID_Calc_Angle(&Follow_PID,0.0f,Find_Min_Angle(),8192,0);
        Temp1_Chassis_Speed.vw = Follow_PID.output*0.0007666f;
    default:
        break;
    }
}


/**
 * @file Chassis.c
 * @brief 键盘控制底盘
 * @author HWX
 * @param target_speed 目标设定速度
 * @param XiePo_speed 当前设定速度
 * @date 2025/2/10
 * @note 这里用了两个斜坡函数，一个始终指向目标速度，一个始终指向0，即一个加速斜坡一个减速斜坡，通过动态控制减速斜坡步长控制功率，
 *       通过控制加速斜坡步长（fen部分）实现定功率下小陀螺速度和移动的功率分配
 */
Chassis_Step step_l, step_s;
float  Speed_V1_Fabs, Speed_V_Dif, Speed_W_Fabs,Speed_W_Dif , Speed_V2_Fabs;
void Chassis_Speed_XiePo(Chassis_Speed_t* target_speed, Chassis_Speed_t* XiePo_speed) 
{
    Speed_V1_Fabs = sqrtf(powf(XiePo_speed->vx, 2) + powf(XiePo_speed->vy, 2));
    Speed_V_Dif = sqrtf(powf(target_speed->vx - XiePo_speed->vx, 2) + powf(target_speed->vy - XiePo_speed->vy, 2));

    Speed_W_Fabs = fabs(XiePo_speed->vw);
    Speed_W_Dif = fabs(target_speed->vw - XiePo_speed->vw);

    if(SuperPower_Switch == SuperPower_Work && SuperPower_Mode == SuperPower_Off)//超电开启且不充电
        step_l.t = 0.005f * powf(SuperPower_Rx_Message.Now_power/(Chassis_Power_Limit-5),2);
    else
        step_l.t = 0.005f * powf((60-Chassis_Power_Buffer)/(60-Chassis_Power_Set),2);
	
    if (Speed_W_Fabs > 0.006f) {
        step_l.w = (0 - XiePo_speed->vw) / Speed_W_Fabs * step_l.t;
    }
    else {
        step_l.w = 0;
    }
	
    if (Speed_V1_Fabs > 0.006f) {
        step_l.x = (0.0f - XiePo_speed->vx) / Speed_V1_Fabs * step_l.t * 1.2f;
        step_l.y = (0.0f - XiePo_speed->vy) / Speed_V1_Fabs * step_l.t * 1.2f;
    }
    else {
        step_l.x = 0;
        step_l.y = 0;
    }
    step_s.t = 0.005f;
	
    Speed_V2_Fabs = sqrtf(powf(target_speed->vx, 2) + powf(target_speed->vy, 2));

    float fen;
    if ((Speed_V2_Fabs>0.01f)&&(Speed_V1_Fabs<Speed_V2_Fabs)) 
	{
        fen = 1 * (Speed_V1_Fabs / Speed_V2_Fabs);
    }
    else {
        fen = 1;
    }
	
	if (Speed_W_Dif > 0.006f)
	{
        step_s.w = (target_speed->vw - XiePo_speed->vw) / Speed_W_Dif * step_s.t * fen;
    }
    else {
        step_s.w = 0;
    }
    if (Speed_V_Dif > 0.006f) {
        step_s.x = (target_speed->vx - XiePo_speed->vx) / Speed_V_Dif * step_s.t * 1.2f;
        step_s.y = (target_speed->vy - XiePo_speed->vy) / Speed_V_Dif * step_s.t * 1.2f;
    }
    else {
        step_s.x = 0;
        step_s.y = 0;
    }
    XiePo_speed->vx = XiePo_speed->vx + step_l.x + step_s.x;
    XiePo_speed->vy = XiePo_speed->vy + step_l.y + step_s.y;
    XiePo_speed->vw = XiePo_speed->vw + step_l.w + step_s.w;
	if(target_speed->vw == 0)
	{
		XiePo_speed->vw = 0;
	}
	float bx,by;
	if(Speed_V1_Fabs > 0.002f && Speed_W_Fabs > 0.002f){
        bx = XiePo_speed->vw / 1000 * XiePo_speed->vy;
        by = -XiePo_speed->vw / 1000 * XiePo_speed->vx;
    }else{
        bx = 0;
        by = 0;
    }
    XiePo_speed->vx = XiePo_speed->vx + bx;
    XiePo_speed->vy = XiePo_speed->vy + by;
}
