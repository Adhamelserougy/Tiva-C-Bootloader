
#ifndef HAL_CAN_TX_CAN_TX_H_
#define HAL_CAN_TX_CAN_TX_H_

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

#define FLASHING_REQUEST 1003

typedef enum
{
    CAN_NOK,
    CAN_OK
}CANStatus;

void can_init(void);
CANStatus can_send(tCANMsgObject *pMSG,uint8_t MsgObj);
CANStatus can_receiveNonBlocking(tCANMsgObject *pMSG,uint8_t MsgObj,uint8_t *data);

#endif /* HAL_CAN_TX_CAN_TX_H_ */
