
#include"CAN_TX.h"


volatile bool g_bErrFlag = 0;

volatile uint8_t sent_Flag =0;

volatile uint8_t g_bRXFlag=0;

void CANIntHandler(void)
{
    uint32_t ui32Status; /*Variable To Carry Status*/

    ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE); /*Read Interrupt Source*/

    if(ui32Status == CAN_INT_INTID_STATUS)                   /*If Error*/
    {
        ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        g_bErrFlag = 1;/*Set Error Flag*/
    }
    else if(ui32Status == 1) /*If object 1*/
    {
        CANIntClear(CAN0_BASE, 1); /*Clear interrupt flag*/

        g_bErrFlag = 0;            /*Clear Error Flag*/
        sent_Flag = 1;             /*Set Sending Flag*/
    }
    else if(ui32Status == 2)
    {

        CANIntClear(CAN0_BASE, 2); /*Clear interrupt flag*/

        g_bErrFlag = 0;            /*Clear Error Flag*/
        g_bRXFlag = 1;             /*Set Recieving flag*/
    }
    else if(ui32Status == 3)
    {
        CANIntClear(CAN0_BASE, 3);

        g_bErrFlag = 0;            /*Clear Error Flag*/
        sent_Flag = 1;             /*Set Sending Flag*/
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

CANStatus can_send(tCANMsgObject *pMSG,uint8_t MsgObj)
{
    CANStatus Loc_Status = CAN_OK;

    sent_Flag=0;
    CANMessageSet(CAN0_BASE, MsgObj, pMSG, MSG_OBJ_TYPE_TX);
    while(sent_Flag==0);
    if(g_bErrFlag)
    {
        Loc_Status = CAN_NOK;
    }
    else
    {
        /*do nothing */
    }

    return Loc_Status;
}
CANStatus can_receiveNonBlocking(tCANMsgObject *pMSG,uint8_t MsgObj,uint8_t *data)
{
    CANStatus LocStatus=CAN_NOK;
    if(g_bRXFlag)
    {
        pMSG->pui8MsgData = data;

        CANMessageGet(CAN0_BASE, MsgObj, pMSG, 0);

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
