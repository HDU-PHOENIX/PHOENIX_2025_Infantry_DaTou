#ifndef __tuxin_h
#define __tuxin_h

#include "judge.h"

extern uint8_t Judge_Self_ID;
extern uint8_t Tx_buff[5][50];
extern uint8_t Tx_buff_seven[5][200];
extern uint32_t Change_Data[7][12];
extern uint32_t Change_arrow0[7][12];
extern uint32_t Change_arrow1[7][12];
extern uint32_t Change_capacitance[7][12];
extern uint32_t Data_P[7][12];
extern uint32_t Data_P[7][12];
extern uint32_t pose[5][12];
extern uint32_t line[12];
extern uint32_t Pose_Modify[5][12];
extern uint32_t zhunxin[7][12];
extern uint32_t zimiao[1][12];
extern uint32_t zhunxin2[7][12];
typedef enum
{
    add = 1,                         //增加
    modify = 2,						 //修改
    Delete = 3 						 //删除
} operate_type;


typedef enum
{
    straight_line = 0,				//直线
    rectangle,						//矩形
    circle,							//整圆
    oval,							//椭圆
    arc,							//圆弧
    floating,						//浮点数
    integer,						//整数
    string							//字符
} graphic_type;

typedef enum
{
    red_blue = 0,				
    yellow,						//黄色
    green,							//绿色
    orange,							//橙色
    zihong,							//紫簛E
    pink,								//粉色
    qing,								//青色
    black,							//黑色
    white								//白色
} color_type;

//对应客户端删除图形
typedef __packed struct
{
    uint8_t operate_type;			//图形操讈E
    uint8_t layer;						//图层数
} ext_client_custom_graphic_delete_t;


//图形数据
typedef __packed struct
{
    uint8_t graphic_name[3];		//图形脕E
    uint32_t operate_type: 3;		//图形操作，
    uint32_t graphic_type: 3;		//图形类型
    uint32_t layer: 4;						//图层数
    uint32_t color: 4;						//颜色
    uint32_t start_angle: 9;			//起始角度
    uint32_t end_angle: 9;				//终止角度
    uint32_t width: 10;					//线条宽度
    uint32_t start_x: 11;				//起点x坐眮E
    uint32_t start_y: 11;				//起点y坐眮E
    uint32_t radius: 10;					//字体大小或半径
    uint32_t end_x: 11;					//终点x坐眮E
    uint32_t end_y: 11;					//终点y坐眮E
} graphic_data_struct_t;



//客户端绘制一个图形 数据的内容 ID：0x0101
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct;
} ext_client_custom_graphic_single_t;

//客户端绘制两个图形	数据的内容 ID：0x0102
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct[2];
} ext_client_custom_graphic_double_t;

//客户端绘制五个图形  数据的内容 ID：0x0103
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct[5];
} ext_client_custom_graphic_five_t;

//客户端绘制七个图形  数据的内容 ID：0x0104
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct[7];
} ext_client_custom_graphic_seven_t;

//客户端绘制字穪E	数据的内容 ID：0x0110
typedef __packed struct
{
    graphic_data_struct_t grapic_data_struct;
    uint8_t data[30];
} ext_client_custom_character_t;

typedef __packed struct
{
    ext_client_custom_graphic_delete_t client_custom_graphic_delete; 	//客户端图形删除
    ext_client_custom_graphic_single_t show_single;										//客户端绘制一个图形
    ext_client_custom_graphic_double_t show_double;										//客户端绘制两个图形
    ext_client_custom_graphic_five_t   show_five;											//客户端绘制五个图形
    ext_client_custom_graphic_seven_t  show_seven;                    //客户端绘制七个图形
    ext_client_custom_character_t      show_char;											//客户端绘制字穪E
} operate_data_t;

typedef struct
{
    frame_header_t frame_header;																	//帧头
    uint16_t cmd_id;																				//命令码
    ext_student_interactive_header_data_t student_interactive_header; 	                            //数据段头结构
    operate_data_t operate_data;																	//操作数据类型
    uint16_t frame_tail;																			//帧尾（16位crc校验）
} judge_show_data_t;

typedef struct
{
    frame_header_t frame_header;
    uint16_t cmd_id;
    ext_client_custom_graphic_delete_t  ext_client_custom_graphic_delete;
} graphic_data_delete_t;

void draw_a_line(uint8_t (*txbuff)[50],uint8_t i,int Start_x, int Start_y,int End_x, int End_y,uint8_t Operate_type,int color, UART_HandleTypeDef UART);
void draw_a_cricle(uint8_t (*txbuff)[50],uint8_t i,int x, int y,uint8_t Operate_type, UART_HandleTypeDef UART);
void draw_a_string(uint8_t (*txbuff)[50],uint8_t i,uint8_t *str,uint8_t len, uint16_t start_x,uint16_t start_y, uint8_t Operate_type, UART_HandleTypeDef UART);
void show_str(uint8_t str[],uint8_t len,uint8_t layer, uint16_t start_x,uint16_t start_y, operate_type operate, uint8_t j,UART_HandleTypeDef UART);
void draw_CHASSIS_GYROSCOPE(uint8_t (*txbuff)[200], uint8_t i, UART_HandleTypeDef UART, uint32_t (*Data)[12]);
void draw_five_line(uint8_t (*txbuff)[200], uint8_t i, UART_HandleTypeDef UART, uint32_t (*Data)[12]);
	
void Delete_All(uint8_t (*txbuff)[50],uint8_t i, UART_HandleTypeDef UART);
void draw_seven_line(uint8_t (*txbuff)[200], uint8_t i, UART_HandleTypeDef UART, uint32_t (*Data)[12]);

void draw_seven_cricle(uint8_t (*txbuff)[200],uint8_t i,UART_HandleTypeDef UART,uint32_t (*Data)[12]);
void Line_of_sight(UART_HandleTypeDef UART);
void shoot_show(UART_HandleTypeDef UART);
#endif

