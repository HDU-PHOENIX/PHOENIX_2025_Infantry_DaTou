/**
 * @file dvc_dji_m2006.c
 * @brief dji_m2006电机控制函数
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @date 2025-4-13
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */
#include "dvc_dji_m2006.h"

#include "main.h"
#include "can.h"
#include "stm32f4xx_hal_can.h"
#include "alg_pid.h"
#include "stdint.h"
/**
 * @brief 取绝对值
 * @param x 目标数据
 * @return 取绝对值后的数据
 */
int ABS(int x)
{
	if(x<0)
		return -x;
	else
		return x;
}

/**
<<<<<<< HEAD
 * @brief M3508接受反馈报文函数
 * @param StdId 电机ID
 * @param rx_data CAN通道收到的数据
=======
 * @file M3508.c
 * @brief M3508鎺ュ彈鍙嶉鎶ユ枃鍑芥暟
 * @param StdId 鐢垫満ID
 * @param rx_data CAN閫氶亾鏀跺埌鐨勬暟鎹�
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void Get_M2006_Motor_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//鎺ユ敹鎸囧畾鐢垫満鍙嶉鐨勪俊鎭�
    {
        case 0x201://鍙嶉鎶ユ枃鏍囪瘑绗�
        {
            M2006_Rammer.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//鎺ユ敹鏈烘瑙掑害锛�16bit锛�
            M2006_Rammer.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//鎺ユ敹杞�燂紙16bit锛�
            M2006_Rammer.torque_current = ((rx_data[4] << 8) | rx_data[5]);//鎺ユ敹瀹為檯杞煩
            Get_Total_Angle(&M2006_Rammer);
            break;
        }
        default:
            break;
    }
}

/**
<<<<<<< HEAD
 * @brief 发送电流报文控制函数
 * @param hcan CAN通道
 * @param M2006_Rammer 拨弹盘电机
=======
 * @file dvc_dji_m2006.h.c
 * @brief dvc_dji_m2006.h鍙戦�佺數娴佹姤鏂囨帶鍒跺嚱鏁�
 * @param hcan CAN閫氶亾
 * @param dvc_dji_m2006.h_Chassis 搴曠洏鐢垫満
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (馃悶 fix(coded): 缂栫爜浠嶨BK鎹㈡垚UTF-8)
 */
void Set_M2006_Motor_Voltage(CAN_HandleTypeDef* hcan,Moto_M2006_t M2006_Rammer)
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x200;//鏍囪瘑绗︼紙瑙佹墜鍐孭6锛�
    tx_header.IDE   = CAN_ID_STD;//鏍囧噯ID
    tx_header.RTR   = CAN_RTR_DATA;//鏁版嵁甯�
    tx_header.DLC   = 8;//瀛楄妭闀垮害

    tx_data[0] = (((int16_t)M2006_Rammer.Speed_PID.output)>>8);
    tx_data[1] = (((int16_t)M2006_Rammer.Speed_PID.output));

    HAL_CAN_AddTxMessage(&hcan2, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}

/**
 * @brief 获取2006旋转累计角度
 * @param p 已经旋转的角度
 */
void Get_Total_Angle(Moto_M2006_t *p)
{
    int res1=0, res2=0, delta=0;
    if(p->rotor_angle < p->last_angle)
  {            //鍙兘鐨勬儏鍐�
        res1 = p->rotor_angle + 8192 - p->last_angle;    //姝ｈ浆锛宒elta=+
        res2 = p->rotor_angle - p->last_angle;                //鍙嶈浆    delta=-
    }
    else
  {    //angle > last
        res1 = p->rotor_angle - 8192 - p->last_angle ;//鍙嶈浆    delta -
        res2 = p->rotor_angle - p->last_angle;                //姝ｈ浆    delta +
    }
    //涓嶇姝ｅ弽杞紝鑲畾鏄浆鐨勮搴﹀皬鐨勯偅涓槸鐪熺殑
    if(ABS(res1)<ABS(res2))
        delta = ABS(res1);
    else
        delta = ABS(res2);

    p->total_angle += delta;
    p->last_angle = p->rotor_angle;    
}

