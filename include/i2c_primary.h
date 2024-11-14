#pragma once

#include <stdint.h>
#include <stdbool.h>

#define I2C_SDA_PIN PB0
#define I2C_SCL_PIN PB2

#define WAIT_LOW 5 // 4,7us
#define WAIT_HIGH 4 // 4,0us

void i2c_init();

void i2c_start();

void i2c_stop();

bool i2c_write_byte(unsigned char data);

unsigned char i2c_read_byte(unsigned char nack);
