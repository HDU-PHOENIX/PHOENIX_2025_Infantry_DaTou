#include "SuperPower.h"

/*
不同的超电有不同的协议，我写的这个仅供参考
*/

extern SuperPower_Tx_Message_t SuperPower_Tx_Message;
extern SuperPower_Rx_Message_t SuperPower_Rx_Message;
extern RC_t RC;
extern SuperPower_Mode_t SuperPower_Mode;
extern SuperPower_Switch_t SuperPower_Switch;
extern Car_Mode_t Car_Mode;

/**
 * @brief 超电初始化函数
 * @author HWX
 * @date 2025/3/1
 */
void SuperPower_Init(void)
{
    SuperPower_Switch = SuperPower_Stop;
    SuperPower_Mode = SuperPower_Off;
}

/**
 * @brief 超电数据处理并发送函数
 * @author HWX
 * @date 2025/3/1
 */
void SuperPower_Tx(void)
{
	CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x0C1;//标识符
    tx_header.IDE   = CAN_ID_STD;//标准ID
    tx_header.RTR   = CAN_RTR_DATA;//数据帧
    tx_header.DLC   = 8;//字节长度

    if(SuperPower_Switch == SuperPower_Stop)
        SuperPower_Tx_Message.Switch=0;
    else if(SuperPower_Switch == SuperPower_Work)
        SuperPower_Tx_Message.Switch=1;
	SuperPower_Tx_Message.Limited_Power = JUDGE_usGetPowerLimit();

	memcpy(tx_data,&SuperPower_Tx_Message,8);
    HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}

/**
 * @brief 超电接收数据处理
 * @param rx_data 接收到的原始数据
 * @author HWX
 * @date 2025/3/1
 */
void SuperPower_Rx(uint8_t rx_data[8])
{
	memcpy(&SuperPower_Rx_Message,rx_data,5);
    SuperPower_Rx_Message.Now_power /= 100;
}

/**
 * @brief 键鼠开关超电
 */
bool E_judge=false;
void SuperPower_KeyBoard_Control(void)
{
    if(IF_KEY_PRESSED_E == 1 && E_judge == true)//点击E开启超电
    {
        if(SuperPower_Mode == SuperPower_On)
            SuperPower_Mode = SuperPower_Off;
        else if(SuperPower_Mode == SuperPower_Off)
            SuperPower_Mode = SuperPower_On;
        E_judge = false;
	}
    if(IF_KEY_PRESSED_E == 0)
    {
        E_judge = true;
    }
}

/**
 * @brief 键鼠启动超电
 */
bool C_judge = false;
void SuperPower_Switch_Control(void)
{
    if(SuperPower_Rx_Message.Stored_Energy <= 5)
        SuperPower_Mode = SuperPower_Off;

    if(IF_KEY_PRESSED_C == 1 && C_judge == true)//点击C启动超电
    {
        if(SuperPower_Switch == SuperPower_Stop)
            SuperPower_Switch = SuperPower_Work;
        else if(SuperPower_Switch == SuperPower_Work)
            SuperPower_Switch = SuperPower_Stop;
        C_judge = false;
    }
    if(IF_KEY_PRESSED_C == 0)
    {
        C_judge = true;
    if(IF_SUPERPOWER_SWITCH_OFFSET_ALIVE != 1)
        SuperPower_Switch = SuperPower_Stop;
    }
}
