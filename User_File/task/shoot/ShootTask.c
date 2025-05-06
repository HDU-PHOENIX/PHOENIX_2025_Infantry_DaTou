#include "ShootTask.h"

extern Car_Mode_t Car_Mode;
extern Moto_M3508_t M3508_Shoot[2];

void ShootTask(void const * argument)
{
    portTickType currentTime;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,850);//850
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    M3508_Shoot[0].Set_Speed = 0;
	M3508_Shoot[1].Set_Speed = 0;
    while(1)
    { 
        currentTime = xTaskGetTickCount();//当前系统时间
        Shoot_Reload_Choose();
        Speed17mm_Control();
        switch (Car_Mode.State)
        {
        case Car_Remote:
 		    Shoot_Remote_Control();
            Shoot_Move();
            break;
        case Car_Keyboard:
            Shoot_KeyBoard_Control();
            Shoot_Move();
            break;
        case Car_Stop:
            Shoot_Stop();
            break;
        default:
            break;
        }
        vTaskDelayUntil(&currentTime,2);//绝对延时
    }
}
