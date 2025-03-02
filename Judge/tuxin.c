#include "judge.h"
#include "tuxin.h"
#include "crc.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

judge_show_data_t show_data = {0};
uint8_t Tx_buff[5][50] = {0};
uint8_t Tx_buff_seven[5][200] = {0};

//绘图段显示数据
void draw_a_cricle(uint8_t (*txbuff)[50], uint8_t i, int x, int y, uint8_t Operate_type, UART_HandleTypeDef UART)
{
    uint32_t length = 0;
    //按要求帖齑帧头
    length = sizeof(show_data.frame_header) + sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_single.grapic_data_struct) + sizeof(show_data.cmd_id) + sizeof(show_data.frame_tail);
    show_data.frame_header.SOF = Judge_Data_SOF;
    show_data.frame_header.Data_Length = sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_single.grapic_data_struct);
    show_data.frame_header.Seq = 0;
    memcpy(txbuff + i, &show_data.frame_header, LEN_HEADER);
    Append_CRC8_Check_Sum(*(txbuff + i), LEN_HEADER);
    //发送时低八位在前
    show_data.cmd_id = 0x0301;
    memcpy(txbuff[i] + CMD_ID, &show_data.cmd_id, LEN_CMDID);
    show_data.student_interactive_header.data_cmd_id = client_custom_graphic_single_id;
    show_data.student_interactive_header.sender_ID = Judge_Self_ID;
    show_data.student_interactive_header.receiver_ID = choose_client(Judge_Self_ID);
    memcpy(txbuff[i] + STU_HEADER, &show_data.student_interactive_header, LEN_STU_HEAD);
    show_data.operate_data.show_single.grapic_data_struct.graphic_name[0] = 'r';
    show_data.operate_data.show_single.grapic_data_struct.graphic_name[1] = 'o';
    show_data.operate_data.show_single.grapic_data_struct.graphic_name[2] = 'd';
    show_data.operate_data.show_single.grapic_data_struct.operate_type = Operate_type;//图形操作
    show_data.operate_data.show_single.grapic_data_struct.graphic_type = circle;
    show_data.operate_data.show_single.grapic_data_struct.layer = i;//图层
    show_data.operate_data.show_single.grapic_data_struct.color = 1;
    show_data.operate_data.show_single.grapic_data_struct.width = 5;//线条宽度
    show_data.operate_data.show_single.grapic_data_struct.start_x = x;
    show_data.operate_data.show_single.grapic_data_struct.start_y =	y;
    show_data.operate_data.show_single.grapic_data_struct.radius = 30;//半径

    memcpy(txbuff[i] + STU_DATA, &show_data.operate_data.show_single.grapic_data_struct, LEN_SINGLE_GRAPH);
    Append_CRC16_Check_Sum(*(txbuff + i), length);
    HAL_UART_Transmit(&UART, *(txbuff + i), length,5);
    memset(txbuff + i, 0, 50);
}
void draw_CHASSIS_GYROSCOPE(uint8_t (*txbuff)[200], uint8_t i, UART_HandleTypeDef UART, uint32_t (*Data)[12])
{
	
    uint32_t length = 0;
    //按要求帖齑帧头
    length = sizeof(show_data.frame_header) + sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_seven.grapic_data_struct) + sizeof(show_data.cmd_id) + sizeof(show_data.frame_tail);
    show_data.frame_header.SOF = Judge_Data_SOF;
    show_data.frame_header.Data_Length = sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_seven.grapic_data_struct);
    show_data.frame_header.Seq = 0;
    memcpy(txbuff + i, &show_data.frame_header, LEN_HEADER);
    Append_CRC8_Check_Sum(*(txbuff + i), LEN_HEADER);
    //发送时低八位在前
    show_data.cmd_id = 0x0301;
    memcpy(txbuff[i] + 3, &show_data.cmd_id, LEN_CMDID);
    show_data.student_interactive_header.data_cmd_id = client_custom_graphic_seven_id;
    show_data.student_interactive_header.sender_ID = Judge_Self_ID;
    show_data.student_interactive_header.receiver_ID = choose_client(Judge_Self_ID);
    memcpy(txbuff[i] + STU_HEADER, &show_data.student_interactive_header, LEN_STU_HEAD);
    for(int j = 0; j < 3; j++)
    {
        show_data.operate_data.show_seven.grapic_data_struct[j].graphic_name[0] = *(Data[j]+7);
        show_data.operate_data.show_seven.grapic_data_struct[j].graphic_name[1] = *(Data[j]+8);
        show_data.operate_data.show_seven.grapic_data_struct[j].graphic_name[2] = j;
        show_data.operate_data.show_seven.grapic_data_struct[j].operate_type = *(Data[j]+0);//Operate_type;//图形操讈E
        show_data.operate_data.show_seven.grapic_data_struct[j].graphic_type = *(Data[j]+1);//;//图形类型
        show_data.operate_data.show_seven.grapic_data_struct[j].layer = *(Data[j]+2);//5;//图瞾E
        show_data.operate_data.show_seven.grapic_data_struct[j].color = *(Data[j]+3);//1;//颜色
        show_data.operate_data.show_seven.grapic_data_struct[j].start_angle = *(Data[j]+4);//10;//
        show_data.operate_data.show_seven.grapic_data_struct[j].end_angle = *(Data[j]+5);//10;//
        show_data.operate_data.show_seven.grapic_data_struct[j].width = *(Data[j]+6);//10;//线条窥胰
        show_data.operate_data.show_seven.grapic_data_struct[j].start_x = *(Data[j]+7);//x;
        show_data.operate_data.show_seven.grapic_data_struct[j].start_y =	*(Data[j]+8);//y;
        show_data.operate_data.show_seven.grapic_data_struct[j].radius = *(Data[j]+9);//10;//皝E?
        show_data.operate_data.show_seven.grapic_data_struct[j].end_x = *(Data[j]+10);//10;//皝E?
        show_data.operate_data.show_seven.grapic_data_struct[j].end_y = *(Data[j]+11);//10;//皝E?
    }

    //show_single
    memcpy(txbuff[i] + STU_DATA, &show_data.operate_data.show_five.grapic_data_struct, LEN_THREE_GRAPH);
    Append_CRC16_Check_Sum(*(txbuff + i), length);
    HAL_UART_Transmit(&UART, *(txbuff + i), length,30);		
	memset(txbuff + i, 0, 200);
}
void Delete_All(uint8_t (*txbuff)[50],uint8_t i, UART_HandleTypeDef UART)
{
    uint32_t length = 0;
    //按要求帖齑帧头
    length = sizeof(show_data.frame_header) + sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_single.grapic_data_struct) + sizeof(show_data.cmd_id) + sizeof(show_data.frame_tail);
    show_data.frame_header.SOF = Judge_Data_SOF;
    show_data.frame_header.Data_Length = sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_single.grapic_data_struct);
    show_data.frame_header.Seq = 0;
    memcpy(txbuff + i, &show_data.frame_header, LEN_HEADER);
    Append_CRC8_Check_Sum(*(txbuff + i), LEN_HEADER);
    //发送时低八位在前
    show_data.cmd_id = 0x0301;
    memcpy(txbuff[i] + CMD_ID, &show_data.cmd_id, LEN_CMDID);
    show_data.student_interactive_header.data_cmd_id = client_custom_graphic_delete_id;
    show_data.student_interactive_header.sender_ID = Judge_Self_ID;
    show_data.student_interactive_header.receiver_ID = choose_client(Judge_Self_ID);
    memcpy(txbuff[i] + STU_HEADER, &show_data.student_interactive_header, LEN_STU_HEAD);

    show_data.operate_data.client_custom_graphic_delete.operate_type = 2;
    show_data.operate_data.client_custom_graphic_delete.layer=9;

    memcpy(txbuff[i] + STU_DATA, &show_data.operate_data.client_custom_graphic_delete, 2);
    Append_CRC16_Check_Sum(*(txbuff + i), length);
    HAL_UART_Transmit(&UART, *(txbuff + i), length,50);
    memset(txbuff + i, 0, 50);
}

