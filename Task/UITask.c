#include "UITask.h"

extern Moto_GM6020_t GM6020_Yaw;
extern Computer_Rx_Message_t Computer_Rx_Message;

void yaw_error_cal(void);

uint8_t chassis_gyro[4][200]={{0},{0},{0},{0}};
uint8_t chassis_gyro1[4][200]={{0},{0},{0},{0}};
uint8_t	zhunxing1[7][200]={{0},{0},{0},{0},{0},{0},{0}};

void UI_Task(void const * argument)
{
	uint16_t UI_PushUp_Counter = 301;
    while(1)
	{
        UI_PushUp_Counter++;
		draw_five_line(chassis_gyro1,0,huart6,pose);
		yaw_error_cal();
		draw_five_line(chassis_gyro1,0,huart6,Pose_Modify);
//			draw_seven_line(zhunxing1,0,huart6,zhunxin);
			
		if(UI_PushUp_Counter %= 8)
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
			draw_seven_line(zhunxing1,0,huart6,zhunxin2);
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
