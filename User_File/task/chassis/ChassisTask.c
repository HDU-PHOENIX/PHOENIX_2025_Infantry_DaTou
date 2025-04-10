#include "ChassisTask.h"

extern Car_Mode_t Car_Mode;
extern float Chassis_Power_Limit,Chassis_Power_Now,Chassis_Power_Buffer;

void ChassisTask(void const * argument)
{
    portTickType currentTime;
    SuperCap_Init();

    while (1)   
    { 
        currentTime = xTaskGetTickCount();//��ǰϵͳʱ��
        Control_Mode_Choose();//ѡ��ģʽ
        Chassis_Power_Limit=JUDGE_usGetPowerLimit();
        Chassis_Power_Buffer=JUDGE_fGetRemainEnergy();
        SuperCap_Switch_Control();
switch (Car_Mode.State)
        {
        case Car_Remote:
			Chassis_Remote_Control();
            Chassis_Move();
            break;
        case Car_Keyboard:
            SuperCap_KeyBoard_Control();
            Chassis_KeyBoard_Control();
            Chassis_Move();
            break;
        case Car_Stop:
            Chassis_Stop();
            break;
        default:
            break;
        }
        SuperCap_Tx();
//		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_11);
        vTaskDelayUntil(&currentTime,1);//������ʱ
    }

}

