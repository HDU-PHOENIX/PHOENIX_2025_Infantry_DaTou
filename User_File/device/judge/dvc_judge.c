/**
  ******************************************************************************
  * @file    judge.c
  * @brief   解析裁判系统发来的数据，以获得机器人当前的状态数据。
  *
  ******************************************************************************
  * @attention
  *
  * 2021.3 已经完成了和tuxin.c的适配。
  *
  *
  *
  ******************************************************************************
  */

#include "dvc_judge.h"
#include "dri_crc.h"
#include "ui.h"
#include <string.h>

frame_t judge_frame_rx = {0};
frame_t judge_frame_tx;				
uint8_t receive_student_data[113] = {0};
extern uint8_t judge_rx_buff[JUDGE_MAX_LENGTH];

judge_show_data_t    Show_data = {0};//客户端信息
send_to_teammate     CommuData;//队友通信信息
robot_type_t robot_type;

bool Judge_Data_TF = false;//裁判数据是否可用，辅助函数调用
uint8_t Judge_Self_ID;//当前机器人的ID
uint16_t Judge_SelfClient_ID;//发送者机器人对应的客户端ID
/**************裁判系统数据辅助****************/
uint16_t ShootNum=0;//统计发弹量,0x0003触发一次则认为发射了一颗
bool Hurt_Data_Update = false;//装甲板伤害数据是否更新,每受一次伤害置TRUE,然后立即置FALSE,给底盘闪避用
uint16_t Hurt_num = 0;
#define BLUE  0
#define RED   1

uint16_t choose_client(uint8_t robot_id)
{
    uint16_t client_id;
    switch (robot_id)
    {
    case red_hero:
        client_id = red_hero_client;
        break;
    case red_engineer:
        client_id = red_engineer_client;
        break;
    case red_infantry_3:
        client_id = red_infantry_3_client;
        break;
    case red_infantry_4:
        client_id = red_infantry_4_client;
        break;
    case red_infantry_5:
        client_id = red_infantry_5_client;
        break;
    case red_aerial:
        client_id = red_aerial_client;
        break;

    case blue_hero:
        client_id = blue_hero_client;
        break;
    case blue_engineer:
        client_id = blue_engineer_client;
        break;
    case blue_infantry_3:
        client_id = blue_infantry_3_client;
        break;
    case blue_infantry_4:
        client_id = blue_infantry_4_client;
        break;
    case blue_infantry_5:
        client_id = blue_infantry_5_client;
        break;
    case blue_aerial:
        client_id = blue_aerial_client;
        break;
    default:
        break;
    }
    return client_id;
}

