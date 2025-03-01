#include "Shoot.h"

extern RC_t RC;
extern Car_Mode_t Car_Mode;
extern Moto_M2006_t M2006_Rammer;
extern Moto_M3508_t M3508_Shoot[2];
extern Computer_Rx_Message_t Computer_Rx_Message;

static uint8_t Single_Mode,Have_Shoot;
int16_t M3508_Speed=6666;

/********************换弹部分********************/
void Shoot_Reload_Choose(void);

/********************输入控制部分********************/
void Shoot_Remote_Control(void);
void Shoot_KeyBoard_Control(void);

/********************输出控制部分********************/
void Shoot_Stop(void);
void Shoot_Move(void);
void Speed17mm_Control(void);

/********************PID部分********************/
void Shoot_PID_Init_ALL(void);
void Shoot_PID_Clean_ALL(void);
void Shoot_PID_Calc(void);


/**
 * @file Shoot.c
 * @brief 判断是否需要换蛋
 * @author HWX
 * @date 2024/10/20
 */
void Shoot_Reload_Choose(void)
{
    if(RC.wheel <= -300)
    {
        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,2500);
    }else
    {
        __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,850);
    }
}

/**
 * @file Shoot.c
 * @brief 射击控制
 * @author HWX
 * @date 2024/10/20
 */
void Shoot_Move(void)
{
    Shoot_PID_Calc();

    Set_M3508_Shoot_Voltage(&hcan2,M3508_Shoot);
    Set_M2006_Motor_Voltage(&hcan2,M2006_Rammer);

}

/**
 * @file Shoot.c
 * @brief 遥控器判断是否需要开始射击
 * @author HWX
 * @date 2024/10/20
 */
void Shoot_Remote_Control(void)
{
    if(RC.s1 == 2)
    {
        M3508_Shoot[0].Set_Speed = M3508_Speed;
		M3508_Shoot[1].Set_Speed = -M3508_Speed;
    }else{
        M3508_Shoot[0].Set_Speed = 0;
        M3508_Shoot[1].Set_Speed = 0;
        M2006_Rammer.Set_Speed = 0;
    }
    switch (Car_Mode.Shoot)
    {
    case Shoot_Plugins:
        if(RC.s1 == 2 && RC.wheel>=300)
			{					
                M2006_Rammer.Set_Speed = M2006_Speed;
			}else 
			{
                M2006_Rammer.Set_Speed = 0;
			}
        break;
    case Shoot_Single:
        if(RC.wheel >= 300 && Single_Mode == 0 && RC.s1 == 2)
        {
            Have_Shoot = 1;
            Single_Mode = 1;
        }else if(RC.wheel == 0 && Single_Mode == 1)
        {
            Single_Mode = 0;
        }

        if(Have_Shoot == 1) //还未打弹
        {
            if(ABS(M2006_Rammer.total_angle) < MOTOR_2006_CIRCLE_ANGLE / 8.0f) //未转过一个齿位
            {
                M2006_Rammer.Set_Speed = M2006_Speed;
            }
            else
            {
                M2006_Rammer.Set_Speed = 0;
                Have_Shoot = 0;
                M2006_Rammer.total_angle = 0;
            }
        }
        break;
    case Shoot_Sustain:
			if(RC.wheel >= 300 && RC.s1 == 2)
			{
                M2006_Rammer.Set_Speed = M2006_Speed;
			}else 
			{
                M2006_Rammer.Set_Speed = 0;
			}
        break;
    default:
        break;
    }
}

/**
 * @file Shoot.c
 * @brief 计算PID
 * @author HWX
 * @date 2024/10/20
 */
void Shoot_PID_Calc(void)
{
    PID_Calc_Speed(&(M3508_Shoot[0].PID),M3508_Shoot[0].Set_Speed,M3508_Shoot[0].rotor_speed);
    PID_Calc_Speed(&(M3508_Shoot[1].PID),M3508_Shoot[1].Set_Speed,M3508_Shoot[1].rotor_speed);
    PID_Calc_Speed(&(M2006_Rammer.Speed_PID),M2006_Rammer.Set_Speed,M2006_Rammer.rotor_speed);
}

