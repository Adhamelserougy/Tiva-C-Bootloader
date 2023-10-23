
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "HAL/CAN_TX/CAN_TX.h"

#include "HAL/Hex_Recieve/Hex_Recieve.h"

#define Max_Length 30700

volatile uint8_t Recived_Byte=0;
volatile uint8_t PC_Buffer_FULL=0;
volatile uint8_t RecievedBufferPC[Max_Length] ={0};
volatile uint16_t PC_Index=0;
volatile uint8_t State=0;
volatile uint8_t Send_Flag=0;


void get_FileSize(uint8_t *size,uint16_t *File_Length)
{

    uint8_t counter_Size=0;
    uint8_t Temp=0;
    uint8_t Temp2=0;

    uint16_t File_Size = 0;
    /*Read Size*/
    while(counter_Size<1)
    {
        if(IsDataAvailable())
        {
            Temp = UARTCharGetNonBlocking(UART0_BASE);
            counter_Size++;
        }
    }

    while(counter_Size<2)
    {
        if(IsDataAvailable())
        {
            Temp = UARTCharGetNonBlocking(UART0_BASE);
            size[0] = Temp;
            counter_Size++;
        }
    }

    while(counter_Size<3)
    {
        if(IsDataAvailable())
        {
            Temp2 = UARTCharGetNonBlocking(UART0_BASE);
            size[1] = Temp2;
            counter_Size++;
        }
    }
    File_Size = Temp<<8 | Temp2;
    *File_Length = File_Size;
}

int main(void)
{
    can_init();
    uart_init();

    tCANMsgObject sCANMessage;
    tCANMsgObject sCANMessage_ProgramReq;
    tCANMsgObject sCANMessage_Rx;
    uint8_t ProgramReq[4]={1,0,0,3};
    uint8_t data[4]={0};            //buffer to receive ACK from BootLoader

    uint16_t File_SizeOf4 =0;
    uint8_t pui8MsgData[4]={0};
    uint16_t My_HEX_Counter=0;
    uint8_t j=0;
    uint16_t file_Size=0;
    uint8_t Rec_Size_Flag=0;
    CANStatus cur_status;

    /*********** Message object to be sent *******************/
    sCANMessage.ui32MsgID = 1;
    sCANMessage.ui32MsgIDMask = 1;
    sCANMessage.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    sCANMessage.ui32MsgLen = sizeof(pui8MsgData);
    sCANMessage.pui8MsgData = pui8MsgData;
    /*********************************************************/

    /*********** Message object to be recieved **************/
    sCANMessage_Rx.ui32MsgID = 2;
    sCANMessage_Rx.ui32MsgIDMask = 2;
    sCANMessage_Rx.ui32Flags = MSG_OBJ_RX_INT_ENABLE;
    sCANMessage_Rx.ui32MsgLen = 4;

    CANMessageSet(CAN0_BASE, 2, &sCANMessage, MSG_OBJ_TYPE_RX);
    /*********************************************************/

    /*********** Message object to be sent *******************/
    sCANMessage_ProgramReq.ui32MsgID = 3;
    sCANMessage_ProgramReq.ui32MsgIDMask = 3;
    sCANMessage_ProgramReq.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    sCANMessage_ProgramReq.ui32MsgLen = sizeof(pui8MsgData);
    sCANMessage_ProgramReq.pui8MsgData = ProgramReq;
    /*********************************************************/

    while(1)
    {

        /*****************Send File Size***************/

        if(Rec_Size_Flag == 0)
        {
            get_FileSize(pui8MsgData,&file_Size);
            Rec_Size_Flag=1;
        }

        if(IsDataAvailable())
        {
            RecievedBufferPC[PC_Index]=UARTCharGetNonBlocking(UART0_BASE);
            PC_Index++;

        }
        else if(PC_Index == file_Size)
        {

            while(can_send(&sCANMessage_ProgramReq,3) != CAN_OK);   /*Send Flash Request*/

            while(can_receiveNonBlocking(&sCANMessage_Rx,2, data) != CAN_OK); /*Recieve Ack from BootLoader*/

            while(can_send(&sCANMessage,1) != CAN_OK);             /*Send Data to BootLoader*/


            My_HEX_Counter=0;
            for(j=0;j<4;j++,My_HEX_Counter++)
            {
                pui8MsgData[j] = RecievedBufferPC[My_HEX_Counter];
            }

            File_SizeOf4 = PC_Index;

            while(My_HEX_Counter<=File_SizeOf4)
            {
                cur_status = can_send(&sCANMessage,1);
                if(cur_status == CAN_NOK)
                {
                }
                else
                {
                    for(j=0;j<4;j++,My_HEX_Counter++)
                    {
                        pui8MsgData[j] = RecievedBufferPC[My_HEX_Counter];
                    }
                }
            }
            PC_Index=0;
            Rec_Size_Flag=0;
        }
    }
    return(0);
}
