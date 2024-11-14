#include <stdlib.h>
#include <stdbool.h>

#include "util.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "i2c_primary.h"
#include "lcd.h"

#define LED_ERR_PIN PB3
#define LED_ACK_PIN PB1


void red_led_flash() {
  PORTB |= _BV(LED_ERR_PIN);
  _delay_ms(50);
  PORTB &= ~_BV(LED_ERR_PIN);
  _delay_ms(50);
}

void yellow_led_flash() {
  PORTB |= _BV(LED_ACK_PIN);
  _delay_ms(50);
  PORTB &= ~_BV(LED_ACK_PIN);
  _delay_ms(50);
}

void error_indicator() {
  while(1) { red_led_flash(); yellow_led_flash(); }
}

int main () {

  disable_interrupts;

  DDRB |= _BV(LED_ERR_PIN);
  DDRB |= _BV(LED_ACK_PIN);

  enable_interrupts;

  unsigned char ack;

  red_led_flash();
  yellow_led_flash();

  i2c_init();

  _delay_ms(100);

  while (1) {

    unsigned char addr = (0x27 << 1);
    i2c_start();
    ack = i2c_write_byte(addr);
    i2c_stop();

    _delay_ms(1);

  }

}
