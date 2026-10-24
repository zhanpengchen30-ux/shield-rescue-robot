# Shield Rescue · 救援机器人控制器

> 基于 STM32F103 的救援机器人嵌入式控制器，包含全向底盘、传感器/执行器底层驱动与控制中间件。
> STM32F103 rescue-robot controller — mecanum chassis, sensor/actuator BSP drivers and control middleware.

## 硬件平台

- 主控：STM32F103xE（72 MHz，HSE + PLL）
- 实时系统：FreeRTOS（CMSIS-RTOS v2）
- 外设：TIM1–TIM5 / TIM8（PWM、编码器、计时）、USART1/2/3（调试、通信）
- 工具链：STM32CubeIDE / Keil MDK（含 `ShieldRescue/ShieldRescue.ioc`）

## 功能模块

| 目录 | 内容 | 说明 |
| --- | --- | --- |
| `ShieldRescue/Core/App` | `nav_task` 导航 · `action_task` · `robot_action` · `monitor` 监控 | 机器人高层任务骨架 |
| `ShieldRescue/Core/Drivers` | `bsp_motor` · `bsp_servo` · `bsp_laser` · `bsp_usart` | 电机 / 舵机 / 激光雷达 / 串口底层驱动 |
| `ShieldRescue/Core/Middlewares` | `odom` 里程计 · `pid` | 运动学解算与 PID 控制中间件 |
| `ShieldRescue/Core/Src` | HAL / FreeRTOS 生成代码 | CubeMX 自动生成 |

## 主要特性

- Mecanum 全向底盘控制（`Chassis_Control_O_Mecanum`）
- 电机 / 舵机 / 激光 / 串口四类底层 BSP 驱动
- 里程计（odom）+ PID 控制中间件，便于搭建闭环运动控制
- FreeRTOS 任务分层：`App`（导航 / 动作 / 监控）与 `Middlewares`（算法）

## 目录结构

```
ShieldRescue/                  # CubeMX 工程根目录
├─ Core/
│  ├─ App/                     # 导航 / 动作 / 监控任务
│  ├─ Drivers/                 # 电机 / 舵机 / 激光 / 串口 BSP
│  ├─ Middlewares/             # odom 里程计 / pid
│  ├─ Src/ Inc/                # HAL + FreeRTOS 生成代码
├─ Drivers/                    # STM32 HAL + CMSIS
├─ Middlewares/Third_Party/    # FreeRTOS 源码
└─ MDK-ARM/                    # Keil 工程
```

## 编译说明

1. 工程位于 `ShieldRescue/` 子目录，用 STM32CubeIDE 打开 `ShieldRescue/ShieldRescue.ioc` 或用 `ShieldRescue/MDK-ARM/` 下 Keil 工程编译。
2. `Drivers/CMSIS` 中的预编译 DSP 库（`*.lib` / `*.a`）按 `.gitignore` 不纳入版本管理，编译前请确保本地固件包包含对应 CMSIS-DSP。
3. 注意：仓库中 `main.c` 的主循环为 Mecanum 直线运动的调试版本，FreeRTOS 调度入口以注释形式保留；如需运行 `App/` 下的任务，请取消 `main.c` 中 `osKernelInitialize / MX_FREERTOS_Init / osKernelStart` 的注释并接入任务创建。

## 状态

底盘驱动、传感器/执行器 BSP 与控制中间件已落地；`App/` 任务层为搭好的骨架，可在此基础上扩展导航与动作逻辑。

---

© 代码归作者所有。HAL / FreeRTOS 等第三方组件遵循其各自许可证（详见对应源码头文件）。
