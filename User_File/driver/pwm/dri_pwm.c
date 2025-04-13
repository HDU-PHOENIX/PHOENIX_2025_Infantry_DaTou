/**
 * @file dri_pwm.c
 * @brief pwm��غ���
 * @author He WenXuan(hewenxuan040923@gmail.com)
 * @date 2025-4-11
 * @version 1.0
 * @copyright HDU-PHOENIX (c) 2025
 */
#include "dri_pwm.h"

#include "stdint.h"
#include "main.h"
/**
 * @brief �趨PWM�ıȽ�ֵ
 * @param pwm �Ƚ�ֵ��ֵ
 */
void imu_pwm_set(uint16_t pwm)
{
    __HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, pwm);
}
