// global_state.h
#ifndef __GLOBAL_STATE_H
#define __GLOBAL_STATE_H

#include "main.h"
#include <stdbool.h>  // 引入布尔类型支持，解决 bool/true/false 未定义问题

extern volatile bool g_laser_trigger;

// ==================== 1. 机械臂/云台独立状态枚举 ====================
typedef enum {
    ARM_STATE_IDLE = 0,
    ARM_STATE_LINE_TRACK_CENTER,   // 正中云台寻路姿态
    ARM_STATE_LINE_TRACK_LEFT,     // 左云台寻路姿态
    ARM_STATE_LINE_TRACK_RIGHT,    // 右云台寻路姿态
    ARM_STATE_SCAN_QR,             // 扫码姿态
    ARM_STATE_DETECT_EXPLOSIVE,    // 寻找/识别物姿态
    ARM_STATE_GRAB_EXPLOSIVE,      // 执行夹取物动作
    ARM_STATE_DETECT_BUCKET,       // 寻找/识别桶姿态
    ARM_STATE_PLACE_EXPLOSIVE,     // 执行放置动作
    ARM_STATE_HIT_TARGET,          // 打靶动作
    ARM_STATE_DETECT_HOSTAGE,      // 寻找/识别人质姿态
    ARM_STATE_GRAB_HOSTAGE         // 执行夹人质动作
} ArmState_t;

// ==================== 2. 底盘独立状态枚举 ====================
typedef enum {
    CHASSIS_STATE_IDLE = 0,
    CHASSIS_STATE_FORWARD,         // 快速直走
    CHASSIS_STATE_FORWARD_SLOW,    // 慢速直走
    CHASSIS_STATE_MOVE_LEFT,       // 左平移
    CHASSIS_STATE_MOVE_RIGHT,      // 右平移
    CHASSIS_STATE_SPIN_LEFT,       // 左自旋
    CHASSIS_STATE_SPIN_RIGHT       // 右自旋
} ChassisState_t;

// ==================== 3. 声明全局变量（使用 volatile 保证中断安全） ====================
extern volatile ArmState_t g_arm_state;
extern volatile ChassisState_t g_chassis_state;

// ==================== 4. 声明状态设置接口（解决隐式声明警告） ====================
void Set_Arm_State(ArmState_t new_state);
void Set_Chassis_State(ChassisState_t new_state);

#endif /* __GLOBAL_STATE_H */
