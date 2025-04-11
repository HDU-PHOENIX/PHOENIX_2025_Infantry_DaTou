/**
 * @file alg_pid.c
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @brief PID�㷨
 * @date 2025-4-11
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */

#include "alg_pid.h"

/**
 * @brief PID�����ʼ��
 * @param PID PID����
 * @param kp ������ϵ��
 * @param ki ������ϵ��
 * @param kd ΢����ϵ��
 * @param kf ǰ����ϵ��
 * @param dead_zone ����
 * @param angle_max ���㱣�������Ƕȣ�
 * @param i_variable_max ���ٻ�������
 * @param i_variable_min ���ٻ�������
 * @param d_first ΢�����У�Ϊ1������
 * @param i_max �����޷�
 * @param out_max ����޷�
 * @note ����б��ٻ��ֻ���i_variable_max��Ҫ��i_separatedС
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
 * @brief PID���㱣��
 * @param pid PID�ṹ��
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
 * @brief PID���㺯��
 * @param PID PID�ṹ��
 * @param target �趨ֵ
 * @param now ʵ��ֵ
 * @return PID������
 */
float fPidCalc(PID_struct_t *PID, float target, float now)
{
    //ǰ�ù���
    PID->target[0] = target;
    PID->now[0] = now;
    //���㱣��
    if(PID->angle_max != 0){
        vPidProtect(PID);
    }
    //�������
    PID->err[0] = PID->target[0] - PID->now[0];
    // �ж�����
    if (PID->err[0] < PID->dead_zone && PID->err[0] > -PID->dead_zone){
        PID->err[0] = 0.0f;
    }
    //���������
    PID->p_out = PID->kp * PID->err[0];
    //���������
    float i_speed_ratio = 0;
    if(PID->i_variable_max == 0 && PID->i_variable_min == 0){
        //�Ǳ��ٻ���
        i_speed_ratio = 1;
    }else{
        //���ٻ���
        if(PID->err[0] < PID->i_variable_min && PID->err[0] > -PID->i_variable_min)
            i_speed_ratio = 1;
        else if((PID->err[0] < PID->i_variable_max && PID->err[0] > PID->i_variable_min) || (PID->err[0] > -PID->i_variable_max && PID->err[0] < -PID->i_variable_min))
            i_speed_ratio = (PID->i_variable_max - PID->err[0]) / (PID->i_variable_max - PID->i_variable_min);
        else if(PID->err[0] > PID->i_variable_max || PID->err[0] < -PID->i_variable_max)
            i_speed_ratio = 0;
    }
    if(PID->i_separated == 0){
        //û�л��ַ���
        PID->i_out += PID->ki * PID->err[0];
    }else{
        //�л��ַ���
        if(PID->err[0] < -PID->i_separated || PID->err[0] > PID->i_separated){
            //�ڻ��ַ���������
            PID->integral = 0;
            PID->i_out = 0;
        }else{
            //���ڻ��ַ���������
            PID->integral += i_speed_ratio * PID->err[0];
            PID->i_out = PID->ki * PID->integral;
        }
    }
    //�����޷�
    PID->i_out=fPidLimit(PID->i_out, -PID->i_max, PID->i_max);
    //����΢����
    if(PID->d_first == 0){
        //û��΢������
        PID->d_out = PID->kd * (PID->err[0] - PID->err[1]);
    }else{
        //΢������
        PID->d_out = -PID->kd * (PID->now[0] - PID->now[1]);
    }
    //����ǰ����
    PID->f_out = PID->kf * (PID->target[0] - PID->target[1]);
    //�������
    PID->output = PID->p_out + PID->i_out + PID->d_out + PID->f_out;
    //����޷�
    PID->output=fPidLimit(PID->output, -PID->out_max, PID->out_max);
    //�ƺ����
    PID->err[1] = PID->err[0];
    PID->target[1] = PID->target[0];
    PID->now[1] = PID->now[0];
    return PID->output;
}

/**
 * @brief ����һ���������� value ��ָ������Сֵ min �����ֵ max ֮��
 * @param value ����ֵ
 * @param min ��Сֵ
 * @param max ���ֵ
 * @return ������
 */
float fPidLimit(float value,float min,float max)
{
	if(value<min)
		return min;
	else if(value>max)
		return max;
	else return value;
}