void draw_a_line(uint8_t (*txbuff)[50], uint8_t i, int Start_x, int Start_y, int End_x, int End_y, uint8_t Operate_type, int color,UART_HandleTypeDef UART)
{
    uint32_t length = 0;
    //按要求帖齑帧头
    length = sizeof(show_data.frame_header) + sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_single.grapic_data_struct) + sizeof(show_data.cmd_id) + sizeof(show_data.frame_tail);
    show_data.frame_header.SOF = Judge_Data_SOF;
    show_data.frame_header.Data_Length = sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_single.grapic_data_struct);
    show_data.frame_header.Seq = 0;
    memcpy(txbuff + i, &show_data.frame_header, LEN_HEADER);
    Append_CRC8_Check_Sum(*(txbuff + i), LEN_HEADER);
    //发送时低八位在前
    show_data.cmd_id = 0x0301;
    memcpy(txbuff[i] + CMD_ID, &show_data.cmd_id, LEN_CMDID);
    show_data.student_interactive_header.data_cmd_id = client_custom_graphic_single_id;
    show_data.student_interactive_header.sender_ID = Judge_Self_ID;
    show_data.student_interactive_header.receiver_ID = choose_client(Judge_Self_ID);
    memcpy(txbuff[i] + STU_HEADER, &show_data.student_interactive_header, LEN_STU_HEAD);
    show_data.operate_data.show_single.grapic_data_struct.graphic_name[0] = 'r';
    show_data.operate_data.show_single.grapic_data_struct.graphic_name[1] = 'o';
    show_data.operate_data.show_single.grapic_data_struct.graphic_name[2] = 'd';
    show_data.operate_data.show_single.grapic_data_struct.operate_type = Operate_type;//图形操作
    show_data.operate_data.show_single.grapic_data_struct.graphic_type = straight_line;//图形类型
    show_data.operate_data.show_single.grapic_data_struct.layer = i;//图层
    show_data.operate_data.show_single.grapic_data_struct.color = color;//颜色
    show_data.operate_data.show_single.grapic_data_struct.width = 2;//线条宽度
    show_data.operate_data.show_single.grapic_data_struct.start_x = Start_x;
    show_data.operate_data.show_single.grapic_data_struct.start_y =	Start_y;
    show_data.operate_data.show_single.grapic_data_struct.end_x = End_x;//10;//皝E?
    show_data.operate_data.show_single.grapic_data_struct.end_y = End_y;//10;//

    memcpy(txbuff[i] + STU_DATA, &show_data.operate_data.show_single.grapic_data_struct, LEN_SINGLE_GRAPH);
    Append_CRC16_Check_Sum(*(txbuff + i), length);
    HAL_UART_Transmit(&UART, *(txbuff + i), length,5);
    memset(txbuff + i, 0, 50);
}