//从裁判系统读取数据
bool Judge_Read_Data(uint8_t *ReadFromUsart)
{
    bool retval_tf = false;//数据正确与否标志,每次调用读取裁判系统数据函数都先默认为错误

    //无数据包，则不作任何处理
    if (ReadFromUsart == NULL)
    {
        return -1;
    }
    //将帧头信息拷贝
    memcpy(&judge_frame_rx.frame_header, ReadFromUsart, LEN_HEADER);

    judge_frame_rx.frame_header.Data_Length = (ReadFromUsart[DATA_LENGTH + 1] << 8 ) | ReadFromUsart[DATA_LENGTH];
    //首先判断帧头
    if(judge_frame_rx.frame_header.SOF == Judge_Data_SOF)
    {
        //判断帧头CRC校验是否正确  
        if(Verify_CRC8_Check_Sum(ReadFromUsart, LEN_HEADER) == true)
        {
            //判断整包CRC校验是否正确
            if(Verify_CRC16_Check_Sum(ReadFromUsart, LEN_HEADER + LEN_CMDID + judge_frame_rx.frame_header.Data_Length + LEN_TAIL) == true)
            {
                retval_tf = true;//都校验过了则说明数据可用

                judge_frame_rx.cmd_id = (uint16_t)ReadFromUsart[6] << 8 | ReadFromUsart[5];
                //解析数据命令码,将数据拷贝到相应结构体中(注意拷贝数据的长度)
                switch(judge_frame_rx.cmd_id)
                {
                case game_status_t:
                    memcpy(&judge_frame_rx.data.game_status, ReadFromUsart + DATA, len_game_status);
                    break ;
                case game_result_t:
                    memcpy(&judge_frame_rx.data.game_result, ReadFromUsart + DATA, len_game_result);
                    break;

                case game_robot_HP_t:
                    memcpy(&judge_frame_rx.data.game_robot_HP, ReadFromUsart + DATA, len_game_robot_HP);
                    break;

//                case dart_status_t:
//                    memcpy(&judge_frame_rx.data.dart_status, ReadFromUsart + DATA, len_dart_status);
//                    break;

//                case ICRA_buff_debuff_zone_status_t:
//                    memcpy(&judge_frame_rx.data.ICRA_buff_debuff_zone_status, ReadFromUsart + DATA, len_ICRA_buff_debuff_zone_status);
//                    break;
                case event_data_t:
                    memcpy(&judge_frame_rx.data.event_data, ReadFromUsart + DATA, len_event_data);
                    break;

                case supply_projectile_action_t:
                    memcpy(&judge_frame_rx.data.supply_projectile_action, ReadFromUsart + DATA, len_supply_projectile_action);
                    break;

                case referee_warning_t:
                    memcpy(&judge_frame_rx.data.referee_warning, ReadFromUsart + DATA, len_referee_warning);
                    break;

                case dart_remaining_time_t:
                    memcpy(&judge_frame_rx.data.dart_remaining_time, ReadFromUsart + DATA, len_dart_remaining_time);
                    break;

                case game_robot_status_t:
                    memcpy(&judge_frame_rx.data.game_robot_status, ReadFromUsart + DATA, len_game_robot_status);
                    Judge_Self_ID = judge_frame_rx.data.game_robot_status.robot_id ;
                    if(Judge_Self_ID < 100)
                    {
                        robot_type = red_robot;
                    }
                    else
                    {
                        robot_type = blue_robot;
                    }
                    break;

                case power_heat_data_t:
                    memcpy(&judge_frame_rx.data.power_heat_data, ReadFromUsart + DATA, len_power_heat_data);
                    break;

                case game_robot_pos_t:
                    memcpy(&judge_frame_rx.data.game_robot_pos, ReadFromUsart + DATA, len_game_robot_pos);
                    break;

                case buff_t:
                    memcpy(&judge_frame_rx.data.buff, ReadFromUsart + DATA, len_buff);
                    break;

                case aerial_robot_energy_t:
                    memcpy(&judge_frame_rx.data.aerial_robot_energy, ReadFromUsart + DATA, len_aerial_robot_energy);
                    break;

                case robot_hurt_t:
                    memcpy(&judge_frame_rx.data.robot_hurt, ReadFromUsart + DATA, len_robot_hurt);
                    if(judge_frame_rx.data.robot_hurt.HP_deduction_reason == 0)//装甲伤害扣血
                    {
                        Hurt_Data_Update = true;	//装甲数据每更新一次则判定为受到一次伤害
                        Hurt_num++;
                    }
                    else
                    {
                        Hurt_Data_Update = false;
                    }
                    break;

                case shoot_data_t:
                    memcpy(&judge_frame_rx.data.shoot_data, ReadFromUsart + DATA, len_shoot_data);
//                    JUDGE_ShootNumCount();//发弹量统计
                    break;

                case bullet_remaining_t:
                    memcpy(&judge_frame_rx.data.bullet_remaining, ReadFromUsart + DATA, len_bullet_remaining);
                    break;

                case rfid_status_t:
                    memcpy(&judge_frame_rx.data.rfid_status, ReadFromUsart + DATA, len_rfid_status);
                    break;

                case dart_client_cmd_t:
                    memcpy(&judge_frame_rx.data.dart_client_cmd, ReadFromUsart + DATA, len_dart_client_cmd);
                    break;

                case student_interactive_header_data_t:
                    memcpy(&receive_student_data, ReadFromUsart + STU_DATA, len_student_interactive_header_data);
                    break;

                case robot_interactive_data_t:
                    memcpy(&judge_frame_rx.data.robot_interactive_data, ReadFromUsart + DATA, len_robot_interactive_data);
                    break;

                case smallmap_communicate_t:
                    memcpy(&judge_frame_rx.data.smallmap_communicate, ReadFromUsart + STU_DATA, len_smallmap_communicate);
                    break;
                case robot_command_t:
                    memcpy(&judge_frame_rx.data.robot_command, ReadFromUsart + STU_DATA, len_robot_command);
                    break;
								case client_map_command_t:
                    memcpy(&judge_frame_rx.data.client_map_command, ReadFromUsart + STU_DATA, len_client_map_command);
                    break;

                default:
                    break;
                }
            }
        }
        //首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,用来判断一个数据包是否有多帧数据
        if(*(ReadFromUsart + sizeof(frame_header_t) + LEN_CMDID + judge_frame_rx.frame_header.Data_Length + LEN_TAIL) == 0xA5)
        {
            //如果一个数据包出现了多帧数据,则再次读取
            Judge_Read_Data(ReadFromUsart + sizeof(frame_header_t) + LEN_CMDID + judge_frame_rx.frame_header.Data_Length + LEN_TAIL);
        }
    }
    if (retval_tf == true)
    {
        Judge_Data_TF = true;//辅助函数用
    }
    else		//只要CRC16校验不通过就为FALSE
    {
        Judge_Data_TF = false;//辅助函数用
    }

    return retval_tf;//对数据正误做处理

}

