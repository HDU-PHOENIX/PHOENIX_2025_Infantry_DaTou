//#include "VOFT_Uartx.h"

//extern uint8_t VOFT_Data[20];
//extern float Set_Yaw,Set_Pitch;
//extern float IMU_angle[3];
//extern Moto_GM6020_t GM6020_Yaw,GM6020_Pitch;


//uint8_t T_pData[20] = {0};
//uint8_t R_pData[20] = {0};

//int fputc(int ch, FILE *f)

//{
//    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1,0xffff);
//    return ch;
//}


//void VOFT_Tx(void)
//{
//	printf("%f,%f,%f,%f\n",Set_Yaw,IMU_angle[0],Set_Pitch,IMU_angle[2]);
//}

////float Get_Data(void)
////{
////	float data_Integer = 0;//整数部分
////	float data_Decimals = 0;//小数部分
////    uint8_t data_Start_Num = 0; // 记录数据位开始的地方
////    uint8_t data_End_Num = 0; // 记录数据位结束的地方
////	uint8_t data_Point_Num = 0;//记录数据中小数点的位置
////    uint8_t data_Integer_Num = 0; // 记录整数位数
////    uint8_t data_Decimals_Num = 0; // 记录小数位数
////    uint8_t minus_Flag = 0; // 判断是不是负数
////    float data_return = 0; // 解析得到的数据
////    for(uint8_t i=0;i<20;i++) // 查找等号和感叹号的位置
////    {
////        if(VOFT_Data[i] == '=')
////			data_Start_Num = i + 1; // +1是直接定位到数据起始位
////        if(VOFT_Data[i] == '!')
////        {
////            data_End_Num = i - 1;
////            break;
////        }
////		if(VOFT_Data[i] == '.')
////			data_Point_Num = i;
////    }
////    if(VOFT_Data[data_Start_Num] == '-') // 如果是负数
////    {
////        data_Start_Num += 1; // 后移一位到数据位
////        minus_Flag = 1; // 负数flag
////    }
////	
////	data_Integer_Num = data_Point_Num - data_Start_Num;
////	data_Decimals_Num = data_End_Num - data_Point_Num;

////    if(minus_Flag == 1)  data_return = -data_return;
////    return data_return;
////}
