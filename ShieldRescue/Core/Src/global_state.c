// global_state.c
#include "global_state.h"

// 定义并初始化全局变量
volatile ArmState_t g_arm_state = ARM_STATE_IDLE;
volatile ChassisState_t g_chassis_state = CHASSIS_STATE_IDLE;
volatile bool g_laser_trigger = false;

/**
 * @brief 设置机械臂状态
 */
void Set_Arm_State(ArmState_t new_state)
{
    g_arm_state = new_state;
}

/**
 * @brief 设置底盘状态
 */
void Set_Chassis_State(ChassisState_t new_state)
{
    g_chassis_state = new_state;
}
