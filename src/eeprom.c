/*
 * eeprom.c
 *
 *  Created on: Dec 1, 2021
 *      Author: janoko
 */

#include "eeprom.h"


HAL_StatusTypeDef EEPROM_Init(EEPROM_HandlerTypedef *eeprom, I2C_HandleTypeDef *hi2c)
{
  eeprom->hi2c = hi2c;
  return HAL_OK;
}


HAL_StatusTypeDef EEPROM_Check(EEPROM_HandlerTypedef *eeprom)
{
  HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(eeprom->hi2c, EEPROM_DEV_ID, 10, 100);
  return status;
}


HAL_StatusTypeDef EEPROM_Read(EEPROM_HandlerTypedef *eeprom, uint16_t addr, void *data, uint16_t length)
{
  HAL_StatusTypeDef status;
  uint16_t partSz, pageAddr;
  uint16_t offset = addr % EEPROM_PAGE_SIZE;
  uint16_t pageIdx = addr / EEPROM_PAGE_SIZE;
  uint8_t *dst = (uint8_t*) data;

  // loop for every page
  while (length && pageIdx < EEPROM_PAGE_NUM) {
    partSz = ((length + offset) > EEPROM_PAGE_SIZE)? EEPROM_PAGE_SIZE: (length+offset);
    partSz -= offset;
    pageAddr = pageIdx*EEPROM_PAGE_SIZE + offset;

    status = HAL_I2C_Mem_Read(eeprom->hi2c, EEPROM_DEV_ID, pageAddr, I2C_MEMADD_SIZE_16BIT, dst, partSz, 100);
    if (status != HAL_OK) return status;

    pageIdx++;
    offset = 0;
    length -= partSz;
    dst += partSz;
  }

  return status;
}


HAL_StatusTypeDef EEPROM_Write(EEPROM_HandlerTypedef *eeprom, uint16_t addr, void *data, uint16_t length)
{
  HAL_StatusTypeDef status;
  uint16_t partSz, pageAddr;
  uint16_t offset = addr % EEPROM_PAGE_SIZE;
  uint16_t pageIdx = addr / EEPROM_PAGE_SIZE;
  uint8_t *src = (uint8_t*) data;

  // loop for every page
  while (length && pageIdx < EEPROM_PAGE_NUM) {
    partSz = ((length + offset) > EEPROM_PAGE_SIZE)? EEPROM_PAGE_SIZE: (length+offset);
    partSz -= offset;
    pageAddr = pageIdx*EEPROM_PAGE_SIZE + offset;

    status = HAL_I2C_Mem_Write(eeprom->hi2c, EEPROM_DEV_ID, pageAddr, I2C_MEMADD_SIZE_16BIT, src, partSz, 100);
    if (status != HAL_OK) return status;

    pageIdx++;
    offset = 0;
    length -= partSz;
    src += partSz;
  }

  return status;
}
