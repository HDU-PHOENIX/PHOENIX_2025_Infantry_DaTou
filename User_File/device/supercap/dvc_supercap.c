/**
 * @file dvc_supercap.c
 * @brief 25赛季超电协议以及超电控制
 * 
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @date 2025-4-13
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */
#include "dvc_supercap.h"

<<<<<<< HEAD
#include "main.h"
#include "can.h"
#include "dvc_judge.h"
#include "string.h"
#include "dvc_dr16.h"
#include "Car_Mode.h"

/**
 * @brief 超电初始化函数
=======
/*
涓嶅悓鐨勮秴鐢垫湁涓嶅悓鐨勫崗璁紝鎴戝啓鐨勮繖涓粎渚涘弬鑰�
*/

extern SuperCap_Tx_Message_t SuperCap_Tx_Message;
extern SuperCap_Rx_Message_t SuperCap_Rx_Message;
extern RC_t RC;
extern SuperCap_Mode_t SuperCap_Mode;
extern SuperCap_Switch_t SuperCap_Switch;
extern Car_Mode_t Car_Mode;

/**
 * @brief 瓒呯數鍒濆鍖栧嚱鏁�
 * @author HWX
 * @date 2025/3/1
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void SuperCap_Init(void)
{
    SuperCap_Switch = SuperCap_Stop;
    SuperCap_Mode = SuperCap_Off;
}

/**
<<<<<<< HEAD
 * @brief 超电数据处理并发送函数
=======
 * @brief 瓒呯數鏁版嵁澶勭悊骞跺彂閫佸嚱鏁�
 * @author HWX
 * @date 2025/3/1
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void SuperCap_Tx(void)
{
	CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x0C1;//鏍囪瘑绗�
    tx_header.IDE   = CAN_ID_STD;//鏍囧噯ID
    tx_header.RTR   = CAN_RTR_DATA;//鏁版嵁甯�
    tx_header.DLC   = 8;//瀛楄妭闀垮害

    if(SuperCap_Switch == SuperCap_Stop)
        SuperCap_Tx_Message.Switch=0;
    else if(SuperCap_Switch == SuperCap_Work)
        SuperCap_Tx_Message.Switch=1;
	SuperCap_Tx_Message.Limited_Power = JUDGE_usGetPowerLimit();

	memcpy(tx_data,&SuperCap_Tx_Message,8);
    HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}

/**
<<<<<<< HEAD
 * @brief 超电接收数据处理
 * @param rx_data 接收到的原始数据
=======
 * @brief 瓒呯數鎺ユ敹鏁版嵁澶勭悊
 * @param rx_data 鎺ユ敹鍒扮殑鍘熷鏁版嵁
 * @author HWX
 * @date 2025/3/1
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void SuperCap_Rx(uint8_t rx_data[8])
{
	memcpy(&SuperCap_Rx_Message,rx_data,5);
    SuperCap_Rx_Message.Now_power /= 100;
}

/**
 * @brief 閿紶寮�鍏宠秴鐢�
 */
bool E_judge=false;
void SuperCap_KeyBoard_Control(void)
{
    if(IF_KEY_PRESSED_E == 1 && E_judge == true)//鐐瑰嚮E寮�鍚秴鐢�
    {
        if(SuperCap_Mode == SuperCap_On)
            SuperCap_Mode = SuperCap_Off;
        else if(SuperCap_Mode == SuperCap_Off)
            SuperCap_Mode = SuperCap_On;
        E_judge = false;
	}
    if(IF_KEY_PRESSED_E == 0)
    {
        E_judge = true;
    }
}

/**
 * @brief 閿紶鍚姩瓒呯數
 */
bool C_judge = false;
void SuperCap_Switch_Control(void)
{
    if(SuperCap_Rx_Message.Stored_Energy <= 5)
        SuperCap_Mode = SuperCap_Off;

    if(IF_KEY_PRESSED_C == 1 && C_judge == true)//鐐瑰嚮C鍚姩瓒呯數
    {
        if(SuperCap_Switch == SuperCap_Stop)
            SuperCap_Switch = SuperCap_Work;
        else if(SuperCap_Switch == SuperCap_Work)
            SuperCap_Switch = SuperCap_Stop;
        C_judge = false;
    }
    if(IF_KEY_PRESSED_C == 0)
    {
        C_judge = true;
    if(IF_SUPERPOWER_SWITCH_OFFSET_ALIVE != 1)
        SuperCap_Switch = SuperCap_Stop;
    }
}
