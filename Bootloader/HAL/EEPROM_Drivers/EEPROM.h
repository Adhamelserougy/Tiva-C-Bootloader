#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include"driverlib/eeprom.h"

// Function to initialize EEPROM
void EEPROM_init(void);

// Function to write data to EEPROM
void EEPROM_Write(uint32_t *data, uint32_t address, uint8_t size);

// Function to read data from EEPROM
void EEPROM_Read(uint32_t *data, uint32_t address, uint8_t size);

#endif /* EEPROM_H */
