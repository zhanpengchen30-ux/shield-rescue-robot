#ifndef __BSP_SERVO_H
#define __BSP_SERVO_H
#include "main.h"

void Servo_Init(void);
// angle: 0~180¶È
void Servo_SetAngle(uint8_t channel, float angle);
#endif