#define send_max_len     200
unsigned char CliendTxBuffer[send_max_len];
void JUDGE_Show_Data(void)
{
    //	static uint8_t datalength,i;
    //	uint8_t judge_led = 0xff;//初始化led为全绿
    //	static uint8_t auto_led_time = 0;
    //	static uint8_t buff_led_time = 0;

    determine_ID();//判断发送者ID和其对应的客户端ID

    Show_data.frame_header.SOF = 0xA5;
    Show_data.frame_header.Data_Length = sizeof(ext_student_interactive_header_data_t) + sizeof(operate_data_t);
    Show_data.frame_header.Seq = 0;
    memcpy(CliendTxBuffer, &Show_data.frame_header, sizeof(frame_header_t));//写入帧头数据
    Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(frame_header_t));//写入帧头CRC8校验码

    Show_data.cmd_id = 0x0301;

    Show_data.student_interactive_header.data_cmd_id = 0xD180;//发送给客户端的cmd，官方固定
    //ID已经是自动读取了
    Show_data.student_interactive_header.sender_ID 	 = Judge_Self_ID;//发送者的ID
    Show_data.student_interactive_header.receiver_ID = Judge_SelfClient_ID;//发送者的ID，只能为发送者机器人对应的客户端
}

#define Teammate_max_len     200
unsigned char TeammateTxBuffer[Teammate_max_len];
bool Send_Color = 0;
bool First_Time_Send_Commu = false;
uint16_t send_time = 0;
void Send_to_Teammate(void)
{
    //	static uint8_t datalength,i;

    Send_Color = is_red_or_blue();//判断发送给哨兵的颜色，17蓝，7红?

    memset(TeammateTxBuffer,0,200);

    CommuData.frame_header.SOF = 0xA5;
    CommuData.frame_header.Data_Length = sizeof(ext_student_interactive_header_data_t) + sizeof(CommuData.student_interactive_data);
    CommuData.frame_header.Seq = 0;
    memcpy(TeammateTxBuffer, &CommuData.frame_header, sizeof(frame_header_t));
    Append_CRC8_Check_Sum(TeammateTxBuffer, sizeof(frame_header_t));

    CommuData.cmd_id = 0x0301;

    CommuData.student_interactive_header_data.sender_ID = Judge_Self_ID;//发送者的ID

    if( First_Time_Send_Commu == true )
    {
        CommuData.student_interactive_header_data.data_cmd_id = 0x0292;//在0x0200-0x02ff之间选?
        CommuData.student_interactive_data[0] = 0x01;
        CommuData.student_interactive_data[1] = 0x03;
        CommuData.student_interactive_data[2] = 0x03;
        CommuData.student_interactive_data[3] = 0x04;
        CommuData.student_interactive_data[4] = 0x05;
        CommuData.student_interactive_data[5] = 0x06;
        CommuData.student_interactive_data[6] = 'W';
        CommuData.student_interactive_data[19] = 0x99;
        //			CommuData.student_interactive_data[7]='o';
        //			CommuData.student_interactive_data[8]='r';
        //			CommuData.student_interactive_data[9]='l';
        //			CommuData.student_interactive_data[10]='d';
        //			CommuData.student_interactive_data[11]='!';
        //			CommuData.student_interactive_data[]="Hello,World!";
        //			send_time++;
        //		if(send_time >= 20)
        //		{
        //			First_Time_Send_Commu = false;
        //		}
        if(Send_Color == BLUE)//自己是蓝，发送给蓝哨兵
        {
            CommuData.student_interactive_header_data.receiver_ID = 107;//接受者ID
        }
        else if(Send_Color == RED)//自己是红，发送给红哨兵
        {
            CommuData.student_interactive_header_data.receiver_ID = 7;//接受者ID
        }
    }
    else
    {
        CommuData.student_interactive_header_data.data_cmd_id = 0x0255;
        send_time = 0;
        CommuData.student_interactive_header_data.receiver_ID = 88;//随便给个ID，不发送
    }

    CommuData.student_interactive_data[0] = 0;//发送的内容，大小不要超过变量的变量类型

    memcpy(TeammateTxBuffer + 5, (uint8_t *)&CommuData.cmd_id, (sizeof(CommuData.cmd_id) + CommuData.frame_header.Data_Length));
    Append_CRC16_Check_Sum(TeammateTxBuffer, sizeof(CommuData.cmd_id) + sizeof(CommuData.frame_header) + sizeof(CommuData.student_interactive_header_data) + 20 + sizeof(CommuData.frame_tail));

    //datalength = sizeof(CommuData);
    //	if( First_Time_Send_Commu == true )
    //	{
    //     HAL_UART_Transmit_DMA(&huart6, TeammateTxBuffer, JUDGE_MAX_LENGTH);
    //	}
}



