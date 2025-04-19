/*
 * LCD.h
 *
 *  Created on: Nov 29, 2024
 *      Author: cslim
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_



#endif /* SRC_LCD_H_ */

/*
 * LCD.cpp
 *
 *  Created on: Nov 29, 2024
 *      Author: cslim
 */

void updateUnit(float frequency, float velocity, ADC_HandleTypeDef* hadc2, UART_HandleTypeDef* huart2,UART_HandleTypeDef* huart3,int method);
void enableMenu();
void m(char* msg);
float msToKmh(float velocity);
float msToMph(float velocity);
void ADC_displayLCD(float frequency, float velocity, UART_HandleTypeDef* huart2, UART_HandleTypeDef* huart3);
void COMP_displayLCD(float frequency, float velocity, UART_HandleTypeDef* huart2, UART_HandleTypeDef* huart3);
void LCD_Init();
void chooseMethod(ADC_HandleTypeDef* hadc2, UART_HandleTypeDef* huart2,int* method);
