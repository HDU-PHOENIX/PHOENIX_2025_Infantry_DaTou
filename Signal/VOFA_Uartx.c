#include "VOFA_Uartx.h"

extern uint8_t VOFA_Data[20];
extern float Set_Yaw,Set_Pitch;
extern float IMU_angle[3];
extern Moto_GM6020_t GM6020_Yaw,GM6020_Pitch;
extern Chassis_Speed_t Temp1_Chassis_Speed,Temp2_Chassis_Speed;
extern Moto_M3508_t M3508_Chassis[4];
extern Moto_M2006_t M2006_Rammer;

#define TX_BUFFER_SIZE 32
uint8_t txBuffer[TX_BUFFER_SIZE];  // ���ͻ�����
volatile uint16_t txLength = 0;    // ��ǰ���������ݳ���

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
    printf("%d,%d,%d,%d\r\n",
        M3508_Chassis[0].torque_current,
        M3508_Chassis[1].torque_current,
        M3508_Chassis[2].torque_current,
        M3508_Chassis[3].torque_current);
}
