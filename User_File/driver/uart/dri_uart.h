#ifndef __BSP_UART_H__
#define __BSP_UART_H__

extern uint8_t VOFT_Data[20];
extern float Set_Yaw,Set_Pitch;
extern float IMU_angle[3];
extern Moto_GM6020_t GM6020_Yaw,GM6020_Pitch;
extern Chassis_Speed_t Temp1_Chassis_Speed,Temp2_Chassis_Speed;
extern Moto_M3508_t M3508_Chassis[4];
extern Moto_M2006_t M2006_Rammer;

void VOFA_Tx(void);

#endif


