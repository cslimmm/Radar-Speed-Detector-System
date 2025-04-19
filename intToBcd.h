/*
 * intToBcd.h
 *
 *  Created on: Feb 18, 2025
 *      Author: cslim
 */

#ifndef SRC_INTTOBCD_H_
#define SRC_INTTOBCD_H_



#endif /* SRC_INTTOBCD_H_ */


void RS485_Transmit(uint8_t *data, uint16_t size,UART_HandleTypeDef* huart3);
uint8_t int_to_bcd(uint16_t speed, uint8_t bcd);
void bcdProcessing(uint16_t speed,UART_HandleTypeDef* huart2,UART_HandleTypeDef* huart3);