//字 字数 
void show_str(uint8_t str[],uint8_t len,uint8_t layer, uint16_t start_x,uint16_t start_y, operate_type operate, uint8_t j,UART_HandleTypeDef UART)
{
	uint32_t length = 0;
	uint8_t txbuff[200] = {0};
		//按要求填写帧头
	length = sizeof(show_data.frame_header) + sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_single.grapic_data_struct) + 30 + sizeof(show_data.cmd_id) + sizeof(show_data.frame_tail);
	show_data.frame_header.SOF = Judge_Data_SOF;
	show_data.frame_header.Data_Length = sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_char);
	show_data.frame_header.Seq = 0;
	memcpy(txbuff,&show_data.frame_header,LEN_HEADER);
	Append_CRC8_Check_Sum(txbuff,LEN_HEADER);
	
	show_data.cmd_id = 0x0301;
	memcpy(txbuff + CMD_ID,&show_data.cmd_id,LEN_CMDID);
	show_data.student_interactive_header.data_cmd_id = client_custom_character_id;
    show_data.student_interactive_header.sender_ID = Judge_Self_ID;//当前机器人id;
    show_data.student_interactive_header.receiver_ID = choose_client(Judge_Self_ID);//该机器人对应的客户端id
	memcpy(txbuff + STU_HEADER,&show_data.student_interactive_header,LEN_STU_HEAD);
	show_data.operate_data.show_char.grapic_data_struct.graphic_name[0] = 's';
	show_data.operate_data.show_char.grapic_data_struct.graphic_name[1] = '1';
	show_data.operate_data.show_char.grapic_data_struct.graphic_name[2] = j;
	show_data.operate_data.show_char.grapic_data_struct.operate_type = operate;
	show_data.operate_data.show_char.grapic_data_struct.graphic_type = string;
	show_data.operate_data.show_char.grapic_data_struct.layer = layer;
	show_data.operate_data.show_char.grapic_data_struct.color = yellow;
	show_data.operate_data.show_char.grapic_data_struct.start_angle = 30;	//字体大小
	show_data.operate_data.show_char.grapic_data_struct.end_angle = len;	//字体长度
	show_data.operate_data.show_char.grapic_data_struct.width = 3;
	show_data.operate_data.show_char.grapic_data_struct.start_x = start_x;
	show_data.operate_data.show_char.grapic_data_struct.start_y = start_y;
	
	for(int i = 0; i < len; i++)
	{
		show_data.operate_data.show_char.data[i] = str[i];
	}
	memcpy(txbuff + STU_DATA ,&show_data.operate_data.show_char.grapic_data_struct, LEN_SINGLE_GRAPH + 30);
	Append_CRC16_Check_Sum(txbuff,length);
	HAL_UART_Transmit(&UART,txbuff,length,0xffff);	
	memset(txbuff,0,sizeof(txbuff));
}

