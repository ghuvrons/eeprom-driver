/*
 * eeprom.c
 *
 *  Created on: Dec 1, 2021
 *      Author: janoko
 */

#include "eeprom.h"


EEPROM_Status_t EEPROM_Init(EEPROM_HandlerTypedef *eeprom, void *i2cDev, uint8_t devId)
{
  eeprom->device.i2c = i2cDev;
  eeprom->device.id = devId;

  if (eeprom->device.devCheck == NULL) return EEPROM_ERROR;
  if (eeprom->device.devRead == NULL) return EEPROM_ERROR;
  if (eeprom->device.devWrite == NULL) return EEPROM_ERROR;

  return EEPROM_OK;
}


EEPROM_Status_t EEPROM_Check(EEPROM_HandlerTypedef *eeprom)
{
  return (eeprom->devCheck != NULL)? eeprom->devCheck(eeprom): EEPROM_ERROR;
}


EEPROM_Status_t EEPROM_Read(EEPROM_HandlerTypedef *eeprom, uint16_t addr, void *data, uint16_t length)
{
  EEPROM_Status_t status;
  uint16_t partSz, pageAddr;
  uint16_t offset = addr % EEPROM_PAGE_SIZE;
  uint16_t pageIdx = addr / EEPROM_PAGE_SIZE;
  uint8_t *dst = (uint8_t*) data;

  if (eeprom->device.i2c == NULL || eeprom->devRead == NULL) return EEPROM_ERROR;

  // loop for every page
  while (length && pageIdx < EEPROM_PAGE_NUM) {
    partSz = ((length + offset) > EEPROM_PAGE_SIZE)? EEPROM_PAGE_SIZE: (length+offset);
    partSz -= offset;
    pageAddr = pageIdx*EEPROM_PAGE_SIZE + offset;

    status = eeprom->devRead(eeprom, pageAddr, dst, partSz);
    if (status != EEPROM_OK) return status;

    pageIdx++;
    offset = 0;
    length -= partSz;
    dst += partSz;
  }

  return status;
}


EEPROM_Status_t EEPROM_Write(EEPROM_HandlerTypedef *eeprom, uint16_t addr, void *data, uint16_t length)
{
  EEPROM_Status_t status;
  uint16_t partSz, pageAddr;
  uint16_t offset = addr % EEPROM_PAGE_SIZE;
  uint16_t pageIdx = addr / EEPROM_PAGE_SIZE;
  uint8_t *src = (uint8_t*) data;

  if (eeprom->device.i2c == NULL || eeprom->devWrite == NULL) return EEPROM_ERROR;

  // loop for every page
  while (length && pageIdx < EEPROM_PAGE_NUM) {
    partSz = ((length + offset) > EEPROM_PAGE_SIZE)? EEPROM_PAGE_SIZE: (length+offset);
    partSz -= offset;
    pageAddr = pageIdx*EEPROM_PAGE_SIZE + offset;

    status = eeprom->devWrite(eeprom, pageAddr, src, partSz);
    if (status != EEPROM_OK) return status;

    pageIdx++;
    offset = 0;
    length -= partSz;
    src += partSz;
  }

  return status;
}
