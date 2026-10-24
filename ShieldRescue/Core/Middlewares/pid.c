// pid.c
#include "pid.h"

#define PID_DT 0.01f  // 10ms 的控制周期对应 0.01 秒的时间步长

void PID_Init(PID_Controller *pid, float kp, float ki, float kd, float integral_limit, float out_limit) {
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->integral = 0;
    pid->prev_error = 0;
    pid->integral_limit = integral_limit;
    pid->out_limit = out_limit;
}

float PID_Compute(PID_Controller *pid, float target, float current) {
    float error = target - current;
    
    // 积分项引入时间步长 dt，消除控制周期耦合
    pid->integral += error * PID_DT;
    
    // 积分限幅 (防积分饱和)
    if (pid->integral > pid->integral_limit) pid->integral = pid->integral_limit;
    if (pid->integral < -pid->integral_limit) pid->integral = -pid->integral_limit;
    
    // 【修正 2】：微分项引入时间步长 dt，消除控制周期耦合
    float derivative = (error - pid->prev_error) / PID_DT;
    
    // 输出计算
    float output = (pid->Kp * error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);
    
    // 输出限幅
    if (output > pid->out_limit) output = pid->out_limit;
    if (output < -pid->out_limit) output = -pid->out_limit;
    
    pid->prev_error = error;
    return output;
}
