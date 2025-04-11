/**
 * @file alg_pid.c
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @brief PID算法
 * @date 2025-4-11
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */

#include "alg_pid.h"

/**
 * @brief PID数组初始化
 * @param PID PID数组
 * @param kp 比例项系数
 * @param ki 积分项系数
 * @param kd 微分项系数
 * @param kf 前馈项系数
 * @param dead_zone 死区
 * @param angle_max 过零保护（最大角度）
 * @param i_variable_max 变速积分上限
 * @param i_variable_min 变速积分下限
 * @param d_first 微分先行（为1启动）
 * @param i_max 积分限幅
 * @param out_max 输出限幅
 * @note 如果有变速积分环节i_variable_max需要比i_separated小
 */
void vPidInit(PID_struct_t *PID,
            float kp,
            float ki,
            float kd,
            float kf,
            float dead_zone,
            float angle_max,
            float i_variable_min,
            float i_variable_max,
            float i_separated,
            uint8_t d_first,
            float i_max,
            float out_max)
{
    PID->kp             = kp;
    PID->ki             = ki;
    PID->kd             = kd;
    PID->kf             = kf;
    PID->dead_zone      = dead_zone;
    PID->angle_max      = angle_max;
    PID->i_variable_min = i_variable_min;
    PID->i_variable_max = i_variable_max;
    PID->i_separated    = i_separated;
    PID->d_first        = d_first;
    PID->i_max          = i_max;
    PID->out_max        = out_max;
}

/**
 * @brief PID过零保护
 * @param pid PID结构体
 */
void vPidProtect(PID_struct_t *pid)
{
    float half_angle = pid->angle_max/2;
    if(pid->target[0] - pid->now[0] > half_angle)
	{
		pid->now[0]+=pid->angle_max;
	}
	else if(pid->target[0] - pid->now[0] < -half_angle)
	{
		pid->now[0]-=pid->angle_max;
	}
}

/**
 * @brief PID计算函数
 * @param PID PID结构体
 * @param target 设定值
 * @param now 实际值
 * @return PID计算结果
 */
float fPidCalc(PID_struct_t *PID, float target, float now)
{
    //前置工作
    PID->target[0] = target;
    PID->now[0] = now;
    //过零保护
    if(PID->angle_max != 0){
        vPidProtect(PID);
    }
    //计算误差
    PID->err[0] = PID->target[0] - PID->now[0];
    // 判断死区
    if (PID->err[0] < PID->dead_zone && PID->err[0] > -PID->dead_zone){
        PID->err[0] = 0.0f;
    }
    //计算比例项
    PID->p_out = PID->kp * PID->err[0];
    //计算积分项
    float i_speed_ratio = 0;
    if(PID->i_variable_max == 0 && PID->i_variable_min == 0){
        //非变速积分
        i_speed_ratio = 1;
    }else{
        //变速积分
        if(PID->err[0] < PID->i_variable_min && PID->err[0] > -PID->i_variable_min)
            i_speed_ratio = 1;
        else if((PID->err[0] < PID->i_variable_max && PID->err[0] > PID->i_variable_min) || (PID->err[0] > -PID->i_variable_max && PID->err[0] < -PID->i_variable_min))
            i_speed_ratio = (PID->i_variable_max - PID->err[0]) / (PID->i_variable_max - PID->i_variable_min);
        else if(PID->err[0] > PID->i_variable_max || PID->err[0] < -PID->i_variable_max)
            i_speed_ratio = 0;
    }
    if(PID->i_separated == 0){
        //没有积分分离
        PID->i_out += PID->ki * PID->err[0];
    }else{
        //有积分分离
        if(PID->err[0] < -PID->i_separated || PID->err[0] > PID->i_separated){
            //在积分分离区间里
            PID->integral = 0;
            PID->i_out = 0;
        }else{
            //不在积分分离区间里
            PID->integral += i_speed_ratio * PID->err[0];
            PID->i_out = PID->ki * PID->integral;
        }
    }
    //积分限幅
    PID->i_out=fPidLimit(PID->i_out, -PID->i_max, PID->i_max);
    //计算微分项
    if(PID->d_first == 0){
        //没有微分先行
        PID->d_out = PID->kd * (PID->err[0] - PID->err[1]);
    }else{
        //微分先行
        PID->d_out = -PID->kd * (PID->now[0] - PID->now[1]);
    }
    //计算前馈项
    PID->f_out = PID->kf * (PID->target[0] - PID->target[1]);
    //计算输出
    PID->output = PID->p_out + PID->i_out + PID->d_out + PID->f_out;
    //输出限幅
    PID->output=fPidLimit(PID->output, -PID->out_max, PID->out_max);
    //善后操作
    PID->err[1] = PID->err[0];
    PID->target[1] = PID->target[0];
    PID->now[1] = PID->now[0];
    return PID->output;
}

/**
 * @brief 限制一个整数变量 value 在指定的最小值 min 和最大值 max 之间
 * @param value 输入值
 * @param min 最小值
 * @param max 最大值
 * @return 计算结果
 */
float fPidLimit(float value,float min,float max)
{
	if(value<min)
		return min;
	else if(value>max)
		return max;
	else return value;
}

