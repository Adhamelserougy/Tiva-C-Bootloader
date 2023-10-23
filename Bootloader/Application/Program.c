#include "Application/Program.h"

#define BANK_SIZE 64U//32U /*TODO: Edit to change bank size if needed */
#define BLOCK_SIZE 0x400U//0x3FCU /* Do not change this number */ /*0x400*/
#define BANK1 1U
#define BANK2 2U
#define BANK1_ADDRESS 0x00010000U
#define BANK2_ADDRESS 0x00020000U

volatile uint8_t Current_Bank = BANK1;
volatile uint32_t Current_Address = BANK1_ADDRESS;

void Assign_Bank(uint8_t Bank)
{
    Current_Bank = Bank;
    switch (Current_Bank)
    {
    case BANK1:
        Current_Address = BANK1_ADDRESS;
        break;
    case BANK2:
        Current_Address = BANK2_ADDRESS;
        break;
    default:
        break;
    }
}

void Flash_New_Program(uint32_t *Code, uint32_t Code_Size)
{
    FlashProgram((uint32_t *)Code, Current_Address, Code_Size);
}

void Erase_Sector(void)
{
    uint8_t i;
    for (i = 0; i < BANK_SIZE; i++)
    {
        FlashErase(Current_Address + i * BLOCK_SIZE);
    }
}

void Jump_To_New_Program(void)
{
    *((volatile uint32_t*) 0xE000ED08) = Current_Address;
//    __set_MSP(*(( uint32_t *)Current_Address));

    switch (Current_Bank) {
        case BANK1:
            __asm("        MOVS      r0,#0x10000");
            __asm("        LDR       r1,[r0, #0x4]");
            __asm("        BX        r1");
            break;
        case BANK2:
            __asm("        MOVS      r0,#0x20000");
            __asm("        LDR       r1,[r0, #0x4]");
            __asm("        BX        r1");
            break;
        default:
            break;
    }

}
