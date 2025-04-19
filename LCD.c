/*
 * LCD.cpp
 *
 *  Created on: Nov 29, 2024
 *      Author: cslim
 */
#include "main.h"
#include "menu.h"
#include "LCD.h"
#include "lcd16x2_v2.h"
#include "intToBcd.h"
#include <stdio.h>
#include <stdbool.h>

#define MS 0
#define KMH 1
#define MPH 2

static uint16_t unit = MS, menuOn = 0;
static uint8_t lcdMessage[16];
uint32_t lastUpdateTime = 0;
uint16_t adc_value;
uint16_t adc_value1;

int last_method=3;

int uart_buf_len2;
char uart_buf2[50];

void updateUnit(float frequency, float velocity, ADC_HandleTypeDef* hadc2, UART_HandleTypeDef* huart2, UART_HandleTypeDef* huart3, int method){
	if (menuOn == 1) {
		// Update every 100ms to check for unit changing
		if (HAL_GetTick() - lastUpdateTime >= 100) {
			lastUpdateTime = HAL_GetTick();

			// Start ADC conversion and check button inputs directly
			adc_value=0;
			HAL_ADC_Start(hadc2);

			if (HAL_ADC_PollForConversion(hadc2, 1) == HAL_OK) {
				adc_value = HAL_ADC_GetValue(hadc2);
			}
			HAL_ADC_Stop(hadc2);
			uart_buf_len2 = snprintf(uart_buf2, sizeof(uart_buf2), "adc_value=%d\n",adc_value);
			HAL_UART_Transmit(huart2, (uint8_t*)uart_buf2, uart_buf_len2, 100);

			// Check ADC value to determine button press and update 'unit'
			if (adc_value >0 && adc_value < 1000) {
				unit = MPH;  // UP button
			}
	  	 	else if (adc_value >= 1000 && adc_value < 2000) {
	  	 		unit = KMH;  // DOWN button logic
	  	 	}
	  	 	else if (adc_value == 0) { // RIGHT button
	  	 		unit = MS;
	  	 	}
			//Display the results based on the method used.
			if(method==0){
				ADC_displayLCD(frequency, velocity, huart2, huart3);
			}
			else{
				COMP_displayLCD(frequency, velocity,huart2, huart3);
			}
	 	}
		HAL_Delay(500);
	}
}

// Enable menu
void enableMenu(){
    menuOn = 1;
}

// Display message on LCD
void m(char* msg){
	lcd16x2_printf(msg);
}

// Convert m/s to km/h
float msToKmh(float velocity){
    return (velocity * 3.6);
}

// Convert m/s to mph
float msToMph(float velocity){
    return(velocity * 2.237);
}

//Function to display speed on LCD in selected units for ADC-based method.
void ADC_displayLCD(float frequency, float velocity, UART_HandleTypeDef* huart2, UART_HandleTypeDef* huart3){
	lcd16x2_clear();
	lcd16x2_1stLine();
	uart_buf_len2 = sprintf(uart_buf2,"\nA Frequency: %0.2f \n",frequency);
	HAL_UART_Transmit(huart2, (uint8_t *)uart_buf2, uart_buf_len2, 100);
    switch(unit){
        case MS:
            snprintf((char*)lcdMessage, sizeof(lcdMessage), "VA: %.2f M/S", velocity);
            lcd16x2_printf((char*)lcdMessage);
            bcdProcessing(velocity,huart2,huart3);

            uart_buf_len2 = sprintf(uart_buf2,"A Speed: %.2f m/s\r\n",velocity);
            HAL_UART_Transmit(huart2, (uint8_t *)uart_buf2, uart_buf_len2, 100);

            break;
        case KMH:
            snprintf((char*)lcdMessage, sizeof(lcdMessage), "VA: %.2f KM/H", msToKmh(velocity));
            lcd16x2_printf((char*)lcdMessage);
            bcdProcessing(msToKmh(velocity),huart2,huart3);

            uart_buf_len2 = sprintf(uart_buf2,"A Speed: %.2f m/s\r\n",msToKmh(velocity));
            HAL_UART_Transmit(huart2, (uint8_t *)uart_buf2, uart_buf_len2, 100);
            break;
        case MPH:
            snprintf((char*)lcdMessage, sizeof(lcdMessage), "VA: %.2f MPH", msToMph(velocity));
            lcd16x2_printf((char*)lcdMessage);
            bcdProcessing(msToMph(velocity),huart2,huart3);

            uart_buf_len2 = sprintf(uart_buf2,"A Speed: %.2f m/s\r\n",msToMph(velocity));
            HAL_UART_Transmit(huart2, (uint8_t *)uart_buf2, uart_buf_len2, 100);
            break;
    }
    lcd16x2_2ndLine();
    snprintf((char*)lcdMessage, sizeof(lcdMessage), "FA: %.2f Hz", frequency);
    lcd16x2_printf((char*)lcdMessage);

    HAL_Delay(500);  // Small delay for stability in LCD refresh
}

