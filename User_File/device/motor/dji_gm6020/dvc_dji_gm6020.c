/**
 * @file dvc_dji_gm6020.c
 * @brief djigm6020电机控制函数
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @date 2025-4-13
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */
#include "dvc_dji_gm6020.h"

#include "main.h"
#include "can.h"
#include "stm32f4xx.h"
#include "alg_pid.h"
HAL_StatusTypeDef can_1;
HAL_StatusTypeDef can_2;

/**
<<<<<<< HEAD
 * @brief GM6020接受反馈报文函数
 * @param StdId 电机ID
 * @param rx_data CAN通道收到的数据
=======
 * @file GM6020.c
 * @brief GM6020鎺ュ彈鍙嶉鎶ユ枃鍑芥暟
 * @param StdId 鐢垫満ID
 * @param rx_data CAN閫氶亾鏀跺埌鐨勬暟鎹�
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void Get_GM6020_Motor_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//鎺ユ敹鎸囧畾鐢垫満鍙嶉鐨勪俊鎭�
    {
        case 0x207://鍙嶉鎶ユ枃鏍囪瘑绗�
        {
            GM6020_Yaw.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//鎺ユ敹鏈烘瑙掑害锛�16bit锛�
            GM6020_Yaw.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//鎺ユ敹杞�燂紙16bit锛�
            GM6020_Yaw.torque_current = ((rx_data[4] << 8) | rx_data[5]);//鎺ユ敹瀹為檯杞煩
            GM6020_Yaw.temp           =   rx_data[6];//鎺ユ敹鐢垫満娓╁害锛�8bit锛�
            break;
        }
        case 0x205://鍙嶉鎶ユ枃鏍囪瘑绗�
        {
            GM6020_Pitch.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//鎺ユ敹鏈烘瑙掑害锛�16bit锛�
            GM6020_Pitch.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//鎺ユ敹杞�燂紙16bit锛�
            GM6020_Pitch.torque_current = ((rx_data[4] << 8) | rx_data[5]);//鎺ユ敹瀹為檯杞煩
            GM6020_Pitch.temp           =   rx_data[6];//鎺ユ敹鐢垫満娓╁害锛�8bit锛�
            break;
        }
    }
}

/**
<<<<<<< HEAD
 * @brief 发送电压控制报文
 * @param hcan CAN通道
 * @param GM6020_Pitch Pitch轴电机
 * @param GM6020_Yaw Yaw轴电机
=======
 * @file GM6020.c
 * @brief 鍙戦�佺數鍘嬫帶鍒舵姤鏂�
 * @param hcan CAN閫氶亾
 * @param GM6020_Pitch Pitch杞寸數鏈�
 * @param GM6020_Yaw Yaw杞寸數鏈�
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void Set_GM6020_Gimbal_Voltage(CAN_HandleTypeDef* hcan,Moto_GM6020_t GM6020_Yaw,Moto_GM6020_t GM6020_Pitch)
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             CAN1_tx_data[8] = {0},CAN2_tx_data[8] = {0};
    
    tx_header.StdId = 0X1FF;//鏍囪瘑绗︼紙瑙佹墜鍐孭6锛�
    tx_header.IDE   = CAN_ID_STD;//鏍囧噯ID
    tx_header.RTR   = CAN_RTR_DATA;//鏁版嵁甯�
    tx_header.DLC   = 8;//瀛楄妭闀垮害
    CAN1_tx_data[4] = ((int16_t)GM6020_Yaw.Speed_PID.output>>8)&0xff;
    CAN1_tx_data[5] = ((int16_t)GM6020_Yaw.Speed_PID.output)&0xff;
    can_1=HAL_CAN_AddTxMessage(&hcan1, &tx_header, CAN1_tx_data,(uint32_t*)CAN_TX_MAILBOX0);

    CAN2_tx_data[0] = ((int16_t)GM6020_Pitch.Speed_PID.output>>8)&0xff;
    CAN2_tx_data[1] = ((int16_t)GM6020_Pitch.Speed_PID.output)&0xff;
    can_2=HAL_CAN_AddTxMessage(&hcan2, &tx_header, CAN2_tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}
