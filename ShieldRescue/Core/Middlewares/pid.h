#ifndef __PID_H
#define __PID_H

#include "main.h"

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    
    float integral;      // 积分项
    float prev_error;    // 上一次的误差
    float integral_limit;// 积分限幅 (防止积分饱和)
    float out_limit;     // 输出限幅
} PID_Controller;

// 初始化PID参数
void PID_Init(PID_Controller *pid, float kp, float ki, float kd, float integral_limit, float out_limit);

// 计算PID输出
float PID_Compute(PID_Controller *pid, float target, float current);

#endif
