/*
 * eeprom.h
 *
 *  Created on: Dec 1, 2021
 *      Author: janoko
 */

#ifndef DRIVER_EEPROM_H_
#define DRIVER_EEPROM_H_

#include "stm32f4xx_hal.h"

#ifndef EEPROM_DEV_ID
#define EEPROM_DEV_ID ((0x50 | (0)) << 1)
#endif

#ifndef EEPROM_PAGE_SIZE
#define EEPROM_PAGE_SIZE 32
#endif

#ifndef EEPROM_PAGE_NUM
#define EEPROM_PAGE_NUM 125
#endif

#ifndef EEPROM_Delay
#define EEPROM_Delay(ms) HAL_Delay(ms)
#endif

typedef struct {
  I2C_HandleTypeDef *hi2c;
} EEPROM_HandlerTypedef;

HAL_StatusTypeDef EEPROM_Init(EEPROM_HandlerTypedef*, I2C_HandleTypeDef*);
HAL_StatusTypeDef EEPROM_Check(EEPROM_HandlerTypedef*);
HAL_StatusTypeDef EEPROM_Read(EEPROM_HandlerTypedef*, uint16_t addr, void *data, uint16_t length);
HAL_StatusTypeDef EEPROM_Write(EEPROM_HandlerTypedef*, uint16_t addr, void *data, uint16_t length);

#endif /* DRIVERS_EEPROM_SOURCE_EEPROM_H_ */
