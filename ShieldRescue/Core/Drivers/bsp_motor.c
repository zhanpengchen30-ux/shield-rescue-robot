#include "bsp_motor.h"
#include <math.h>

extern TIM_HandleTypeDef htim1;

// 定义 nav_task.c 中需要用到的全局目标速度数组
float target_speed[4] = {0};

// 1. 电机初始化函数（开启TIM1的4个PWM通道）
void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

/**
 * @brief 单个电机底层控制（适配 1000 差值 PWM 逻辑）
 * @param invert: 0表示不反转，1表示将方向反转
 */
static void Motor_Control(uint32_t channel, GPIO_TypeDef* dir_port, uint16_t dir_pin, int16_t speed, uint8_t invert)
{
    // 限制输入速度的上限（比如最大 800）
    int16_t max_speed = 800; 
    if (speed > max_speed)  speed = max_speed;
    if (speed < -max_speed) speed = -max_speed;

    if (speed >= 0) {
        // 正转
        HAL_GPIO_WritePin(dir_port, dir_pin, invert ? GPIO_PIN_RESET : GPIO_PIN_SET);   
    } 
    else {
        // 反转
        speed = -speed; 
        HAL_GPIO_WritePin(dir_port, dir_pin, invert ? GPIO_PIN_SET : GPIO_PIN_RESET); 
    }

    // 核心修改：使用 1000 的差值作为实际的 PWM 比较值
    // 这样当 speed = 200 时，pwm_val = 800；当 speed = 800 时，pwm_val = 200
    int16_t pwm_val = 1000 - speed; 

    // 限幅保护，确保在 0 ~ 1000 之间
    if (pwm_val > 1000) pwm_val = 1000;
    if (pwm_val < 0)    pwm_val = 0;

    __HAL_TIM_SET_COMPARE(&htim1, channel, (uint16_t)pwm_val);
}

// 3. O型麦轮底盘运动学控制
void Chassis_Control_O_Mecanum(int16_t vx, int16_t vy, int16_t wz)
{
    int16_t v_lf = vx - vy - wz; // 左前轮 (MOTORA)
    int16_t v_rf = vx + vy + wz; // 右前轮 (MOTORB)
    int16_t v_lr = vx + vy - wz; // 左后轮 (MOTORC)
    int16_t v_rr = vx - vy + wz; // 右后轮 (MOTORD)

    // 最后一个参数：0 = 正常方向， 1 = 方向反转
    Motor_Control(TIM_CHANNEL_1, GPIOE, GPIO_PIN_8,  v_lf, 0); // 左前轮：不反转
    Motor_Control(TIM_CHANNEL_3, GPIOE, GPIO_PIN_12, v_rf, 1); // 右前轮：反转
    Motor_Control(TIM_CHANNEL_2, GPIOE, GPIO_PIN_10, v_lr, 0); // 左后轮：不反转
    Motor_Control(TIM_CHANNEL_4, GPIOE, GPIO_PIN_15, v_rr, 1); // 右后轮：反转
}

// 4. 完美匹配 nav_task.c 的接口
void Chassis_SetVelocity(int16_t vx, int16_t vy, int16_t wz)
{
    Chassis_Control_O_Mecanum(vx, vy, wz);
}

int16_t Get_Motor_Speed(uint8_t motor_id)
{
    // 如果暂时没写编码器，返回0，防止报错
    return 0;
}