bool Color;
bool is_red_or_blue(void)//判断自己红蓝方
{
    Judge_Self_ID = judge_frame_rx.data.game_robot_status.robot_id;//读取当前机器人ID

    if(judge_frame_rx.data.game_robot_status.robot_id > 10)
    {
        return BLUE;
    }
    else
    {
        return RED;
    }
}

void determine_ID(void)
{
    Color = is_red_or_blue();
    if(Color == BLUE)
    {
        Judge_SelfClient_ID = 0x0164 + (Judge_Self_ID - 100); //计算客户端id
    }
    else if(Color == RED)
    {
        Judge_SelfClient_ID = 0x0100 + Judge_Self_ID;//计算客户端ID
    }
}

/********************裁判数据辅助判断函数***************************/

/**
  * @brief  数据是否可用
  * @param  void
  * @retval  TRUE可用   FALSE不可用
  * @attention  在裁判读取函数中实时改变返回值
  */
bool JUDGE_sGetDataState(void)
{
    return Judge_Data_TF;
}


/**
  * @brief  比赛状态
  * @param  void
  * @retval void
  * @attention
  */
float JUDGE_fGetgame_progress(void)
{
    return (judge_frame_rx.data.game_status.game_progress);
}

/**
  * @brief  比赛状态
  * @param  void
  * @retval void
  * @attention
  */
/**
  * @brief  判断大小符
  * @param  void
  * @retval void
  * @attention
  */
uint16_t JUDGE_Get_FU(void)
{
    if(judge_frame_rx.data.game_status.stage_remain_time > 210)
    {
        return  '1';
    }
    else
    {
        return'2';
    }
        
}
int JUDGE_Gethurt_reason(void)
{
    return (judge_frame_rx.data.robot_hurt.HP_deduction_reason);
}


