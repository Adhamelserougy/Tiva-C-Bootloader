#include "HAL/EEPROM_Drivers/EEPROM.h"
uint32_t EEPROM_Init_state;

void EEPROM_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0)){}
    EEPROM_Init_state = EEPROMInit();
    if(EEPROM_Init_state != EEPROM_INIT_OK)
    {
    while(1)
    {
    }
    }
}

void EEPROM_Write(uint32_t *data,uint32_t address,uint8_t size)
{
    EEPROMProgram(data,address,size);
}

void EEPROM_Read(uint32_t *data,uint32_t address,uint8_t size)
{
    EEPROMRead(data, address, size);
}


