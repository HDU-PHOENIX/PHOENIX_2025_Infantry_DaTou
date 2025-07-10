#ifndef __SHOOT_H__
#define __SHOOT_H__


#define Single_Angle 1024
//#define M2006_Speed -2000
#define Speed17mm_PT 200

void Shoot_Reload_Choose(void);
void Shoot_Remote_Control(void);
void Shoot_Move(void);
void Shoot_PID_Init_ALL(void);
void Shoot_fPidCalc(void);
void Shoot_PID_Clean_ALL(void);
void Shoot_Stop(void);
void Speed17mm_Control(void);
void Shoot_KeyBoard_Control(void);

#endif