/**
  * @brief  读取瞬时功率
  * @param  void
  * @retval 实时功率值
  * @attention
  */
float JUDGE_fGetChassisPower(void)
{
    return (judge_frame_rx.data.power_heat_data.chassis_power);
}

/**
  * @brief  读取底盘功率限制
  * @param  void
  * @retval 实时底盘功率限制
  * @attention
  */
uint8_t JUDGE_usGetPowerLimit(void)
{
    return (judge_frame_rx.data.game_robot_status.chassis_power_limit);
}
/**
  * @brief  读取剩余焦耳能量
  * @param  void
  * @retval 剩余缓冲焦耳能量(最大60)
  * @attention
  */
uint16_t JUDGE_fGetRemainEnergy(void)
{
    return (judge_frame_rx.data.power_heat_data.buffer_energy);
}

/**
  * @brief  读取当前等级
  * @param  void
  * @retval 当前等级
  * @attention
  */
uint8_t JUDGE_ucGetRobotLevel(void)
{
    return	judge_frame_rx.data.game_robot_status.robot_level;
}

/**
  * @brief  读取枪口热量
  * @param  void
  * @attention  实时热量
  */
uint16_t JUDGE_usGetRemoteHeat17(void)
{
    return judge_frame_rx.data.power_heat_data.shooter_17mm_1_barrel_heat;
}

/**
  * @brief  读取射速
  * @param  void
  * @attention  实时射速
  */
float JUDGE_usGetSpeedHeat17(void)
{
    return (judge_frame_rx.data.shoot_data.initial_speed);
}



/**
  * @brief  读取发弹量
  * @param  void
  * @retval 发弹量
  * @attention 不适用于双枪管
  */
uint16_t JUDGE_usGetShootNum(void)
{
    return ShootNum;
}

/**
  * @brief  发弹量清零
  * @param  void
  * @retval void
  * @attention
  */
void JUDGE_ShootNum_Clear(void)
{
    ShootNum = 0;
}

/**
  * @brief  读取枪口热量
  * @param  void
  * @retval 当前等级42mm热量上限
  * @attention
  */
uint16_t JUDGE_usGetHeatLimit(void)
{
    return judge_frame_rx.data.game_robot_status.shooter_barrel_heat_limit;
}
///**
//  * @brief  读取枪口射速上限
//  * @param  void
//  * @retval 当前等级42mm射速上限
//  * @attention
//  */
//uint8_t JUDGE_usGetSpeedLimit(void)
//{
//    return judge_frame_rx.data.game_robot_status.shooter_id1_17mm_speed_limit;
//}
/**
  * @brief  当前等级对应的枪口每秒冷却值
  * @param  void
  * @retval 当前等级42mm冷却速度
  * @attention
  */
uint16_t JUDGE_usGetShootCold(void)
{
    return judge_frame_rx.data.game_robot_status.shooter_barrel_cooling_value;
}
//雷达标记

/****************底盘自动闪避判断用*******************/
/**
  * @brief  装甲板伤害数据是否更新
  * @param  void
  * @retval TRUE已更新   FALSE没更新
  * @attention
  */
//bool JUDGE_IfArmorHurt(void)
//{
//    static portTickType ulCurrent = 0;
//    static uint32_t ulDelay = 0;
//    static bool IfHurt = false;//默认装甲板处于离线状态


//    ulCurrent = xTaskGetTickCount();

//    if (Hurt_Data_Update == true)//装甲板数据更新
//    {
//        Hurt_Data_Update = false;//保证能判断到下次装甲板伤害更新
//        ulDelay = ulCurrent + 200;//
//        IfHurt = true;
//    }
//    else if (ulCurrent > ulDelay)//
//    {
//        IfHurt = false;
//    }

//    return IfHurt;
//}

bool Judge_If_Death(void)
{
    if(judge_frame_rx.data.game_robot_status.current_HP == 0 && JUDGE_sGetDataState() == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}


// 0 位 机器人编号
// 1 位 机器人位置


