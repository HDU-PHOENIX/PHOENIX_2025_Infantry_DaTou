#include "Shoot.h"

extern RC_t RC;
extern Car_Mode_t Car_Mode;
extern Moto_M2006_t M2006_Rammer;
extern Moto_M3508_t M3508_Shoot[2];
extern Computer_Rx_Message_t Computer_Rx_Message;

static uint8_t Single_Mode,Have_Shoot;
int16_t M3508_Speed=6666;
bool Shoot = false;
float M2006_Speed = -1800;

/********************��������********************/
void Shoot_Reload_Choose(void);

/********************������Ʋ���********************/
void Shoot_Remote_Control(void);
void Shoot_KeyBoard_Control(void);

/********************������Ʋ���********************/
void Shoot_Stop(void);
void Shoot_Move(void);
void Speed17mm_Control(void);

/********************PID����********************/
void Shoot_PID_Init_ALL(void);
void Shoot_PID_Clean_ALL(void);
void Shoot_PID_Calc(void);


/**
 * @file Shoot.c
 * @brief �ж��Ƿ���Ҫ����
 * @author HWX
 * @date 2024/10/20
 */
void Shoot_Reload_Choose(void)
{
	switch (Car_Mode.State)
	{
	case Car_Remote:
		if(RC.wheel <= -300)
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,2500);
		}else
		{
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,850);
		}
		break;
	case Car_Keyboard:
//��סR���ص���		
        if(IF_KEY_PRESSED_R == 1)
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,2500);
		else if(IF_KEY_PRESSED_R == 0)
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,850);

		break;
	case Car_Stop:
		break;
	default:
		break;
    }
}

/**
 * @file Shoot.c
 * @brief �������
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
 * @brief ң�����ж��Ƿ���Ҫ��ʼ���
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
    case Shoot_Single://����ģʽ
        if(RC.wheel >= 300 && Single_Mode == 0 && RC.s1 == 2)
        {
            Have_Shoot = 1;
            Single_Mode = 1;
        }else if(RC.wheel == 0 && Single_Mode == 1)
        {
            Single_Mode = 0;
        }
        if(Have_Shoot == 1) //��δ��
        {
            if(ABS(M2006_Rammer.total_angle)< MOTOR_2006_CIRCLE_ANGLE /8.0f) //δת��һ����λ
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
 * @brief ����PID
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
 * @brief ��ʼ��PID
 * @author HWX
 * @date 2024/10/20                                                                                                                                                                                                                                                                                                                                                           
 */
void Shoot_PID_Init_ALL(void)
{
    PID_init(&(M3508_Shoot[0].PID),75,0,80,16380,16380);//40,0,0//45,5,0//10,0.8,2//75,0,80
    PID_init(&(M3508_Shoot[1].PID),75,0,80,16380,16380);//40,0,0//45,5,0/10,0.8,2//68,0,87
    PID_init(&(M2006_Rammer.Speed_PID),10,0.02,0,16380,16380);//10,4,0//8,0.16,4.15
}

/**
 * @file Shoot.c
 * @brief ���PID
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
 * @brief ����ϵ�
 * @author HWX
 * @date 2024/10/20
 */
void Shoot_Stop(void)
{
    Shoot_PID_Calc();
    Set_M3508_Shoot_Voltage(&hcan2,M3508_Shoot);
    Set_M2006_Motor_Voltage(&hcan2,M2006_Rammer);
}

/**
 * @brief ͨ��������Ƶ���
 */
float Speed17mm_Now;
uint8_t Min_cnt = 0,Max_cnt = 0;
bool Z_judge = false,X_judge = false,G_judge = false;
void Speed17mm_Control(void)
{
    Speed17mm_Now = JUDGE_usGetSpeedHeat17();
    if(IF_KEY_PRESSED_Z == 1)//���Z������
    {
        if(Z_judge == true)
        {
            M3508_Speed -= 100;
            Z_judge = false;
		}
    }
	if(IF_KEY_PRESSED_Z == 0)//���ּ��
	{
        Z_judge = true;
	}
	
	if(IF_KEY_PRESSED_X == 1)//���X�ӵ���
    {
        if(X_judge == true)
        {
            M3508_Speed += 100;
            X_judge = false;
        }
    }
	if(IF_KEY_PRESSED_X == 0)//���ּ��
	{
        X_judge = true;
	}
	
	if(IF_KEY_PRESSED_G == 1)//���G�ظ�����ʼ�趨ֵ
    {
        if(G_judge == true)
        {
            M3508_Speed = 6666;
            G_judge = false;
        }
    }
	if(IF_KEY_PRESSED_G == 0)//���ּ��
	{
        G_judge = true;
	}
}

/**
 * @brief ��������Ƿ����Լ�����Ħ����
 */
bool CTRL_judge = false;
void Shoot_KeyBoard_Control(void)
{

    if(IF_KEY_PRESSED_CTRL == 1)//���ctrl����Ħ����
    {
        if(Shoot == false && CTRL_judge == true)
        {
            Shoot = true;
            CTRL_judge = false;
        }
				if(Shoot == true && CTRL_judge == true)
        {
            Shoot = false;
            CTRL_judge = false;
        }
    }
		if(IF_KEY_PRESSED_CTRL == 0)//���ּ��
		{
        CTRL_judge = true;
		}

    if(Shoot == true)
    {
        M3508_Shoot[0].Set_Speed = M3508_Speed;
        M3508_Shoot[1].Set_Speed = -M3508_Speed;
    }else if(Shoot == false)
    {
        M3508_Shoot[0].Set_Speed = 0;
        M3508_Shoot[1].Set_Speed = 0;
        M2006_Rammer.Set_Speed = 0;
    }
    switch (Car_Mode.Shoot)
    {
    case Shoot_Plugins:
        if(Shoot == true && RC.mouse.press_l == 1)//Ħ���ֿ������������
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

        if(Have_Shoot == 1) //��δ��
        {
            if(ABS(M2006_Rammer.total_angle)<MOTOR_2006_CIRCLE_ANGLE / 8.0f) //δת��һ����λ
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

