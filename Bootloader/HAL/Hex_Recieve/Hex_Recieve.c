

#include"Hex_Recieve.h"




volatile bool g_bRXFlag = 0;

volatile bool g_bErrFlag = 0;

volatile uint8_t sent_Flag =0;


void CANIntHandler(void)
{
    uint32_t ui32Status;
    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    if(ui32Status == CAN_INT_INTID_STATUS)
    {
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
        g_bErrFlag = 1;
    }
    else if(ui32Status == 1)
    {
        CANIntClear(CAN0_BASE, 1);

        g_bRXFlag = 1;
        g_bErrFlag = 0;
    }
    else if(ui32Status == 2)
    {
        CANIntClear(CAN0_BASE, 2);

        g_bErrFlag = 0;
        sent_Flag = 1;

    }
    else
    {
        /*Do Nothing*/
    }
}

void can_init(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    CANInit(CAN0_BASE);

    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
    CANIntRegister(CAN0_BASE, CANIntHandler); // if using dynamic vectors

    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
    IntEnable(INT_CAN0);
    CANEnable(CAN0_BASE);
}




CanStatus can_receiveNonBlocking(tCANMsgObject *pMSG,uint8_t *data)
{
    CanStatus LocStatus=CAN_NOData;

    if(g_bRXFlag)
    {
        pMSG->pui8MsgData = data;

        CANMessageGet(CAN0_BASE, 1, pMSG, 0);

        g_bRXFlag = 0;
        if(pMSG->ui32Flags & MSG_OBJ_DATA_LOST)
        {
            LocStatus=CAN_NOK;
        }
        else
        {
            LocStatus=CAN_OK;
        }
    }
    return LocStatus;
}

void can_receiveFile(tCANMsgObject *pMSG,uint8_t *file,uint16_t *size)
{
    uint8_t uIdx=0;
    CanStatus cur_status;
    uint8_t Flag_Size =1;
    uint16_t Loc_size=0;
    uint16_t Received_Packets=0;
    uint16_t counter=0;
    uint8_t data[4]={0};

    for(;;) /*SysTick Timer*/
    {
        cur_status = can_receiveNonBlocking(pMSG, data);
        if(cur_status == CAN_OK)
        {
            if(Flag_Size == 1)
            {
                Flag_Size=0;

                *size = data[0]<<8 | data[1];
                Loc_size = *size/4;
            }
            else
            {
                for(uIdx = 0; uIdx < pMSG->ui32MsgLen; uIdx++,counter++)
                {
                    file[counter] = data[uIdx];
                }
                Received_Packets++;
            }
            if(Received_Packets == Loc_size)
            {
                Received_Packets = Loc_size;
                break;
            }
        }
    }
}
CanStatus can_send(tCANMsgObject *pMSG,uint8_t MsgObj)
{
    CanStatus Loc_Status = CAN_OK;
    sent_Flag=0;
    CANMessageSet(CAN0_BASE, MsgObj, pMSG, MSG_OBJ_TYPE_TX);
    while(sent_Flag==0);
    if(g_bErrFlag)
    {
        Loc_Status = CAN_NOK;
    }
    else
    {
    }
    return Loc_Status;
}


