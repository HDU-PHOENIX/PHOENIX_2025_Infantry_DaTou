#include "ChassisTask.h"

extern Car_Mode_t Car_Mode;
extern float Chassis_Power_Limit,Chassis_Power_Now,Chassis_Power_Buffer;

void ChassisTask(void const * argument)
{
    portTickType currentTime;
    while (1)   
    { 
        currentTime = xTaskGetTickCount();//当前系统时间
        Control_Mode_Choose();//选择模式
        Chassis_Power_Now=JUDGE_fGetChassisPower();
        Chassis_Power_Limit=JUDGE_usGetPowerLimit();
        Chassis_Power_Buffer=JUDGE_fGetRemainEnergy();
        switch (Car_Mode.State)
        {
        case Car_Remote:
			Chassis_Remote_Control();
            Chassis_Move();
            break;
        case Car_Keyboard:
            Chassis_KeyBoard_Control();
            Chassis_Move();
            break;
        case Car_Stop:
            Chassis_Stop();
            break;
                
        default:
            break;
        }
        vTaskDelayUntil(&currentTime,2);//绝对延时
    }

}

