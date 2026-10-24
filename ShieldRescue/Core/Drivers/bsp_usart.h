#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"

// 初始化接收中断
void BSP_UART_Init(void);
// 处理串口指令
void UART_Parse_Data(uint8_t command);
// 向副板发送控制指令的接口
void Send_Cmd_To_Sub_Board(uint8_t cmd); 

#endif /* __BSP_UART_H */

// bsp_usart.h / global_state.h
// 定义虚拟遥控器的“按键值”（功能码）

//// 1. 底盘运动控制按键
//#define KEY_CHASSIS_STOP        0x01  // 停车
//#define KEY_CHASSIS_FORWARD     0x02  // 前进
//#define KEY_CHASSIS_BACKWARD    0x03  // 后退
//#define KEY_CHASSIS_LEFT        0x04  // 左平移
//#define KEY_CHASSIS_RIGHT       0x05  // 右平移
//#define KEY_CHASSIS_SPIN_LEFT   0x06  // 左自旋
//#define KEY_CHASSIS_SPIN_RIGHT  0x07  // 右自旋
//#define KEY_CHASSIS_PID_ALIGN   0x08  // 视觉 PID 寻路（此时速度由 rx_dx 决定）

//// 2. 机械臂一键动作包按键
//#define KEY_ARM_POSE_SCAN_QR    0x10  // 机械臂：固定扫码姿态
//#define KEY_ARM_GRAB_BOMB       0x11  // 机械臂：一键夹取爆炸物
//#define KEY_ARM_PLACE_BOMB      0x12  // 机械臂：一键把爆炸物放到防爆桶
//#define KEY_ARM_SHOOT           0x13  // 机械臂：一键打靶姿态
//#define KEY_ARM_GRAB_HOSTAGE    0x14  // 机械臂：一键夹取人质姿态


