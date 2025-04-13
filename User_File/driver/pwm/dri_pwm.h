#ifndef BSP_IMU_PWM_H
#define BSP_IMU_PWM_H

extern TIM_HandleTypeDef htim10;

void imu_pwm_set(uint16_t pwm);
#endif
