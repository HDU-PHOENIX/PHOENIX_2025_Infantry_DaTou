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
 * @brief Printf重定向
 * @param ch 这是一个数组，包含了要写入的以空字符终止的字符序列。
 * @param f  这是指向 FILE 对象的指针，该 FILE 对象标识了要被写入字符串的流
 * @return 输出的字符
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
 * @brief 通过串口发送数据，上位机以vofa进行数据处理 
 */
void VOFA_Tx(void) {
    printf("%d\r\n",
           GM6020_Yaw.rotor_angle);
}
