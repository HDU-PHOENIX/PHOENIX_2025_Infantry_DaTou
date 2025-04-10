#include "Gimbal.h"


extern float IMU_angle[3];
extern Moto_GM6020_t GM6020_Yaw,GM6020_Pitch;
extern Car_Mode_t Car_Mode;
extern RC_t RC;
extern Gimbal_Add_t Gimbal_Add;
extern float Set_Yaw,Set_Pitch,ZiMiao_Add_Yaw,ZiMiao_Add_Pitch;
extern Computer_Rx_Message_t Computer_Rx_Message;
//����������
float Yaw_ZiMiao_Filter[2]={0.025,0.975};
float Pitch_ZiMiao_Filter[2]={0.01,0.99};

/********************������Ʋ���********************/
void Gimbal_Remote_Control(void);
void Gimbal_KeyBorad_Control(void);
void Gimbal_FastTurn(void);

/********************���㲿��********************/
void Gimbal_Calculate(void);
void Gimbal_Init(void);

/********************������Ʋ���********************/
void Gimbal_Move(void);
void Gimbal_Stop(void);

/********************PID���Ʋ���********************/
void Gimbal_PID_Init_All(void);
void Gimbal_PID_Clean_All(void);
void Gimbal_PID_Calc(void);

/**
 * @file Gimbal.c
 * @brief ��̨��ʼ��
 * @author HWX
 * @date 2024/10/20
 */
void Gimbal_Init(void)
{
    Set_Pitch = 0.0f;
    Set_Yaw = IMU_angle[0];
}

/**
 * @file Gimbal.c
 * @brief ����Ŀ��ֵ
 * @author HWX
 * @date 2024/10/20
 */
void Gimbal_Calculate(void)
{
    //Yaw
    switch(Car_Mode.Shoot)
    {
		case Shoot_Sustain:
        Set_Yaw -= Gimbal_Add.Yaw;
        Set_Pitch += Gimbal_Add.Pitch;
        break;
    case Shoot_Single:
		if(Computer_Rx_Message.find_bool == '1')
		{
            Set_Yaw=Computer_Rx_Message.yaw*Yaw_ZiMiao_Filter[0]+Set_Yaw*Yaw_ZiMiao_Filter[1];
            Set_Pitch=Computer_Rx_Message.pitch*Pitch_ZiMiao_Filter[0]+Set_Pitch*Pitch_ZiMiao_Filter[1];
		}else
		{
            Set_Yaw -= Gimbal_Add.Yaw;
            Set_Pitch += Gimbal_Add.Pitch;
		}
        break;
    case Shoot_Plugins://װ�װ�����
		if(Computer_Rx_Message.find_bool == '1')
		{
            Set_Yaw = Computer_Rx_Message.yaw*Yaw_ZiMiao_Filter[0]+Set_Yaw*Yaw_ZiMiao_Filter[1];
            Set_Pitch = Computer_Rx_Message.pitch*Pitch_ZiMiao_Filter[0]+Set_Pitch*Pitch_ZiMiao_Filter[1];
            ZiMiao_Add_Yaw -= Gimbal_Add.Yaw/1000;
            ZiMiao_Add_Pitch += Gimbal_Add.Pitch/1500;
            Set_Yaw += ZiMiao_Add_Yaw;
            Set_Pitch += ZiMiao_Add_Pitch;
		}else
		{
            Set_Yaw -= Gimbal_Add.Yaw;
            Set_Pitch += Gimbal_Add.Pitch;
		}
        break;

    }
    while(Set_Yaw > 180)
    {
        Set_Yaw -= 360;
    }
    while(Set_Yaw < -180)
    {
        Set_Yaw += 360;
    }
    while(Set_Pitch > Gimbal_Pitch_MAX)
    {
        Set_Pitch = Gimbal_Pitch_MAX;
    }
    while(Set_Pitch < Gimbal_Pitch_MIN)
    {
        Set_Pitch = Gimbal_Pitch_MIN;
    }
}
/**
 * @file Gimbal.c
 * @brief ��̨��ʼ�˶�
 * @author HWX
 * @date 2024/10/20
 */
void Gimbal_Move(void)
{
    Gimbal_Calculate();

    Gimbal_PID_Calc();

    Set_GM6020_Gimbal_Voltage(&hcan1,GM6020_Yaw,GM6020_Pitch);
}

/**
 * @file Gimbal.c
 * @brief ��̨�ϵ�
 * @author HWX
 * @date 2024/10/20
 */
