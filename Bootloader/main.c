#include "main.h"

#include "inc/hw_ints.h"
#include "inc/hw_can.h"

#include "HAL/EEPROM_Drivers/EEPROM.h"

#define MAX_Size 30700
#define EEPROM_RESET_VALUE 0xFFFFFFFF

uint8_t Code_hex[MAX_Size] = {0};
uint16_t Code_Size=0;

int main(void)
{
    tCANMsgObject sCANMessage;
    tCANMsgObject sCANMessage_Tx;
    uint32_t Program_Session_Flag=0;
    uint8_t data[4] = {1,2,3,4};
    EEPROM_init();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);


    /*********************************************************/
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0U);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0U);
    EEPROM_Read(&Program_Session_Flag, 0x400, 4); /*Read Program request flag from EEPROM*/

    if((Program_Session_Flag == 1) || (Program_Session_Flag == EEPROM_RESET_VALUE))
    {
        can_init();
        /*********** Message object to be recieved **************/
        sCANMessage.ui32MsgID = 1;
        sCANMessage.ui32MsgIDMask = 1;
        sCANMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE;
        sCANMessage.ui32MsgLen = 4;
        CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_RX);
        /*********************************************************/

        /*****************Message object to be sent****************************/
        sCANMessage_Tx.ui32MsgID = 2;
        sCANMessage_Tx.ui32MsgIDMask = 2;
        sCANMessage_Tx.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
        sCANMessage_Tx.ui32MsgLen = sizeof(data);
        sCANMessage_Tx.pui8MsgData = data;

        /***********************************************************************/
//        /*Send ACK*/

        while(can_send(&sCANMessage_Tx, 2) != CAN_OK);
        can_receiveFile(&sCANMessage,Code_hex,&Code_Size); /*Receive a program to flash*/
        Assign_Bank(1);                                    /*Assign To specific Bank*/
        Erase_Sector();                                    /*Erase specific Bank*/
        Flash_New_Program((uint32_t *)Code_hex, Code_Size);/*Flash the new code*/
        SysCtlPeripheralDisable(SYSCTL_PERIPH_CAN0);       /*De-init CAN*/
        Program_Session_Flag=0;
        EEPROM_Write(&Program_Session_Flag, 0x400, 4);     /*Clear Program request flag in EEPROM*/
    }

    /***********************************************************************/
    /*Send ACK*/

//    while(can_send(&sCANMessage_Tx, 2) != CAN_OK);



//    can_receiveFile(&sCANMessage,Code_hex,&Code_Size);
//    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0U);
//    SysCtlPeripheralDisable(SYSCTL_PERIPH_CAN0);
//    Assign_Bank(1);
//    Erase_Sector();
//    Flash_New_Program((uint32_t *)Code_hex, Code_Size);

    Jump_To_New_Program();

    /*
    Assign_Bank(Bank); TODO: Choose either bank 1 or bank 2 to flash by reading the flag in EEPROM
    Erase_Sector(); TODO: Call Erase_Sector to clear bank before flashing new program
    Flash_New_Program(Code, Code_Size); TODO: Flash new program by giving it a uint32_t pointer to the array that contains code and the number of elements in array
    Jump_To_New_Program(); TODO: Call this to jump to new program
    */
}
