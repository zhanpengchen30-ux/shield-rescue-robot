// nav_task.h
#ifndef __NAV_TASK_H
#define __NAV_TASK_H

#include "main.h"

/* ==================== 1. 外部可调用的公共函数声明 ==================== */

/**
 * @brief  导航控制任务入口（由 FreeRTOS 调用）
 */
void StartNavTask(void *argument);

/**
 * @brief  如果以后需要由其他任务（如 monitor.c）获取导航模块的数据，
 *         可以在这里声明相关的 Getter/Setter 函数。
 */
// float Nav_GetChassisSpeed(void); 

#endif /* __NAV_TASK_H */