void Gimbal_Stop(void)
{
    Gimbal_PID_Calc();
    Set_GM6020_Gimbal_Voltage(&hcan1,GM6020_Yaw,GM6020_Pitch);
}

/**
 * @file Gimbal.c
 * @brief ң����������̨
 * @author HWX
 * @date 2024/10/20
 */
void Gimbal_Remote_Control(void)
{
	Gimbal_Add.Pitch = (float)RC.ch1/Gimbal_Pithch_Remote;
	Gimbal_Add.Yaw = (float)RC.ch0/Gimbal_Yaw_Remote;
}

/**
 * @file Gimbal.c
 * @brief ��̨PID����
 * @author HWX
 * @date 2024/10/20
 */
void Gimbal_PID_Calc(void)
{
    //Yaw
    PID_Calc_Angle(&GM6020_Yaw.Angle_PID,Set_Yaw,IMU_angle[0],360,0);
    GM6020_Yaw.Set_Speed = GM6020_Yaw.Angle_PID.output;
    PID_Calc_Speed(&(GM6020_Yaw.Speed_PID),GM6020_Yaw.Set_Speed,GM6020_Yaw.rotor_speed);
    //Pitch
    PID_Calc_Angle(&GM6020_Pitch.Angle_PID,Set_Pitch,IMU_angle[2],360,0);
    GM6020_Pitch.Set_Speed = -GM6020_Pitch.Angle_PID.output;
    PID_Calc_Speed(&GM6020_Pitch.Speed_PID,GM6020_Pitch.Set_Speed,GM6020_Pitch.rotor_speed);
}

/**
 * @file Gimbal.c
 * @brief ��̨���PID��ʼ��
 * @author HWX
 * @date 2024/10/20
 */
void Gimbal_PID_Init_All(void)
{
	Gimbal_Init();

	PID_init(&GM6020_Yaw.Angle_PID,60,0,1800,16380,25000);//60,0,2500//80,0,1600//60,0,1800//10,0,200//20,0,200//25,0,700
	//110,0.05,830//110,0.045,800
    PID_init(&(GM6020_Yaw.Speed_PID),180,1.3,0,16380,25000);//200,1.3.0//195,1.3,0//142,2,0//185,1.3,0
	//80,0.45,150//80,0.45,150
	
    PID_init(&(GM6020_Pitch.Angle_PID),30,0,1000,16500,25000);//30,0,1000//35,0,700
    //35,0,1000//30,0,1000
    PID_init(&(GM6020_Pitch.Speed_PID),110,1,0,16500,25000);//130,1,0//155,2.8,0
    //120,1,0//130,1,0
}

/**
 * @file Gimbal.c
 * @brief ��̨���PID���
 * @author HWX
 * @date 2024/10/20
 */
void Gimbal_PID_Clean_All(void)
{
    PID_init(&(GM6020_Pitch.Angle_PID),0,0,0,0,0);
    PID_init(&(GM6020_Pitch.Speed_PID),0,0,0,0,0);
    PID_init(&(GM6020_Yaw.Angle_PID),0,0,0,0,0);
    PID_init(&(GM6020_Yaw.Speed_PID),0,0,0,0,0);
}

bool fastturn = false, Q_judge = false;
float Fastturn_Yaw, Temp_Yaw;
void Gimbal_FastTurn(void)
{
		
    if(IF_KEY_PRESSED_Q == 1 && Q_judge == true)
    {
        fastturn = true;
        Fastturn_Yaw = Set_Yaw + 180;
				while(Fastturn_Yaw > 180)
				{
					Fastturn_Yaw -= 360;
				}
				while(Fastturn_Yaw < -180)
				{
					Fastturn_Yaw += 360;
				}
					Temp_Yaw = Fastturn_Yaw;
					Q_judge = false;
		}
				if(IF_KEY_PRESSED_Q == 0)
				{
					Q_judge = true;
				}
    if(fastturn == true)
    {
        Set_Yaw=Fastturn_Yaw*0.01f+Set_Yaw*0.99f;
		if(fabs(Set_Yaw - Temp_Yaw)<5)
		{
			fastturn = false;
		}
    }

}

void Gimbal_KeyBorad_Control(void)
{
    if(fastturn == false)
    {
        Gimbal_Add.Pitch = -(float)RC.mouse.y / Gimbal_Pitch_KeyBoard;
        Gimbal_Add.Yaw = (float)RC.mouse.x / Gimbal_Yaw_KeyBoard;
    }
}