//Function to display speed on LCD in selected units for comparator-based method.
void COMP_displayLCD(float frequency, float velocity, UART_HandleTypeDef* huart2, UART_HandleTypeDef* huart3){
	lcd16x2_clear();
	lcd16x2_1stLine();
	uart_buf_len2 = sprintf(uart_buf2,"\nA Frequency: %0.2f \n",frequency);
	HAL_UART_Transmit(huart2, (uint8_t *)uart_buf2, uart_buf_len2, 100);
    switch(unit){
        case MS:
            snprintf((char*)lcdMessage, sizeof(lcdMessage), "VC: %.2f M/S", velocity);
            lcd16x2_printf((char*)lcdMessage);
            bcdProcessing(velocity,huart2,huart3);

            uart_buf_len2 = sprintf(uart_buf2,"A Speed: %.2f m/s\r\n",velocity);
            HAL_UART_Transmit(huart2, (uint8_t *)uart_buf2, uart_buf_len2, 100);
            break;
        case KMH:
            snprintf((char*)lcdMessage, sizeof(lcdMessage), "VC: %.2f KM/H", msToKmh(velocity));
            lcd16x2_printf((char*)lcdMessage);
            bcdProcessing(msToKmh(velocity),huart2,huart3);

            uart_buf_len2 = sprintf(uart_buf2,"A Speed: %.2f m/s\r\n",msToKmh(velocity));
            HAL_UART_Transmit(huart2, (uint8_t *)uart_buf2, uart_buf_len2, 100);

            break;
        case MPH:
            snprintf((char*)lcdMessage, sizeof(lcdMessage), "VC: %.2f MPH", msToMph(velocity));
            lcd16x2_printf((char*)lcdMessage);
            bcdProcessing(msToMph(velocity),huart2,huart3);

            uart_buf_len2 = sprintf(uart_buf2,"A Speed: %.2f m/s\r\n",msToMph(velocity));
            HAL_UART_Transmit(huart2, (uint8_t *)uart_buf2, uart_buf_len2, 100);
            break;
    }
    lcd16x2_2ndLine();
    snprintf((char*)lcdMessage, sizeof(lcdMessage), "FC: %.2f Hz", frequency);
    lcd16x2_printf((char*)lcdMessage);

    HAL_Delay(500);  // Small delay for stability in LCD refresh
}

//Function to choose the method used.
void chooseMethod(ADC_HandleTypeDef* hadc2, UART_HandleTypeDef* huart2,int* method){
	enableMenu();  // Sets menuOn = 1
	if (menuOn == 1) {
		lcd16x2_clear();
		lcd16x2_1stLine();
		snprintf((char*)lcdMessage, sizeof(lcdMessage), "Press.");
		lcd16x2_printf((char*)lcdMessage);


		uart_buf_len2 = snprintf(uart_buf2, sizeof(uart_buf2), "\nSelect\n\n");
		HAL_UART_Transmit(huart2, (uint8_t*)uart_buf2, uart_buf_len2, 100);

		adc_value1=0;

		HAL_ADC_Start(hadc2);

		if (HAL_ADC_PollForConversion(hadc2, 1) == HAL_OK) {
			adc_value1 = HAL_ADC_GetValue(hadc2);
		}
		HAL_ADC_Stop(hadc2);
		uart_buf_len2 = snprintf(uart_buf2, sizeof(uart_buf2), "adc_value=%d\n\n",adc_value1);
		HAL_UART_Transmit(huart2, (uint8_t*)uart_buf2, uart_buf_len2, 100);

		// Check ADC value to determine button press and update 'method'
		if ((adc_value1 >=2000)&&(adc_value1 < 4095)) {
			//Toggle between the method used.
			*method=(last_method==0)?1:0;
			uart_buf_len2 = snprintf(uart_buf2, sizeof(uart_buf2), "Method=%d\n\n",*method);
			HAL_UART_Transmit(huart2, (uint8_t*)uart_buf2, uart_buf_len2, 100);

			//Store the method used to compare with the next cycle method.
			last_method=*method;
			uart_buf_len2 = snprintf(uart_buf2, sizeof(uart_buf2), "\nLast_Method=%d\n\n",last_method);
			HAL_UART_Transmit(huart2, (uint8_t*)uart_buf2, uart_buf_len2, 100);
		}
		HAL_Delay(500);
	}
}
