#include "SuperPower.h"

extern SuperPower_Tx_Message_t SuperPower_Tx_Message;
extern SuperPower_Rx_Message_t SuperPower_Rx_Message;

void SuperPower_Tx(void)
{
	SuperPower_Tx_Message.Switch = 1;
	SuperPower_Tx_Message.Current_Power = JUDGE_fGetChassisPower();
	SuperPower_Tx_Message.Limited_Power = JUDGE_usGetPowerLimit();

	CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[9] = {0};
    
    tx_header.StdId = 0x0C1;//标识符
    tx_header.IDE   = CAN_ID_STD;//标准ID
    tx_header.RTR   = CAN_RTR_DATA;//数据帧
    tx_header.DLC   = 9;//字节长度

    tx_data[0] = SuperPower_Tx_Message.Switch;
	
	tx_data[1] = (*(int32_t*)&SuperPower_Tx_Message.Current_Power)&0xff;
	tx_data[2] = (*(int32_t*)&SuperPower_Tx_Message.Current_Power>>8)&0xff;
	tx_data[3] = (*(int32_t*)&SuperPower_Tx_Message.Current_Power>>16)&0xff;
	tx_data[4] = (*(int32_t*)&SuperPower_Tx_Message.Current_Power>>24)&0xff;
	
	tx_data[5] = (*(int32_t*)&SuperPower_Tx_Message.Limited_Power)&0xff;
	tx_data[6] = (*(int32_t*)&SuperPower_Tx_Message.Limited_Power>>8)&0xff;
	tx_data[7] = (*(int32_t*)&SuperPower_Tx_Message.Limited_Power>>16)&0xff;
	tx_data[8] = (*(int32_t*)&SuperPower_Tx_Message.Limited_Power>>24)&0xff;

    HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}

void SuperPower_Rx(uint8_t rx_data[8])
{
	SuperPower_Rx_Message.Switch = rx_data[0];
	SuperPower_Rx_Message.Stored_Energy = rx_data[1];
	SuperPower_Rx_Message.Temperature = rx_data[2];
}

