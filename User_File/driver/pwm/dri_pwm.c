/**
 * @file dri_pwm.c
 * @brief pwm相关函数
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @date 2025-4-11
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */
#include "dri_pwm.h"

#include "stdint.h"
#include "main.h"
/**
 * @brief 设定PWM的比较值
 * @param pwm 比较值的值
 */
void imu_pwm_set(uint16_t pwm)
{
    __HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, pwm);
}
