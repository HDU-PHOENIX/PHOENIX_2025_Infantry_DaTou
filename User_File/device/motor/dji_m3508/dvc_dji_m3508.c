/**
 * @file dvc_dji_m3508.c
 * @brief dji_m3508电机控制函数
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @date 2025-4-13
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */
#include "dvc_dji_m3508.h"

#include "main.h"
#include "can.h"
#include "stm32f4xx.h"
#include "alg_pid.h"
#include "stdint.h"
#include "stm32f4xx_hal_can.h"
/**
<<<<<<< HEAD
 * @brief 底盘M3508接受反馈报文函数
 * @param StdId 电机ID
 * @param rx_data CAN通道收到的数据
=======
 * @file M3508.c
 * @brief 搴曠洏M3508鎺ュ彈鍙嶉鎶ユ枃鍑芥暟
 * @param StdId 鐢垫満ID
 * @param rx_data CAN閫氶亾鏀跺埌鐨勬暟鎹�
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void Get_M3508_Chassis_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//鎺ユ敹鎸囧畾鐢垫満鍙嶉鐨勪俊鎭�
    {
        case 0x201://鍙嶉鎶ユ枃鏍囪瘑绗�
        {
            M3508_Chassis[0].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//鎺ユ敹鏈烘瑙掑害锛�16bit锛�
            M3508_Chassis[0].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//鎺ユ敹杞�燂紙16bit锛�
            M3508_Chassis[0].torque_current = ((rx_data[4] << 8) | rx_data[5]);//鎺ユ敹瀹為檯杞煩
            M3508_Chassis[0].temp           =   rx_data[6];//鎺ユ敹鐢垫満娓╁害锛�8bit锛�
            break;
        }
        case 0x202://鍙嶉鎶ユ枃鏍囪瘑绗�
        {
            M3508_Chassis[1].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//鎺ユ敹鏈烘瑙掑害锛�16bit锛�
            M3508_Chassis[1].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//鎺ユ敹杞�燂紙16bit锛�
            M3508_Chassis[1].torque_current = ((rx_data[4] << 8) | rx_data[5]);//鎺ユ敹瀹為檯杞煩
            M3508_Chassis[1].temp           =   rx_data[6];//鎺ユ敹鐢垫満娓╁害锛�8bit锛�
            break;
        }
        case 0x203://鍙嶉鎶ユ枃鏍囪瘑绗�
        {
            M3508_Chassis[2].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//鎺ユ敹鏈烘瑙掑害锛�16bit锛�
            M3508_Chassis[2].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//鎺ユ敹杞�燂紙16bit锛�
            M3508_Chassis[2].torque_current = ((rx_data[4] << 8) | rx_data[5]);//鎺ユ敹瀹為檯杞煩
            M3508_Chassis[2].temp           =   rx_data[6];//鎺ユ敹鐢垫満娓╁害锛�8bit锛�
            break;
        } 
        case 0x204://鍙嶉鎶ユ枃鏍囪瘑绗�
        {
            M3508_Chassis[3].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//鎺ユ敹鏈烘瑙掑害锛�16bit锛�
            M3508_Chassis[3].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//鎺ユ敹杞�燂紙16bit锛�
            M3508_Chassis[3].torque_current = ((rx_data[4] << 8) | rx_data[5]);//鎺ユ敹瀹為檯杞煩
            M3508_Chassis[3].temp           =   rx_data[6];//鎺ユ敹鐢垫満娓╁害锛�8bit锛�
            break;
        }
    }
}

/**
<<<<<<< HEAD
 * @brief 底盘M3508发送电流报文控制函数
 * @param hcan CAN通道
 * @param M3508_Chassis 底盘电机
=======
 * @file M3508.c
 * @brief 搴曠洏M3508鍙戦�佺數娴佹姤鏂囨帶鍒跺嚱鏁�
 * @param hcan CAN閫氶亾
 * @param M3508_Chassis 搴曠洏鐢垫満
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void Set_M3508_Chassis_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Chassis[4])
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x200;//鏍囪瘑绗︼紙瑙佹墜鍐孭6锛�
    tx_header.IDE   = CAN_ID_STD;//鏍囧噯ID
    tx_header.RTR   = CAN_RTR_DATA;//鏁版嵁甯�
    tx_header.DLC   = 8;//瀛楄妭闀垮害

    tx_data[0] = ((int16_t)M3508_Chassis[0].PID.output>>8)&0xff;
    tx_data[1] = ((int16_t)M3508_Chassis[0].PID.output)&0xff;

    tx_data[2] = ((int16_t)M3508_Chassis[1].PID.output>>8)&0xff;
    tx_data[3] = ((int16_t)M3508_Chassis[1].PID.output)&0xff;

    tx_data[4] = ((int16_t)M3508_Chassis[2].PID.output>>8)&0xff;
    tx_data[5] = ((int16_t)M3508_Chassis[2].PID.output)&0xff;

    tx_data[6] = ((int16_t)M3508_Chassis[3].PID.output>>8)&0xff;
    tx_data[7] = ((int16_t)M3508_Chassis[3].PID.output)&0xff;

    HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}

/**
<<<<<<< HEAD
 * @brief 射击M3508接受反馈报文函数
 * @param StdId 电机ID
 * @param rx_data CAN通道收到的数据
=======
 * @file M3508.c
 * @brief 灏勫嚮M3508鎺ュ彈鍙嶉鎶ユ枃鍑芥暟
 * @param StdId 鐢垫満ID
 * @param rx_data CAN閫氶亾鏀跺埌鐨勬暟鎹�
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void Get_M3508_Shoot_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//鎺ユ敹鎸囧畾鐢垫満鍙嶉鐨勪俊鎭�
    {
        case 0x204://鍙嶉鎶ユ枃鏍囪瘑绗�
        {
            M3508_Shoot[0].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//鎺ユ敹鏈烘瑙掑害锛�16bit锛�
            M3508_Shoot[0].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//鎺ユ敹杞�燂紙16bit锛�
            M3508_Shoot[0].torque_current = ((rx_data[4] << 8) | rx_data[5]);//鎺ユ敹瀹為檯杞煩
            M3508_Shoot[0].temp           =   rx_data[6];//鎺ユ敹鐢垫満娓╁害锛�8bit锛�
            break;
        }
        case 0x203://鍙嶉鎶ユ枃鏍囪瘑绗�
        {
            M3508_Shoot[1].rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//鎺ユ敹鏈烘瑙掑害锛�16bit锛�
            M3508_Shoot[1].rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//鎺ユ敹杞�燂紙16bit锛�
            M3508_Shoot[1].torque_current = ((rx_data[4] << 8) | rx_data[5]);//鎺ユ敹瀹為檯杞煩
            M3508_Shoot[1].temp           =   rx_data[6];//鎺ユ敹鐢垫満娓╁害锛�8bit锛�
            break;
        }
    }
}


/**
<<<<<<< HEAD
 * @brief 射击M3508发送电流报文控制函数
 * @param hcan CAN通道
 * @param M3508_Shoot 射击电机
=======
 * @file M3508.c
 * @brief 灏勫嚮M3508鍙戦�佺數娴佹姤鏂囨帶鍒跺嚱鏁�
 * @param hcan CAN閫氶亾
 * @param M3508_Shoot 灏勫嚮鐢垫満
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void Set_M3508_Shoot_Voltage(CAN_HandleTypeDef* hcan,Moto_M3508_t M3508_Shoot[2])
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x200;//鏍囪瘑绗︼紙瑙佹墜鍐孭6锛�
    tx_header.IDE   = CAN_ID_STD;//鏍囧噯ID
    tx_header.RTR   = CAN_RTR_DATA;//鏁版嵁甯�
    tx_header.DLC   = 8;//瀛楄妭闀垮害

    tx_data[6] = ((int16_t)M3508_Shoot[0].PID.output>>8)&0xff;
    tx_data[7] = ((int16_t)M3508_Shoot[0].PID.output)&0xff;

    tx_data[4] = ((int16_t)M3508_Shoot[1].PID.output>>8)&0xff;
    tx_data[5] = ((int16_t)M3508_Shoot[1].PID.output)&0xff;

    HAL_CAN_AddTxMessage(&hcan2, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}
