/*
 * Hex_Recieve.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Ahmed
 */

#ifndef HAL_HEX_RECIEVE_HEX_RECIEVE_H_
#define HAL_HEX_RECIEVE_HEX_RECIEVE_H_

void send_StringPC(uint8_t *str);
void uart_init();
bool IsDataAvailable();



#endif /* HAL_HEX_RECIEVE_HEX_RECIEVE_H_ */
