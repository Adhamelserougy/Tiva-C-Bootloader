/*
 * Hex_Recieve.c
 *
 *  Created on: Oct 9, 2023
 *      Author: Ahmed
 */
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

#include "Hex_Recieve.h"

void send_StringPC(uint8_t *str)
{
    uint16_t i=0;
    for(i=0;str[i];i++)
    {
        UARTCharPut(UART0_BASE, str[i]);
    }
}

void uart_init()
{
    /*****************************************************UART PC********************************************************/
    // Enable the UART5 module.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Wait for the UART5 module to be ready.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}
    // Initialize the UART

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    /*ENABLE PORTE */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    /********************************************************************************************************************/
}

bool IsDataAvailable()
{
    bool state;
    state= UARTCharsAvail(UART0_BASE);

    return state;
}

