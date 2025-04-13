#include "dri_uart.h"

extern uint8_t VOFT_Data[20];
extern float Set_Yaw,Set_Pitch;
extern float IMU_angle[3];
extern Moto_GM6020_t GM6020_Yaw,GM6020_Pitch;
extern Chassis_Speed_t Temp1_Chassis_Speed,Temp2_Chassis_Speed;
extern Moto_M3508_t M3508_Chassis[4];
extern Moto_M2006_t M2006_Rammer;

#define TX_BUFFER_SIZE 256
uint8_t txBuffer[TX_BUFFER_SIZE];  // 发送缓冲区
volatile uint16_t txLength = 0;    // 当前缓冲区数据长度

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
       if(huart->Instance == USART3)
	{
			RC_Processing_Data();
	}
}

int fputc(int ch, FILE *f)
{
    if(txLength < TX_BUFFER_SIZE-1)
	{
        txBuffer[txLength++] = (uint8_t)ch;
	}
	if(ch == '\n')
	{
		HAL_UART_Transmit_DMA(&huart1,txBuffer,txLength);
		txLength=0;
	}
    return ch;
}

void VOFA_Tx(void) {
    printf("%f,%f,%f\r\n",
           GM6020_Yaw.Angle_PID.p_out,
	       GM6020_Yaw.Angle_PID.d_out,
	       GM6020_Yaw.Angle_PID.f_out);
}
