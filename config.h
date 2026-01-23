#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"
#include "i2c.h"
#include "tim.h"
#include <stdint.h>

#define DEVICE_OLED     0
#if DEVICE_OLED 
    #define OLED_W_SCL(x)		HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, (x))
    #define OLED_W_SDA(x)		HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, (x))
#endif

#define DEVICE_MPU6050  0
#if DEVICE_MPU6050
    #define MPU6050_I2C         hi2c2
#endif

#define DEVICE_ENCODER  0
#if DEVICE_ENCODER
    #define Encoder_Left        htim3
    #define Encoder_Right       htim1
    #define ENCODER_WHEEL_DIAMETER         0.65f
    #define ENCODER_PULSE_PER_REV          1470.0f
    #define ENCODER_PI                     3.1415926f
#endif

#endif
