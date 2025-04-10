#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "main.h"
#include "usart.h"
#include "Chassis.h"
#include "Gimbal.h"
#include "Shoot.h"
#include "dvc_supercap.h"


#define    KEY_PRESSED_OFFSET_W        ((uint16_t)0x01<<0)
#define    KEY_PRESSED_OFFSET_S        ((uint16_t)0x01<<1)
#define    KEY_PRESSED_OFFSET_A        ((uint16_t)0x01<<2)
#define    KEY_PRESSED_OFFSET_D        ((uint16_t)0x01<<3)
#define    KEY_PRESSED_OFFSET_SHIFT    ((uint16_t)0x01<<4)
#define    KEY_PRESSED_OFFSET_CTRL     ((uint16_t)0x01<<5)
#define    KEY_PRESSED_OFFSET_Q        ((uint16_t)0x01<<6)
#define    KEY_PRESSED_OFFSET_E        ((uint16_t)0x01<<7)
#define    KEY_PRESSED_OFFSET_R        ((uint16_t)0x01<<8)
#define    KEY_PRESSED_OFFSET_F        ((uint16_t)0x01<<9)
#define    KEY_PRESSED_OFFSET_G        ((uint16_t)0x01<<10)
#define    KEY_PRESSED_OFFSET_Z        ((uint16_t)0x01<<11)
#define    KEY_PRESSED_OFFSET_X        ((uint16_t)0x01<<12)
#define    KEY_PRESSED_OFFSET_C        ((uint16_t)0x01<<13)
#define    KEY_PRESSED_OFFSET_V        ((uint16_t)0x01<<14)
#define    KEY_PRESSED_OFFSET_B        ((uint16_t)0x01<<15)

/* 检测键盘按键状态 
   若对应按键被按下，则逻辑表达式的值为1，否则为0 */
    #define    IF_KEY_PRESSED         (  RC.key.all  )
    #define    IF_KEY_PRESSED_W       ( (RC.key.all & KEY_PRESSED_OFFSET_W)    != 0 )
	#define    IF_KEY_PRESSED_S       ( (RC.key.all & KEY_PRESSED_OFFSET_S)    != 0 )
	#define    IF_KEY_PRESSED_A       ( (RC.key.all & KEY_PRESSED_OFFSET_A)    != 0 )
	#define    IF_KEY_PRESSED_D       ( (RC.key.all & KEY_PRESSED_OFFSET_D)    != 0 )
	#define    IF_KEY_PRESSED_Q       ( (RC.key.all & KEY_PRESSED_OFFSET_Q)    != 0 )
	#define    IF_KEY_PRESSED_E       ( (RC.key.all & KEY_PRESSED_OFFSET_E)    != 0 )
	#define    IF_KEY_PRESSED_G       ( (RC.key.all & KEY_PRESSED_OFFSET_G)    != 0 )
	#define    IF_KEY_PRESSED_X       ( (RC.key.all & KEY_PRESSED_OFFSET_X)    != 0 )
	#define    IF_KEY_PRESSED_Z       ( (RC.key.all & KEY_PRESSED_OFFSET_Z)    != 0 )
	#define    IF_KEY_PRESSED_C       ( (RC.key.all & KEY_PRESSED_OFFSET_C)    != 0 )
	#define    IF_KEY_PRESSED_B       ( (RC.key.all & KEY_PRESSED_OFFSET_B)    != 0 )
	#define    IF_KEY_PRESSED_V       ( (RC.key.all & KEY_PRESSED_OFFSET_V)    != 0 )
	#define    IF_KEY_PRESSED_F       ( (RC.key.all & KEY_PRESSED_OFFSET_F)    != 0 )
	#define    IF_KEY_PRESSED_R       ( (RC.key.all & KEY_PRESSED_OFFSET_R)    != 0 )
	#define    IF_KEY_PRESSED_CTRL    ( (RC.key.all & KEY_PRESSED_OFFSET_CTRL) != 0 )
	#define    IF_KEY_PRESSED_SHIFT   ( (RC.key.all & KEY_PRESSED_OFFSET_SHIFT) != 0 )
typedef struct
{
	//摇杆
	int16_t ch0;
	int16_t ch1;
	int16_t ch2;
	int16_t ch3;

	//三位开关
	int8_t s1;
	int8_t s2;

	//鼠标
    struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;
	//按键（Bit0 -- W 键Bit1 -- S 键Bit2 -- A 键Bit3 -- D 键Bit4 -- Q 键Bit5 -- E 键Bit6 -- Shift 键Bit7 -- Ctrl 键）
    struct
    {
        uint16_t all;
    } key;
	//wheel
	int16_t wheel;
}RC_t;

void RC_Processing_Data(void);
void Control_Mode_Choose(void);
void Car_Init(void);

#endif


