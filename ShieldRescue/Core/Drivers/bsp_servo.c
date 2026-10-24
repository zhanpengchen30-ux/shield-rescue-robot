// bsp_servo.c
#include "bsp_servo.h"
#include "tim.h"

void Servo_Init(void) {
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
}

// 统一封装：映射 0-180度 -> 500-2500 (ARR=19999)
void Servo_SetAngle(uint8_t channel, float angle) {
	
	  if (angle < 0.0f)   angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;
	
    uint16_t pulse = (uint16_t)(500 + (angle * 2000.0 / 180.0));
    
    switch(channel) {
        case 1: __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, pulse); break;
        case 2: __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, pulse); break;
        case 3: __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, pulse); break;
        case 4: __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, pulse); break;
    }
}
