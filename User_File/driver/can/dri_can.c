/**
 * @file dri_can.c
 * @brief CAN
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @date 2025-4-13
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */

#include "dri_can.h"

#include "main.h"
#include "can.h"
#include "stm32f4xx.h"
#include "dvc_dji_gm6020.h"
#include "dvc_dji_m3508.h"
#include "dvc_dji_m2006.h"
#include "dvc_supercap.h"

/**
 * @file BSP_Can.c
 * @brief 初始化筛选器（这里显码和掩码都是0x0000）
 * @author HWX
 * @date 2024/10/20

 */
void CAN_Filter_Init(void)
{
    CAN_FilterTypeDef can1_filter_st,can2_filter_st;
	
    can1_filter_st.FilterIdHigh = 0x0000;
    can1_filter_st.FilterIdLow = 0x0000;
    can1_filter_st.FilterMaskIdHigh = 0x0000;
    can1_filter_st.FilterMaskIdLow = 0x0000;
    can1_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    can1_filter_st.FilterActivation = ENABLE;
    can1_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can1_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can1_filter_st.FilterBank = 0;
    can1_filter_st.SlaveStartFilterBank = 14;
	
    can2_filter_st.FilterIdHigh = 0x0000;
    can2_filter_st.FilterIdLow = 0x0000;
    can2_filter_st.FilterMaskIdHigh = 0x0000;
    can2_filter_st.FilterMaskIdLow = 0x0000;
    can2_filter_st.FilterFIFOAssignment = CAN_RX_FIFO1;
    can2_filter_st.FilterActivation = ENABLE;
    can2_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can2_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can2_filter_st.FilterBank = 14;
    can2_filter_st.SlaveStartFilterBank = 14;
	//使能CAN通道
    if (HAL_CAN_ConfigFilter(&hcan1, &can1_filter_st) != HAL_OK)// 配置 CAN1 过滤器
    {
        Error_Handler();  // 处理错误
    }
    if (HAL_CAN_Start(&hcan1) != HAL_OK)// 启动 CAN1
    {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)// 使能 CAN1 接收 FIFO0 消息中断
    {
        Error_Handler();
    }
    HAL_Delay(10);
    if (HAL_CAN_ConfigFilter(&hcan2, &can2_filter_st) != HAL_OK)    // 配置 CAN2 过滤器
    {
        Error_Handler();
    }
    if (HAL_CAN_Start(&hcan2) != HAL_OK)// 启动 CAN2
    {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)// 使能 CAN2 接收 FIFO1 消息中断
    {
        Error_Handler();
    }}

/**
 * @file BSP_Can.c
 * @brief CAN接受中断函数
 * @param hcan CAN通道
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
	if(hcan->Instance == CAN1){
    if(rx_header.StdId == 0x207){
        Get_GM6020_Motor_Message(rx_header.StdId,rx_data);
    }else if(rx_header.StdId >= 0x201 && rx_header.StdId <= 0x204){
        Get_M3508_Chassis_Message(rx_header.StdId,rx_data);
    }else if(rx_header.StdId == 0x0C2){
        SuperCap_Rx(rx_data);
	}
    }
}

/**
 * @file BSP_Can.c
 * @brief CAN接受中断函数
 * @param hcan CAN通道
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header, rx_data);
    if(hcan->Instance == CAN2)
    {
        if(rx_header.StdId == 0x201)
			{
				Get_M2006_Motor_Message(rx_header.StdId,rx_data);
			}else if(rx_header.StdId == 0x203 || rx_header.StdId == 0x204)
			{
				Get_M3508_Shoot_Message(rx_header.StdId,rx_data);
			}else if(rx_header.StdId == 0x205)
			{
				Get_GM6020_Motor_Message(rx_header.StdId,rx_data);
			}
		}
}
