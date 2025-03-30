#include "GimbalTask.h"

extern Car_Mode_t Car_Mode;

void GimbalTask(void const * argument)
{
    portTickType currentTime;
    while(1)
    {
        currentTime = xTaskGetTickCount();//��ǰϵͳʱ��
//        Computer_Rx();//��ȡ��������
        switch (Car_Mode.State)
        {

        case Car_Remote:
            Gimbal_Remote_Control();
            Gimbal_Move();
            break;
        case Car_Stop:
            Gimbal_Stop();
            break;
        case Car_Keyboard:
            Gimbal_KeyBorad_Control();
            Gimbal_FastTurn();
            Gimbal_Move();
        default:
            break;
        }
        vTaskDelayUntil(&currentTime,1);//������ʱ
    }
}
