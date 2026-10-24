#ifndef __BSP_LASER_H
#define __BSP_LASER_H
#include "main.h"

// ¼¤¹âÒý½Å
#define LASER_PORT  GPIOC
#define LASER_PIN   GPIO_PIN_13 

void Laser_On(void);
void Laser_Off(void);
#endif