/**
 * @file Shoot.c
 * @brief 初始化PID
 * @author HWX
 * @date 2024/10/20                                                                                                                                                                                                                                                                                                                                                           
 */
void Shoot_PID_Init_ALL(void)
{
    PID_init(&(M3508_Shoot[0].PID),50,5,0,16380,16380);//40,0,0
    PID_init(&(M3508_Shoot[1].PID),50,5,0,16380,16380);//40,0,0
    PID_init(&(M2006_Rammer.Speed_PID),45,0,10,16380,16380);
}

/**
 * @file Shoot.c
 * @brief 清空PID
 * @author HWX
 * @date 2024/10/20
 */
void Shoot_PID_Clean_ALL(void)
{
    PID_init(&(M3508_Shoot[0].PID),0,0,0,0,0);
    PID_init(&(M3508_Shoot[1].PID),0,0,0,0,0);
    PID_init(&(M2006_Rammer.Angle_PID),0,0,0,0,0);
    PID_init(&(M2006_Rammer.Speed_PID),0,0,0,0,0);
}

/**
 * @file Shoot.c
 * @brief 射击断电
 * @author HWX
 * @date 2024/10/20
 */
void Shoot_Stop(void)
{
    Shoot_PID_Calc();
    Set_M3508_Shoot_Voltage(&hcan2,M3508_Shoot);
    Set_M2006_Motor_Voltage(&hcan2,M2006_Rammer);
}

float Speed17mm_Min = 22.0f, Speed17mm_Max = 24.0f;
float Speed17mm_Now ,Speed17mm_Last;
void Speed17mm_Control(void)
{
    Speed17mm_Now = JUDGE_usGetSpeedHeat17();
	if(Speed17mm_Now != Speed17mm_Last)
	{
		Speed17mm_Last = Speed17mm_Now;
		if(Speed17mm_Now >= Speed17mm_Max)
			M3508_Speed -= 500;
		if(Speed17mm_Now <= Speed17mm_Min)
			M3508_Speed += 300;	
	}
}

bool Shoot = false,R_judge = false;
void Shoot_KeyBoard_Control(void)
{
    if(IF_KEY_PRESSED_R == 1)
    {
        if(Shoot == false && R_judge == true)
        {
            Shoot = true;
            R_judge = false;
        }
				if(Shoot == true && R_judge == true)
        {
            Shoot = false;
            R_judge = false;
        }
    }
		if(IF_KEY_PRESSED_R == 0)//松手检测
		{
        R_judge = true;
		}

    if(Shoot == true)
    {
        M3508_Shoot[0].Set_Speed = M3508_Speed;//6688
        M3508_Shoot[1].Set_Speed = -M3508_Speed;//-6688
    }else if(Shoot == false)
    {
        M3508_Shoot[0].Set_Speed = 0;
        M3508_Shoot[1].Set_Speed = 0;
        M2006_Rammer.Set_Speed = 0;
    }

    switch (Car_Mode.Shoot)
    {
    case Shoot_Plugins:
        if(Shoot == true && RC.mouse.press_l == 1)
			{					
                M2006_Rammer.Set_Speed = M2006_Speed;
			}else 
			{
                M2006_Rammer.Set_Speed = 0;
			}
        break;
    case Shoot_Single:
        if(Shoot == true && RC.mouse.press_l == 1 && Single_Mode == 0)
        {
            Have_Shoot = 1;
            Single_Mode = 1;
        }else if(RC.mouse.press_l == 0 && Single_Mode == 1)
        {
            Single_Mode = 0;
        }

        if(Have_Shoot == 1) //还未打弹
        {
            if(ABS(M2006_Rammer.total_angle) < MOTOR_2006_CIRCLE_ANGLE / 8.0f) //未转过一个齿位
            {
                M2006_Rammer.Set_Speed = M2006_Speed;
            }
            else
            {
                M2006_Rammer.Set_Speed = 0;
                Have_Shoot = 0;
                M2006_Rammer.total_angle = 0;
            }
        }
        break;
    case Shoot_Sustain:
			if(Shoot == true && RC.mouse.press_l == 1)
			{
                M2006_Rammer.Set_Speed = M2006_Speed;
			}else 
			{
                M2006_Rammer.Set_Speed = 0;
			}
        break;
    default:
        break;
    }
}