void draw_seven_line(uint8_t (*txbuff)[200], uint8_t i, UART_HandleTypeDef UART, uint32_t (*Data)[12])
{
    uint32_t length = 0;
    //按要求帖齑帧头
    length = sizeof(show_data.frame_header) + sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_seven.grapic_data_struct) + sizeof(show_data.cmd_id) + sizeof(show_data.frame_tail);
    show_data.frame_header.SOF = Judge_Data_SOF;
    show_data.frame_header.Data_Length = sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_seven.grapic_data_struct);
    show_data.frame_header.Seq = 0;
    memcpy(txbuff + i, &show_data.frame_header, LEN_HEADER);
    Append_CRC8_Check_Sum(*(txbuff + i), LEN_HEADER);
    //发送时低八位在前
    show_data.cmd_id = 0x0301;
    memcpy(txbuff[i] + CMD_ID, &show_data.cmd_id, LEN_CMDID);
    show_data.student_interactive_header.data_cmd_id = client_custom_graphic_seven_id;
    show_data.student_interactive_header.sender_ID = Judge_Self_ID;
    show_data.student_interactive_header.receiver_ID = choose_client(Judge_Self_ID);
    memcpy(txbuff[i] + STU_HEADER, &show_data.student_interactive_header, LEN_STU_HEAD);
    for(int j = 0; j < 7; j++)
    {
        show_data.operate_data.show_seven.grapic_data_struct[j].graphic_name[0] = *(Data[j]+2);
        show_data.operate_data.show_seven.grapic_data_struct[j].graphic_name[1] = i;
        show_data.operate_data.show_seven.grapic_data_struct[j].graphic_name[2] = j;
        show_data.operate_data.show_seven.grapic_data_struct[j].operate_type = *(Data[j]+0);//Operate_type;//图形操作
        show_data.operate_data.show_seven.grapic_data_struct[j].graphic_type = *(Data[j]+1);//;//图形类型
        show_data.operate_data.show_seven.grapic_data_struct[j].layer = *(Data[j]+2);//5;//图层
        show_data.operate_data.show_seven.grapic_data_struct[j].color = *(Data[j]+3);//1;//颜色
        show_data.operate_data.show_seven.grapic_data_struct[j].start_angle = *(Data[j]+4);//10;//
        show_data.operate_data.show_seven.grapic_data_struct[j].end_angle = *(Data[j]+5);//10;//
        show_data.operate_data.show_seven.grapic_data_struct[j].width = *(Data[j]+6);//10;//线条宽度
        show_data.operate_data.show_seven.grapic_data_struct[j].start_x = *(Data[j]+7);//x;
        show_data.operate_data.show_seven.grapic_data_struct[j].start_y =	*(Data[j]+8);//y;
        show_data.operate_data.show_seven.grapic_data_struct[j].radius = *(Data[j]+9);
        show_data.operate_data.show_seven.grapic_data_struct[j].end_x = *(Data[j]+10);
        show_data.operate_data.show_seven.grapic_data_struct[j].end_y = *(Data[j]+11);
    }

    //show_single
    memcpy(txbuff[i] + STU_DATA, &show_data.operate_data.show_seven.grapic_data_struct, LEN_SEVEN_GRAPH);
    Append_CRC16_Check_Sum(*(txbuff + i), length);
    HAL_UART_Transmit(&UART, *(txbuff + i), length,50);

    memset(txbuff + i, 0, 200);
}

