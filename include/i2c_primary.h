#pragma once

#include <stdint.h>
#include <stdbool.h>

#define I2C_SDA_PORT PORTB
#define I2C_SCL_PORT PORTB

#define I2C_SDA_PINR PINB
#define I2C_SCL_PINR PINB

#define I2C_SDA_DDR DDRB
#define I2C_SCL_DDR DDRB

#define I2C_SDA_PIN PB0
#define I2C_SCL_PIN PB2

#define WAIT_LONG 5 // 4,7us
#define WAIT_SHORT 4 // 4,0us

// USISR mask
#define USISR_CLOCK_8_BITS 0b11110000
#define USISR_CLOCK_1_BIT  0b11111110

void i2c_init();

void i2c_start();

void i2c_stop();

bool i2c_write_byte(uint8_t data);

unsigned char i2c_read_byte(unsigned char nack);
