/**
 * @file dvc_dji_m2006.c
 * @brief dji_m2006µç»ú¿ØÖÆº¯Êı
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
 * @brief È¡¾ø¶ÔÖµ
 * @param x Ä¿±êÊı¾İ
 * @return È¡¾ø¶ÔÖµºóµÄÊı¾İ
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
 * @brief M3508½ÓÊÜ·´À¡±¨ÎÄº¯Êı
 * @param StdId µç»úID
 * @param rx_data CANÍ¨µÀÊÕµ½µÄÊı¾İ
=======
 * @file M3508.c
 * @brief M3508æ¥å—åé¦ˆæŠ¥æ–‡å‡½æ•°
 * @param StdId ç”µæœºID
 * @param rx_data CANé€šé“æ”¶åˆ°çš„æ•°æ®
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (ğŸ fix(coded): ç¼–ç ä»GBKæ¢æˆUTF-8)
 */
void Get_M2006_Motor_Message(uint32_t StdId,uint8_t rx_data[8])
{
    switch(StdId)//æ¥æ”¶æŒ‡å®šç”µæœºåé¦ˆçš„ä¿¡æ¯
    {
        case 0x201://åé¦ˆæŠ¥æ–‡æ ‡è¯†ç¬¦
        {
            M2006_Rammer.rotor_angle    = ((rx_data[0] << 8) | rx_data[1]);//æ¥æ”¶æœºæ¢°è§’åº¦ï¼ˆ16bitï¼‰
            M2006_Rammer.rotor_speed    = ((rx_data[2] << 8) | rx_data[3]);//æ¥æ”¶è½¬é€Ÿï¼ˆ16bitï¼‰
            M2006_Rammer.torque_current = ((rx_data[4] << 8) | rx_data[5]);//æ¥æ”¶å®é™…è½¬çŸ©
            Get_Total_Angle(&M2006_Rammer);
            break;
        }
        default:
            break;
    }
}

/**
<<<<<<< HEAD
 * @brief ·¢ËÍµçÁ÷±¨ÎÄ¿ØÖÆº¯Êı
 * @param hcan CANÍ¨µÀ
 * @param M2006_Rammer ²¦µ¯ÅÌµç»ú
=======
 * @file dvc_dji_m2006.h.c
 * @brief dvc_dji_m2006.hå‘é€ç”µæµæŠ¥æ–‡æ§åˆ¶å‡½æ•°
 * @param hcan CANé€šé“
 * @param dvc_dji_m2006.h_Chassis åº•ç›˜ç”µæœº
 * @author HWX
 * @date 2024/10/20
>>>>>>> e5d65cc (ğŸ fix(coded): ç¼–ç ä»GBKæ¢æˆUTF-8)
 */
void Set_M2006_Motor_Voltage(CAN_HandleTypeDef* hcan,Moto_M2006_t M2006_Rammer)
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t             tx_data[8] = {0};
    
    tx_header.StdId = 0x200;//æ ‡è¯†ç¬¦ï¼ˆè§æ‰‹å†ŒP6ï¼‰
    tx_header.IDE   = CAN_ID_STD;//æ ‡å‡†ID
    tx_header.RTR   = CAN_RTR_DATA;//æ•°æ®å¸§
    tx_header.DLC   = 8;//å­—èŠ‚é•¿åº¦

    tx_data[0] = (((int16_t)M2006_Rammer.Speed_PID.output)>>8);
    tx_data[1] = (((int16_t)M2006_Rammer.Speed_PID.output));

    HAL_CAN_AddTxMessage(&hcan2, &tx_header, tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}

/**
 * @brief »ñÈ¡2006Ğı×ªÀÛ¼Æ½Ç¶È
 * @param p ÒÑ¾­Ğı×ªµÄ½Ç¶È
 */
void Get_Total_Angle(Moto_M2006_t *p)
{
    int res1=0, res2=0, delta=0;
    if(p->rotor_angle < p->last_angle)
  {            //å¯èƒ½çš„æƒ…å†µ
        res1 = p->rotor_angle + 8192 - p->last_angle;    //æ­£è½¬ï¼Œdelta=+
        res2 = p->rotor_angle - p->last_angle;                //åè½¬    delta=-
    }
    else
  {    //angle > last
        res1 = p->rotor_angle - 8192 - p->last_angle ;//åè½¬    delta -
        res2 = p->rotor_angle - p->last_angle;                //æ­£è½¬    delta +
    }
    //ä¸ç®¡æ­£åè½¬ï¼Œè‚¯å®šæ˜¯è½¬çš„è§’åº¦å°çš„é‚£ä¸ªæ˜¯çœŸçš„
    if(ABS(res1)<ABS(res2))
        delta = ABS(res1);
    else
        delta = ABS(res2);

    p->total_angle += delta;
    p->last_angle = p->rotor_angle;    
}

