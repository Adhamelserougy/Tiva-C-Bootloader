/*
 * CAN.h
 *
 *  Created on: Oct 5, 2023
 *      Author: mtear
 */

#ifndef HAL_CAN_RX_CAN_RX_H_
#define HAL_CAN_RX_CAN_RX_H_

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


typedef enum
{
    CAN_NOData,CAN_OK,CAN_NOK
}CanStatus;

void can_init(void);
CanStatus can_receiveNonBlocking(tCANMsgObject *pMSG,uint8_t *data);
void can_receiveFile(tCANMsgObject *pMSG,uint8_t *file,uint16_t *size);
CanStatus can_send(tCANMsgObject *pMSG,uint8_t Msg_ObjId);
#endif /* HAL_CAN_RX_CAN_RX_H_ */