void draw_five_line(uint8_t (*txbuff)[200], uint8_t i, UART_HandleTypeDef UART, uint32_t (*Data)[12])
{
    uint32_t length = 0;
    //按要求帖齑帧头
    length = sizeof(show_data.frame_header) + sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_five.grapic_data_struct) + sizeof(show_data.cmd_id) + sizeof(show_data.frame_tail);
    show_data.frame_header.SOF = Judge_Data_SOF;
    show_data.frame_header.Data_Length = sizeof(show_data.student_interactive_header) + sizeof(show_data.operate_data.show_five.grapic_data_struct);
    show_data.frame_header.Seq = 0;
    memcpy(txbuff + i, &show_data.frame_header, LEN_HEADER);
    Append_CRC8_Check_Sum(*(txbuff + i), LEN_HEADER);
    //发送时低八位在前
    show_data.cmd_id = 0x0301;
    memcpy(txbuff[i] + CMD_ID, &show_data.cmd_id, LEN_CMDID);
    show_data.student_interactive_header.data_cmd_id = client_custom_graphic_five_id;
    show_data.student_interactive_header.sender_ID = Judge_Self_ID;
    show_data.student_interactive_header.receiver_ID = choose_client(Judge_Self_ID);
    memcpy(txbuff[i] + STU_HEADER, &show_data.student_interactive_header, LEN_STU_HEAD);
    for(int j = 0; j < 5; j++)
    {
        show_data.operate_data.show_five.grapic_data_struct[j].graphic_name[0] = '0';
        show_data.operate_data.show_five.grapic_data_struct[j].graphic_name[1] = '0';
        show_data.operate_data.show_five.grapic_data_struct[j].graphic_name[2] = j;
        show_data.operate_data.show_five.grapic_data_struct[j].operate_type = *(Data[j]+0);//Operate_type;//图形操作
        show_data.operate_data.show_five.grapic_data_struct[j].graphic_type = *(Data[j]+1);//;//图形类型
        show_data.operate_data.show_five.grapic_data_struct[j].layer = *(Data[j]+2);//5;//图层
        show_data.operate_data.show_five.grapic_data_struct[j].color = *(Data[j]+3);//1;//颜色
        show_data.operate_data.show_five.grapic_data_struct[j].start_angle = *(Data[j]+4);//起始角度
        show_data.operate_data.show_five.grapic_data_struct[j].end_angle = *(Data[j]+5);//终点角度
        show_data.operate_data.show_five.grapic_data_struct[j].width = *(Data[j]+6);//10;//线条宽度
        show_data.operate_data.show_five.grapic_data_struct[j].start_x = *(Data[j]+7);//起点x
        show_data.operate_data.show_five.grapic_data_struct[j].start_y = *(Data[j]+8);//起点y
        show_data.operate_data.show_five.grapic_data_struct[j].radius = *(Data[j]+9);//半径
        show_data.operate_data.show_five.grapic_data_struct[j].end_x = *(Data[j]+10);//终点x
        show_data.operate_data.show_five.grapic_data_struct[j].end_y = *(Data[j]+11);//终点y
    }

    //show_single
    memcpy(txbuff[i] + STU_DATA, &show_data.operate_data.show_five.grapic_data_struct, LEN_FIVE_GRAPH);
    Append_CRC16_Check_Sum(*(txbuff + i), length);
		while(HAL_UART_GetState(&UART) == HAL_UART_STATE_BUSY_TX)
		{
			osDelay(1);
		}
		HAL_UART_Transmit(&UART, *(txbuff + i), length,20);
		
		memset(txbuff + i, 0, 200);
    
}

