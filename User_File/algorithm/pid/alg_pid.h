#ifndef __PID_H_
#define __PID_H_

#include "main.h"
#include "stm32f4xx.h"

typedef struct
{
  float kp;//比例系数
  float ki;//计数系数
  float kd;//微分系数
  float kf;//前馈系数
  float i_max;//积分限幅
  float out_max;//输出限幅
  float dead_zone;//死区
  float angle_max;//角度限幅
  float i_variable_min;//变速积分下限
  float i_variable_max;//变速积分上限
  float i_separated;//积分分离
  uint8_t d_first;//微分先行
  float target[2];//设定值
  float now[2];//实际值
  float integral;//积分值
  float err[2];//误差
  float p_out;//比例输出
  float i_out;//积分输出
  float d_out;//微分输出
  float f_out;//前馈输出
  float output;//总输出
}PID_struct_t;

void vPidInit(PID_struct_t *PID,float kp,float ki,float kd,float kf,float dead_zone,float angle_max,float i_variable_min,float i_variable_max,float i_separated,uint8_t d_first,float i_max,float out_max);
void vPidProtect(PID_struct_t *pid);
float fPidLimit(float value,float min,float max);
float fPidCalc(PID_struct_t *PID, float target, float now);

#endif


