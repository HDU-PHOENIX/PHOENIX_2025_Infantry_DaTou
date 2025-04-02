#include "UITask.h"

extern Moto_GM6020_t GM6020_Yaw;
extern Computer_Rx_Message_t Computer_Rx_Message;
extern Chassis_Speed_t Temp1_Chassis_Speed,Temp2_Chassis_Speed;
extern Car_Mode_t Car_Mode;
extern SuperPower_Rx_Message_t SuperPower_Rx_Message;
extern SuperPower_Mode_t SuperPower_Mode;
extern bool Shoot;
extern SuperPower_Switch_t SuperPower_Switch;
extern Moto_M3508_t M3508_Shoot[2];

void yaw_error_cal(void);

uint8_t chassis_gyro[4][200]={{0},{0},{0},{0}};
uint8_t chassis_gyro1[4][200]={{0},{0},{0},{0}};
uint8_t	zhunxing1[7][200]={{0},{0},{0},{0},{0},{0},{0}};
uint8_t Speed_Str[20] = {"   Shoot    W:10.23"};
uint8_t FOLLOW_Str[20] = {"FOLLOW"};
uint8_t NORMAL_Str[20] = {"NORMAL"};
uint8_t GYROSCOPE_Str[20] = {"GYROSCOPE"};
uint8_t SuperPower_Str[20] = {"CAP:"};

void UI_Task(void const * argument)
{
    uint16_t UI_PushUp_Counter = 0;
    while(1)
	{
		VOFT_Tx();
        UI_PushUp_Counter++;
		if(UI_PushUp_Counter>=65500)
			UI_PushUp_Counter = 0;
		if(UI_PushUp_Counter%401 == 0)
		{
            draw_five_line(chassis_gyro1,0,huart6,pose);    
		}
		if(UI_PushUp_Counter%331 == 0)
		{
            draw_seven_line(zhunxing1,1,huart6,zhunxin);					
		}
		if(UI_PushUp_Counter%391 == 0)
		{
            show_str(Speed_Str,20,3,700,80, add,5,huart6,yellow);
		}
		if(UI_PushUp_Counter%259 == 0)
		{
            show_str(NORMAL_Str,20,5,600,130, add,4,huart6,yellow);
		}
		if(UI_PushUp_Counter%499 == 0)
		{
            show_str(SuperPower_Str,20,2,1000,130,add,0,huart6,yellow);				
		}

		if(UI_PushUp_Counter%16 == 0)
		{
			if(Car_Mode.Action == NORMAL)
                show_str(NORMAL_Str,20,5,700,130,modify,4,huart6,yellow);
			if(Car_Mode.Action == GYROSCOPE)
                show_str(GYROSCOPE_Str,20,5,700,130,modify,4,huart6,yellow);                 
            if(Car_Mode.Action == FOLLOW)
                show_str(FOLLOW_Str,20,5,700,130,modify,4,huart6,yellow);				
		}

		if(UI_PushUp_Counter%13 == 0)
		{
            SuperPower_Str[4] = 48 + (SuperPower_Rx_Message.Stored_Energy/100)%10;
            SuperPower_Str[5] = 48 + (SuperPower_Rx_Message.Stored_Energy/10)%10;
			SuperPower_Str[6] = 48 + SuperPower_Rx_Message.Stored_Energy%10;
			uint8_t SuperPower_Color;
			if(SuperPower_Mode == SuperPower_On && SuperPower_Switch == SuperPower_Work)
			{
				SuperPower_Str[8] = 'O';
				SuperPower_Str[9] = 'N';
				SuperPower_Str[10] = ' ';
				SuperPower_Str[11] = ' ';
				SuperPower_Color = green;
			}else if(SuperPower_Mode == SuperPower_Off && SuperPower_Switch == SuperPower_Work)
			{
				SuperPower_Str[8] = 'O';
				SuperPower_Str[9] = 'F';
				SuperPower_Str[10] = 'F';
				SuperPower_Str[11] = ' ';
				SuperPower_Color = yellow;
			}else if(SuperPower_Switch == SuperPower_Stop)
			{
				SuperPower_Str[8] = 'S';
				SuperPower_Str[9] = 'T';
				SuperPower_Str[10] = 'O';
				SuperPower_Str[11] = 'P';
				SuperPower_Color = pink;
			}
            show_str(SuperPower_Str,20,2,1000,130,modify,0,huart6,SuperPower_Color);				
		}

        if(UI_PushUp_Counter%6 == 0)
		{
            yaw_error_cal();
            draw_five_line(chassis_gyro1,0,huart6,Pose_Modify);
		}

		if(UI_PushUp_Counter%7 == 0)
        {
			if(Computer_Rx_Message.find_bool=='1')
			{
                zhunxin2[0][3]=green;
				zhunxin2[1][3]=green;
				zhunxin2[2][3]=green;	
				zhunxin2[3][3]=green;	
			}else{
				zhunxin2[0][3]=pink;
				zhunxin2[1][3]=pink;
				zhunxin2[2][3]=pink;	
				zhunxin2[3][3]=pink;	
			}
				draw_seven_line(zhunxing1,1,huart6,zhunxin2);
        }
		if(UI_PushUp_Counter%11 == 0)
		{
			uint8_t Shoot_Color;			
			Speed_Str[14]=(int)(Temp2_Chassis_Speed.vw/10)%10+48;	    //第一位
			Speed_Str[15]=(int)(Temp2_Chassis_Speed.vw)%10+48;		    //第二位
			Speed_Str[17]=(int)(Temp2_Chassis_Speed.vw*10)%10+48;		//第三位
            Speed_Str[18]=(int)(Temp2_Chassis_Speed.vw*100)%10+48;		//第四位
			
			if(Shoot == false)
                Shoot_Color = pink;
			else if(M3508_Shoot[0].rotor_speed < 1000)
                Shoot_Color = yellow;
			else if(M3508_Shoot[0].rotor_speed > 4000)
                Shoot_Color = green;
			show_str(Speed_Str,20,3,700,80,modify,5,huart6,Shoot_Color);
		}
		vTaskDelay(10);
	}
}

/**
计算云台与底盘差角的UI图形起始角度与结束角度
大圆弧和小圆弧互补
*/
void yaw_error_cal(void)
{		
	float substarct_angle;	//云台和底盘编码器差值的角度 
	substarct_angle=(GM6020_Yaw.rotor_angle-Gimbal_Yaw_ZERO)*0.04394f;
	float start_angle,end_angle;
	start_angle = 30 + substarct_angle;
	if(start_angle < 0)
		start_angle += 360;
	end_angle = 330 + substarct_angle;
	if(end_angle > 360)
		end_angle -= 360;
	Pose_Modify[1][4] = (uint32_t)start_angle;
	Pose_Modify[1][5] = (uint32_t)end_angle;
	Pose_Modify[2][4] = (uint32_t)end_angle;
	Pose_Modify[2][5] = (uint32_t)start_angle;
}	
