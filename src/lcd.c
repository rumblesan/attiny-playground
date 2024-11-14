#include <stdint.h>
#include <stdbool.h>

#include "util.h"

#include <util/delay.h>

#include "lcd.h"

#include "i2c_primary.h"

bool lcd_setup() {
  i2c_start();

  // 4-bit mode initialization
  _delay_us(50000);
  if (i2c_write_byte(LCD_ADDRESS << 1)) return true;
  if (i2c_write_byte(0b00110100)) return true;
  if (i2c_write_byte(0b00110000)) return true;
  _delay_us(4100);
  if (i2c_write_byte(0b00110100)) return true;
  if (i2c_write_byte(0b00110000)) return true;
  _delay_us(100);
  if (i2c_write_byte(0b00110100)) return true;
  if (i2c_write_byte(0b00110000)) return true;
  _delay_us(4100);
  if (i2c_write_byte(0b00100100)) return true;
  if (i2c_write_byte(0b00100000)) return true;

  // function set
  _delay_us(40);
  if (i2c_write_byte(0b00100100)) return true;
  if (i2c_write_byte(0b00100000)) return true;
  if (i2c_write_byte(0b10000100)) return true;
  if (i2c_write_byte(0b10000000)) return true;

  // turn display off
  _delay_us(40);
  if (i2c_write_byte(0b00000100)) return true;
  if (i2c_write_byte(0b00000000)) return true;
  if (i2c_write_byte(0b10000100)) return true;
  if (i2c_write_byte(0b10000000)) return true;

  // display clear cursor home
  _delay_us(40);
  if (i2c_write_byte(0b00000100)) return true;
  if (i2c_write_byte(0b00000000)) return true;
  if (i2c_write_byte(0b00010100)) return true;
  if (i2c_write_byte(0b00010000)) return true;

  // cursor direction
  _delay_us(40);
  if (i2c_write_byte(0b00000100)) return true;
  if (i2c_write_byte(0b00000000)) return true;
  if (i2c_write_byte(0b01100100)) return true;
  if (i2c_write_byte(0b01100000)) return true;

  // turn on the display
  _delay_us(40);
  if (i2c_write_byte(0b00000100)) return true;
  if (i2c_write_byte(0b00000000)) return true;
  if (i2c_write_byte(0b11100100)) return true;
  if (i2c_write_byte(0b11100000)) return true;

  i2c_stop();
	return false;
}


bool lcd_write_consecutive_bytes(uint8_t reg_addr, uint8_t *values, uint8_t count) {
    if (i2c_write_byte(LCD_ADDRESS << 1)) return true;
    if (i2c_write_byte(reg_addr)) return true;
    for (int i = 0; i < count; i += 1) {
        if (i2c_write_byte(values[i])) return true;
    }
    return false;
}
