#include "dri_uart.h"


extern uint8_t VOFT_Data[20];
extern float Set_Yaw,Set_Pitch;
extern float IMU_angle[3];
extern Moto_GM6020_t GM6020_Yaw,GM6020_Pitch;
extern Chassis_Speed_t Temp1_Chassis_Speed,Temp2_Chassis_Speed;
extern Moto_M3508_t M3508_Chassis[4];

#define TX_BUFFER_SIZE 256
uint8_t txBuffer[TX_BUFFER_SIZE];
volatile uint16_t txLength = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3){
			RC_Processing_Data();
	}
}

/**
 * @brief Printf�ض���
 * @param ch ����һ�����飬������Ҫд����Կ��ַ���ֹ���ַ����С�
 * @param f  ����ָ�� FILE �����ָ�룬�� FILE �����ʶ��Ҫ��д���ַ�������
 * @return ������ַ�
 */
int fputc(int ch, FILE *f)
{
    if(txLength < TX_BUFFER_SIZE-1){
        txBuffer[txLength++] = (uint8_t)ch;
	}
	if(ch == '\n'){
		HAL_UART_Transmit_DMA(&huart1,txBuffer,txLength);
		txLength=0;
	}
    return ch;
}

/**
 * @brief ͨ�����ڷ������ݣ���λ����vofa�������ݴ��� 
 */
void VOFA_Tx(void) {
    printf("%d\r\n",
           GM6020_Yaw.rotor_angle);
}
