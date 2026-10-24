#include "bsp_laser.h"

void Laser_On(void) {
    HAL_GPIO_WritePin(LASER_PORT, LASER_PIN, GPIO_PIN_SET);
}

void Laser_Off(void) {
    HAL_GPIO_WritePin(LASER_PORT, LASER_PIN, GPIO_PIN_RESET);
}
