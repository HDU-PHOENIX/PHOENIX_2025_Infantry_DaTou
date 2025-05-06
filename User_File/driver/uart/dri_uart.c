#include "dri_uart.h"

#include "main.h"
#include "dvc_dr16.h"
#include "dvc_minipc.h"
#include "Gimbal.h"
#include "Usart.h"
#include "dvc_dji_gm6020.h"
#include <stdio.h>
#include <string.h>

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
