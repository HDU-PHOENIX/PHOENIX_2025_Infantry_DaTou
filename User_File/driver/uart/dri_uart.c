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
uint8_t txBuffer[TX_BUFFER_SIZE];  // å‘é€ç¼“å†²åŒº
volatile uint16_t txLength = 0;    // å½“å‰ç¼“å†²åŒºæ•°æ®é•¿åº¦

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3){
			RC_Processing_Data();
	}
}

/**
 * @brief PrintfÖØ¶¨Ïò
 * @param ch ÕâÊÇÒ»¸öÊı×é£¬°üº¬ÁËÒªĞ´ÈëµÄÒÔ¿Õ×Ö·ûÖÕÖ¹µÄ×Ö·ûĞòÁĞ¡£
 * @param f  ÕâÊÇÖ¸Ïò FILE ¶ÔÏóµÄÖ¸Õë£¬¸Ã FILE ¶ÔÏó±êÊ¶ÁËÒª±»Ğ´Èë×Ö·û´®µÄÁ÷
 * @return Êä³öµÄ×Ö·û
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
 * @brief Í¨¹ı´®¿Ú·¢ËÍÊı¾İ£¬ÉÏÎ»»úÒÔvofa½øĞĞÊı¾İ´¦Àí 
 */
void VOFA_Tx(void) {
    printf("%d\r\n",
           GM6020_Yaw.rotor_angle);
}
