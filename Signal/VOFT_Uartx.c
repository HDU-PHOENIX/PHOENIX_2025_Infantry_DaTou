#include "VOFT_Uartx.h"

extern uint8_t VOFT_Data[20];
extern float Set_Yaw,Set_Pitch;
extern float IMU_angle[3];
extern Moto_GM6020_t GM6020_Yaw,GM6020_Pitch;
extern Chassis_Speed_t Temp1_Chassis_Speed,Temp2_Chassis_Speed;
extern Moto_M3508_t M3508_Chassis[4];
extern Moto_M2006_t M2006_Rammer;

uint8_t T_pData[20] = {0};
uint8_t R_pData[20] = {0};

int fputc(int ch, FILE *f)

{
    HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xffff);
    return ch;
}


void VOFT_Tx(void)
{
	printf("%d,%d,%d,%d,%f\n",M3508_Chassis[0].torque_current,M3508_Chassis[1].torque_current,M3508_Chassis[2].torque_current,M3508_Chassis[3].torque_current,Temp2_Chassis_Speed.vx);
}

