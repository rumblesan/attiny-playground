#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MPR121_ADDRESS 0x5A

// mpr121 register addresses
#define MPR121_TOUCHSTATUS_L 0x00
#define MPR121_TOUCHSTATUS_H 0x01
#define MPR121_FILTDATA_0L 0x04
#define MPR121_FILTDATA_0H 0x05
#define MPR121_BASELINE_0 0x1E

#define MPR121_MHDR 0x2B
#define MPR121_NHDR 0x2C
#define MPR121_NCLR 0x2D
#define MPR121_FDLR 0x2E
#define MPR121_MHDF 0x2F
#define MPR121_NHDF 0x30
#define MPR121_NCLF 0x31
#define MPR121_FDLF 0x32
#define MPR121_NHDT 0x33
#define MPR121_NCLT 0x34
#define MPR121_FDLT 0x35

#define MPR121_TOUCHTH_0 0x41
#define MPR121_RELEASETH_0 0x42

#define MPR121_DEBOUNCE 0x5B

#define MPR121_CONFIG1 0x5C
#define MPR121_CONFIG2 0x5D

#define MPR121_ECR 0x5E

#define MPR121_SOFTRESET 0x80

bool mpr121_setup();

bool mpr121_read_registry_byte(uint8_t reg_addr, uint8_t *buffer, uint8_t length);
