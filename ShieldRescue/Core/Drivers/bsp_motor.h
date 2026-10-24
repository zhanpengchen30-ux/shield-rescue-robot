#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

#include "stm32f1xx_hal.h"

void Motor_Init(void);
void Chassis_Control_O_Mecanum(int16_t vx, int16_t vy, int16_t wz);
void Chassis_SetVelocity(int16_t vx, int16_t vy, int16_t wz);
int16_t Get_Motor_Speed(uint8_t motor_id);

extern float target_speed[4];

#endif /* __BSP_MOTOR_H */
