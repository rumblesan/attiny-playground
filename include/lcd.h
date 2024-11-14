#pragma once

#include <stdbool.h>

#define LCD_ADDRESS 0x27

#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

bool lcd_setup();
