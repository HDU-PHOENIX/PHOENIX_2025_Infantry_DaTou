#include "PID.h"

/**************************几个PID除了过零保护外没有任何区别**********************************/
/********************************************************************************************
*角度环-KI给0
*速度环-KD给0
*先调速度环，等速度环能实现跳变跟随再调角度。调速度时，pitch轴如果有角度限制，叫结构先把角度限制拆除
********************************************************************************************/

float Limit_Min_Max(float value,float min,float max);

/**
 * @brief PID数组初始化
 * @param PID PID数组
 * @param kp 
 * @param ki 
 * @param kd 
 * @param i_max 
 * @param out_max 
 */
void PID_init(PID_struct_t *PID,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max)//PID初始化函数
{
  PID->kp      = kp;
  PID->ki      = ki;
  PID->kd      = kd;
  PID->i_max   = i_max;//积分限幅
  PID->out_max = out_max;//输出限幅
}

 void PID_Protect_Angle(PID_struct_t *pid)
{
	if(pid->ref - pid->fdb > 4096)
	{
		pid->fdb+=8192;
	}
	else if(pid->ref - pid->fdb < -4096)
	{
		pid->fdb-=8192;
	}
}

void PID_Protect_Ink(PID_struct_t *pid)
{
	if(pid->ref - pid->fdb > 180)
	{
		pid->fdb+=360;
	}
	else if(pid->ref - pid->fdb < -180)
	{
		pid->fdb-=360;
	}
}

float PID_Calc_Angle(PID_struct_t *PID, float ref, float fdb)//PID运算函数（目标，实际）
{
  PID->ref = ref;
  PID->fdb = fdb;

	PID_Protect_Angle(PID);//过零保护

  PID->err[1] = PID->err[0];
  PID->err[0] = PID->ref - PID->fdb;
  
  PID->p_out  = PID->kp * PID->err[0];
  PID->i_out += PID->ki * PID->err[0];
  PID->d_out  = PID->kd * (PID->err[0] - PID->err[1]);
  PID->i_out=Limit_Min_Max(PID->i_out, -PID->i_max, PID->i_max);
  
  PID->output = PID->p_out + PID->i_out + PID->d_out;
  PID->output=Limit_Min_Max(PID->output, -PID->out_max, PID->out_max);
  return PID->output;
}

float PID_Calc_Ink(PID_struct_t *PID, float ref, float fdb)//PID运算函数（目标，实际）
{
  PID->ref = ref;
  PID->fdb = fdb;

	PID_Protect_Ink(PID);//过零保护

  PID->err[1] = PID->err[0];
  PID->err[0] = PID->ref - PID->fdb;
  
  PID->p_out  = PID->kp * PID->err[0];
  PID->i_out += PID->ki * PID->err[0];
  PID->d_out  = PID->kd * (PID->err[0] - PID->err[1]);
  PID->i_out=Limit_Min_Max(PID->i_out, -PID->i_max, PID->i_max);
  
  PID->output = PID->p_out + PID->i_out + PID->d_out;
  PID->output=Limit_Min_Max(PID->output, -PID->out_max, PID->out_max);
  return PID->output;
}

float PID_Calc_Speed(PID_struct_t *PID, float ref, float fdb)//PID运算函数（目标，实际）
{
  PID->ref = ref;
  PID->fdb = fdb;

  PID->err[1] = PID->err[0];
  PID->err[0] = PID->ref - PID->fdb;

  PID->p_out  = PID->kp * PID->err[0];
  PID->i_out += PID->ki * PID->err[0];
  PID->d_out  = PID->kd * (PID->err[0] - PID->err[1]);
  PID->i_out=Limit_Min_Max(PID->i_out, -PID->i_max, PID->i_max);
  
//  if(PID->err[0]>20)PID->i_out=0;
	
  PID->output = PID->p_out + PID->i_out + PID->d_out;
  PID->output=Limit_Min_Max(PID->output, -PID->out_max, PID->out_max);
  return PID->output;
}

/**
 * @brief 限制一个整数变量 value 在指定的最小值 min 和最大值 max 之间
 * @param value 输入值
 * @param min 最小值
 * @param max 最大值
 * @return 
 */
float Limit_Min_Max(float value,float min,float max)
{
	if(value<min)
		return min;
	else if(value>max)
		return max;
	else return value;
}

