#ifndef APPLICATION_PROGRAM_H_
#define APPLICATION_PROGRAM_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/flash.h"
#include "inc/hw_flash.h"

void Assign_Bank(uint8_t Bank);
void Flash_New_Program(uint32_t *Code, uint32_t Code_Size);
void Erase_Sector(void);
void Jump_To_New_Program(void);

#endif /* APPLICATION_PROGRAM_H_ */
