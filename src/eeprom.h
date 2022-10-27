/*
 * eeprom.h
 *
 *  Created on: Dec 1, 2021
 *      Author: janoko
 */

#ifndef DRIVER_EEPROM_H_
#define DRIVER_EEPROM_H_

#include "stddef.h"
#include "stdint.h"

#ifndef EEPROM_DEV_ID
#define EEPROM_DEV_ID ((0x50 | (0)) << 1)
#endif

#ifndef EEPROM_PAGE_SIZE
#define EEPROM_PAGE_SIZE 32
#endif

#ifndef EEPROM_PAGE_NUM
#define EEPROM_PAGE_NUM 125
#endif

typedef enum
{
  EEPROM_OK       = 0x00,
  EEPROM_ERROR    = 0x01,
  EEPROM_BUSY     = 0x02,
  EEPROM_TIMEOUT  = 0x03
} EEPROM_Status_t;

typedef struct EEPROM_HandlerTypedef {
  struct {
    void *i2c;
    uint8_t id;
  } device;
  EEPROM_Status_t (*devCheck)(struct EEPROM_HandlerTypedef* i2cDev);
  EEPROM_Status_t (*devRead)(struct EEPROM_HandlerTypedef* i2cDev,
                             uint16_t memAddress,
                             uint8_t *dst, uint16_t size);
  EEPROM_Status_t (*devWrite)(struct EEPROM_HandlerTypedef* i2cDev,
                              uint16_t memAddress,
                              uint8_t *src, uint16_t size);
} EEPROM_HandlerTypedef;

EEPROM_Status_t EEPROM_Init(EEPROM_HandlerTypedef*, void *i2cDev, uint8_t devId);
EEPROM_Status_t EEPROM_Check(EEPROM_HandlerTypedef*);
EEPROM_Status_t EEPROM_Read(EEPROM_HandlerTypedef*, uint16_t addr, void *data, uint16_t length);
EEPROM_Status_t EEPROM_Write(EEPROM_HandlerTypedef*, uint16_t addr, void *data, uint16_t length);

#endif /* DRIVERS_EEPROM_SOURCE_EEPROM_H_ */
