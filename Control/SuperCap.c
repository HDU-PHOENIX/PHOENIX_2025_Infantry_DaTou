#include "SuperCap.h"

/*
不同的超电有不同的协议，我写的这个仅供参考
*/

extern SuperCap_Tx_Message_t SuperCap_Tx_Message;
extern SuperCap_Rx_Message_t SuperCap_Rx_Message;
extern RC_t RC;
extern SuperCap_Mode_t SuperCap_Mode;
extern SuperCap_Switch_t SuperCap_Switch;
extern Car_Mode_t Car_Mode;

/**
 * @brief 超电初始化函数
 * @author HWX
 * @date 2025/3/1
 */
void SuperCap_Init(void)
{
    SuperCap_Switch = SuperCap_Stop;
    SuperCap_Mode = SuperCap_Off;
}

/**
 * @brief 超电数据处理并发送函数
 * @author HWX
 * @date 2025/3/1
 */
void SuperCap_Tx(void)
{
	CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x0C1;//标识符
    tx_header.IDE   = CAN_ID_STD;//标准ID
    tx_header.RTR   = CAN_RTR_DATA;//数据帧
    tx_header.DLC   = 8;//字节长度

    if(SuperCap_Switch == SuperCap_Stop)
        SuperCap_Tx_Message.Switch=0;
    else if(SuperCap_Switch == SuperCap_Work)
        SuperCap_Tx_Message.Switch=1;
	SuperCap_Tx_Message.Limited_Power = JUDGE_usGetPowerLimit();

	memcpy(tx_data,&SuperCap_Tx_Message,8);
    HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}

/**
 * @brief 超电接收数据处理
 * @param rx_data 接收到的原始数据
 * @author HWX
 * @date 2025/3/1
 */
void SuperCap_Rx(uint8_t rx_data[8])
{
	memcpy(&SuperCap_Rx_Message,rx_data,5);
    SuperCap_Rx_Message.Now_power /= 100;
}

/**
 * @brief 键鼠开关超电
 */
bool E_judge=false;
void SuperCap_KeyBoard_Control(void)
{
    if(IF_KEY_PRESSED_E == 1 && E_judge == true)//点击E开启超电
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
 * @brief 键鼠启动超电
 */
bool C_judge = false;
void SuperCap_Switch_Control(void)
{
    if(SuperCap_Rx_Message.Stored_Energy <= 5)
        SuperCap_Mode = SuperCap_Off;

    if(IF_KEY_PRESSED_C == 1 && C_judge == true)//点击C启动超电
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
