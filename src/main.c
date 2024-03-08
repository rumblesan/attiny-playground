#include <stdlib.h>
#include <stdbool.h>

#include "util.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "i2c_primary.h"
#include "mpr121.h"

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

  uint8_t read_byte;

  red_led_flash();
  yellow_led_flash();

  i2c_init();

  if (mpr121_setup()) {
    error_indicator();
  }

  while (1) {

    read_byte = 0x00;

    /*if (mpr121_read_registry_byte(MPR121_TOUCHSTATUS_L, &read_byte, 1)) {*/
      /*red_led_flash();*/
    /*}*/
    if (mpr121_read_registry_byte(MPR121_CONFIG2, &read_byte, 1)) {
      red_led_flash();
    }

    if (read_byte > 0) {
      yellow_led_flash();
    }
    _delay_ms(10);

  }

}
