#include "ChassisTask.h"

extern Car_Mode_t Car_Mode;
extern float Chassis_Power_Limit,Chassis_Power_Now,Chassis_Power_Buffer;

void ChassisTask(void const * argument)
{
    portTickType currentTime;
    SuperPower_Init();
//    uint8_t voft_cnt=0;
    while (1)   
    { 
        currentTime = xTaskGetTickCount();//��ǰϵͳʱ��
        Control_Mode_Choose();//ѡ��ģʽ
        Chassis_Power_Limit=JUDGE_usGetPowerLimit();
        Chassis_Power_Buffer=JUDGE_fGetRemainEnergy();
//         voft_cnt++;
//         if(voft_cnt>=10)
//         {
//             voft_cnt =0;
//             VOFT_Tx();            
//         }
        SuperPower_Switch_Control();
switch (Car_Mode.State)
        {
        case Car_Remote:
			Chassis_Remote_Control();
            Chassis_Move();
            break;
        case Car_Keyboard:
            SuperPower_KeyBoard_Control();
            Chassis_KeyBoard_Control();
            Chassis_Move();
            break;
        case Car_Stop:
            Chassis_Stop();
            break;
        default:
            break;
        }
        SuperPower_Tx();
        vTaskDelayUntil(&currentTime,2);//������ʱ
    }

}

