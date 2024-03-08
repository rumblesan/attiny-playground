#include <stdint.h>
#include <stdbool.h>

#include "util.h"

#include <util/delay.h>

#include "mpr121.h"

#include "i2c_primary.h"

bool mpr121_read_registry_byte_raw(uint8_t reg_addr, uint8_t *buffer, uint8_t length) {
    if (i2c_write_byte(MPR121_ADDRESS << 1)) return true;
    if (i2c_write_byte(reg_addr)) return true;
    i2c_start();
    if (i2c_write_byte((MPR121_ADDRESS << 1) | 0x01)) return true;

    for (int i = 0; i < length; i += 1) {
      uint8_t read_another = (i == length - 1) ? 0x00 : 0x01;
      buffer[i] = i2c_read_byte(read_another);
    }
    return false;
}

bool mpr121_write_registry_byte_raw(uint8_t reg_addr, uint8_t value) {
    if (i2c_write_byte(MPR121_ADDRESS << 1)) return true;
    if (i2c_write_byte(reg_addr)) return true;
    if (i2c_write_byte(value)) return true;
    return false;
}

bool mpr121_read_registry_byte(uint8_t reg_addr, uint8_t *buffer, uint8_t length) {
  i2c_start();
  if (mpr121_read_registry_byte_raw(reg_addr, buffer, length)) return true;
  i2c_stop();
  return false;
}

bool mpr121_write_registry_byte(uint8_t reg_addr, uint8_t value) {
  bool stop_required = true;
  if (reg_addr == MPR121_ECR || reg_addr <= 0x7A) {
    stop_required = false;
  }

  if (stop_required) {
    i2c_start();
    if (mpr121_write_registry_byte_raw(MPR121_ECR, 0x00)) return true;
    i2c_stop();
  }
  i2c_start();
  if (mpr121_write_registry_byte_raw(reg_addr, value)) return true;
  i2c_stop();
  if (stop_required) {
    i2c_start();
    if (mpr121_write_registry_byte_raw(MPR121_ECR, 0x8F)) return true;
    i2c_stop();
  }
  return false;
}

bool mpr121_setup() {
  unsigned char read_byte = 0x00;

  // write to the reset register
  if (mpr121_write_registry_byte(MPR121_SOFTRESET, 0x63)) return true;
  _delay_ms(1);

  // Set electrode configuration to default values
  if (mpr121_write_registry_byte_raw(MPR121_ECR, 0x00)) return true;

  if (mpr121_read_registry_byte(MPR121_CONFIG2, &read_byte, 1)) return true;
  if (read_byte != 0x24) {
    return true;
  }

  // Default touch and release thresholds
  for (int i = 0; i < 12; i += 1) {
    if (mpr121_write_registry_byte_raw(MPR121_TOUCHTH_0 + (2 * i), 12)) return true;
    if (mpr121_write_registry_byte_raw(MPR121_RELEASETH_0 + (2 * i), 6)) return true;
  }

  // Configure baseline filtering control registers
  if (mpr121_write_registry_byte_raw(MPR121_MHDR, 0x01)) return true;
  if (mpr121_write_registry_byte_raw(MPR121_NHDR, 0x01)) return true;
  if (mpr121_write_registry_byte_raw(MPR121_NCLR, 0x0E)) return true;
  if (mpr121_write_registry_byte_raw(MPR121_FDLR, 0x00)) return true;

  if (mpr121_write_registry_byte_raw(MPR121_MHDF, 0x01)) return true;
  if (mpr121_write_registry_byte_raw(MPR121_NHDF, 0x05)) return true;
  if (mpr121_write_registry_byte_raw(MPR121_NCLF, 0x01)) return true;
  if (mpr121_write_registry_byte_raw(MPR121_FDLF, 0x00)) return true;

  if (mpr121_write_registry_byte_raw(MPR121_NHDT, 0x00)) return true;
  if (mpr121_write_registry_byte_raw(MPR121_NCLT, 0x00)) return true;
  if (mpr121_write_registry_byte_raw(MPR121_FDLT, 0x00)) return true;

  // Set other configuration registers
  if (mpr121_write_registry_byte_raw(MPR121_DEBOUNCE, 0x00)) return true;
  // default 16uA charge current
  if (mpr121_write_registry_byte_raw(MPR121_CONFIG1, 0x10)) return true;
  // 0.5uS encoding, 1ms period
  if (mpr121_write_registry_byte_raw(MPR121_CONFIG2, 0x20)) return true;

  // Enable all electrodes
  if (mpr121_write_registry_byte_raw(MPR121_ECR, 0x8F)) return true;

  return false;
}

