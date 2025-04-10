#include "dvc_dji_m2006.h"

extern Moto_M2006_t M2006_Rammer;

int ABS(int x)
{
	if(x<0)
		return -x;
	else
		return x;
}

/**
 * @file M3508.c
 * @brief M3508接受反馈报文函数
 * @param StdId 电机ID
 * @param rx_data CAN通道收到的数据
 * @author HWX
 * @date 2024/10/20
 */
void Get_M2006_Motor_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//接收指定电机反馈的信息
    {
        case 0x201://反馈报文标识符
        {
            M2006_Rammer.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//接收机械角度（16bit）
            M2006_Rammer.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//接收转速（16bit）
            M2006_Rammer.torque_current = ((rx_data[4] << 8) | rx_data[5]);//接收实际转矩
            Get_Total_Angle(&M2006_Rammer);
            break;
        }
        default:
            break;
    }
}

/**
 * @file dvc_dji_m2006.h.c
 * @brief dvc_dji_m2006.h发送电流报文控制函数
 * @param hcan CAN通道
 * @param dvc_dji_m2006.h_Chassis 底盘电机
 * @author HWX
 * @date 2024/10/20
 */
void Set_M2006_Motor_Voltage(CAN_HandleTypeDef* hcan,Moto_M2006_t M2006_Rammer)
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x200;//标识符（见手册P6）
    tx_header.IDE   = CAN_ID_STD;//标准ID
    tx_header.RTR   = CAN_RTR_DATA;//数据帧
    tx_header.DLC   = 8;//字节长度

    tx_data[0] = (((int16_t)M2006_Rammer.Speed_PID.output)>>8);
    tx_data[1] = (((int16_t)M2006_Rammer.Speed_PID.output));

    HAL_CAN_AddTxMessage(&hcan2, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}


void Get_Total_Angle(Moto_M2006_t *p)
{
    int res1=0, res2=0, delta=0;
    if(p->rotor_angle < p->last_angle)
  {            //可能的情况
        res1 = p->rotor_angle + 8192 - p->last_angle;    //正转，delta=+
        res2 = p->rotor_angle - p->last_angle;                //反转    delta=-
    }
    else
  {    //angle > last
        res1 = p->rotor_angle - 8192 - p->last_angle ;//反转    delta -
        res2 = p->rotor_angle - p->last_angle;                //正转    delta +
    }
    //不管正反转，肯定是转的角度小的那个是真的
    if(ABS(res1)<ABS(res2))
        delta = ABS(res1);
    else
        delta = ABS(res2);

    p->total_angle += delta;
    p->last_angle = p->rotor_angle;    
}

