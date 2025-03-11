#include "bsp_can.h"

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
    can2_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    can2_filter_st.FilterActivation = ENABLE;
    can2_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can2_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can2_filter_st.FilterBank = 0;
    can2_filter_st.SlaveStartFilterBank = 0;
		//使能CAN通道
    HAL_CAN_ConfigFilter(&hcan1, &can1_filter_st);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(&hcan1);
    HAL_Delay(10);
    HAL_CAN_ConfigFilter(&hcan2, &can2_filter_st);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_Start(&hcan2);
}

/**
 * @file BSP_Can.c
 * @brief CAN接受中断函数
 * @param hcan CAN通道
 * @author HWX
 * @date 2024/10/20
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
	if(hcan->Instance == CAN1)
    {
      if(rx_header.StdId == 0x207)
      {
          Get_GM6020_Motor_Message(rx_header.StdId,rx_data);
      }else if(rx_header.StdId >= 0x201 && rx_header.StdId <= 0x204)
      {
          Get_M3508_Chassis_Message(rx_header.StdId,rx_data);
      }else if(rx_header.StdId == 0x0C2)
	  {
		  SuperPower_Rx(rx_data);
	  }
    }else if(hcan->Instance == CAN2)
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