////operate_type graphic_type layer color start_angle end_angle width start_x start_y radius end_x end_y
//uint32_t Change_Data[7][12]= {{add,oval,1,black,0,0,4,960,540,10,10,25},
//                            {add,oval,1,black,0,0,4,960,200,10,10,25},
//                            {add,circle,1,black,0,0,4,960,700,10,0,0}
//};//判断(之前)
//uint32_t Change_capacitance[7][12]={//电容
//												{add,straight_line,3,yellow,0,0,60,500,900,10,540,900},
//												{add,straight_line,3,yellow,0,0,60,550,900,10,590,900},
//												{add,straight_line,3,yellow,0,0,60,600,900,10,640,900},
//												{add,straight_line,3,yellow,0,0,60,650,900,10,690,900},
//												{add,straight_line,3,yellow,0,0,60,700,900,10,740,900}
//};
//uint32_t Data_S[7][12]={//S
//												{add,straight_line,5,yellow,0,0,5,500,800,10,470,800},
//												{add,straight_line,5,yellow,0,0,5,470,800,10,470,770},
//												{add,straight_line,5,yellow,0,0,5,470,770,10,500,770},
//												{add,straight_line,5,yellow,0,0,5,500,770,10,500,740},
//												{add,straight_line,5,yellow,0,0,5,500,740,10,470,740}
//};
//uint32_t Data_P[7][12]={//P
//												{add,straight_line,4,yellow,0,0,5,470,640,10,500,640},
//												{add,straight_line,4,yellow,0,0,5,500,640,10,500,610},
//												{add,straight_line,4,yellow,0,0,5,500,610,10,470,610},
//												{add,straight_line,4,yellow,0,0,5,470,640,10,470,580}
//};


//uint32_t Change_arrow0[7][12]= {{add,straight_line,2,yellow,0,0,4,1200,630,10,1200,840},
//                            {add,straight_line,2,yellow,0,0,4,1130,700,10,1270,700}//上
//};//指针
//uint32_t Change_arrow1[7][12]= {{modify,straight_line,2,yellow,0,0,4,1200,630,10,1200,840},
//                            {modify,straight_line,2,yellow,0,0,4,1130,700,10,1270,700}//上
//};

uint32_t pose[6][12]  = {
	{add,straight_line,6,green,0,0,3,1600,780,0,1600,830},//朝前的线
	{add,arc,6,green,30,330,3,1600,780,5,30,30},//大圆弧1600,780,5,30,30
	{add,arc,6,pink,330,30,3,1600,780,0,30,30},//底盘前面的圆弧
    };
uint32_t Pose_Modify[3][12]  = {
	{modify,straight_line,6,green,0,0,3,1600,780,0,1600,830},//朝前的线
	{modify,arc,6,green,30,330,3,1600,780,5,30,30},//大圆弧1600,780,5,30,30
	{modify,arc,6,pink,330,30,3,1600,780,0,30,30},//底盘前面的圆弧
	
    };

uint32_t zhunxin[7][12]={
////operate_type graphic_type layer color start_angle end_angle width start_x start_y radius end_x end_y	//
	//自瞄框
	{add,straight_line,4,white,0,0,3,699,321,0,699,749},// |
	{add,straight_line,4,white,0,0,3,699,321,0,1216,321},// —
	{add,straight_line,4,white,0,0,3,699,749,0,1216,749},// —
	{add,straight_line,4,white,0,0,3,1216,321,0,1216,749},// |
	
	{add,straight_line,4,white,0,0,2,0,0,0,0,0},
	{add,straight_line,4,white,0,0,2,0,0,0,0,0},
	{add,straight_line,4,white,0,0,2,0,0,0,0,0},
};
uint32_t zhunxin2[7][12]={
////operate_type graphic_type layer color start_angle end_angle width start_x start_y radius end_x end_y	//
//自瞄框	
	{modify,straight_line,4,white,0,0,3,699,321,0,699,749},// |
	{modify,straight_line,4,white,0,0,3,699,321,0,1216,321},// —
	{modify,straight_line,4,white,0,0,3,699,749,0,1216,749},// —
	{modify,straight_line,4,white,0,0,3,1216,321,0,1216,749},// |
	
	{modify,straight_line,4,white,0,0,2,0,0,0,0,0},
	{modify,straight_line,4,white,0,0,2,0,0,0,0,0},					
	{modify,straight_line,4,white,0,0,2,0,0,0,0,0},					
};

