/*
 * COMP.cpp
 *
 *  Created on: Nov 29, 2024
 *      Author: cslim
 */
#include "main.h"
#include <stdio.h>
#include "LCD.h"
#include "COMP.h"
#include "intToBcd.h"

#define WAVE_VELOCITY 300000000 // Speed of electromagnetic wave

float frequency_COMP = 0.0;
float velocity_COMP = 0.0;

int uart_buf_len1;
char uart_buf1[50];

float fwave1=10525000000.0;
int ticks=0;

//total time passed when calculating period and frequency, from 0 to 65535 is the fix time (which is 65535us)
float total_period = 0.065535;
//period of the input signal
float period;

//Function to compute the total number of rising edge.
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp){
	ticks++;
}

//Function to compute the period, frequency and speed of the input signal.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
	if(htim->Instance == TIM16){
		if(ticks){	//when there is cycle recorded
			period = total_period/((double)ticks);	//divide total time with number of cycle passed
			frequency_COMP = 1.0/period;	//frequency of input
			velocity_COMP= 0.5 * WAVE_VELOCITY * frequency_COMP / fwave1;
			ticks=0;		//reset the  number of half a cycle is recorded to 0
		}
	}
}

//Function to start the comparator-based system.
void COMP(COMP_HandleTypeDef* hcomp1, UART_HandleTypeDef* huart2, TIM_HandleTypeDef* htim16, ADC_HandleTypeDef* hadc2,UART_HandleTypeDef* huart3){

	uart_buf_len1 = sprintf(uart_buf1,"In COMP\n\n");
	HAL_UART_Transmit(huart2, (uint8_t *)uart_buf1, uart_buf_len1, 100);

	float Vref = 2.3;						//reference voltage
	int VrefLevel = (int)(Vref*4095/3.3);	//reference voltage value in term of voltage level


	HAL_COMP_Start(hcomp1);
	HAL_TIM_Base_Start_IT(htim16);	//start the timer

	//Loop for 15 times to show the results clearly.
	int b=0;
	while (b<15){
		updateUnit(frequency_COMP, velocity_COMP, hadc2, huart2, huart3, 1);

		uart_buf_len1 = sprintf(uart_buf1,"C Frequency: %0.2f \r\n",frequency_COMP);			//transmit freq to serial port
		HAL_UART_Transmit(huart2, (uint8_t *)uart_buf1, uart_buf_len1, 100);	//
		uart_buf_len1 = sprintf(uart_buf1,"C Speed: %.2f m/s\r\n",velocity_COMP);			//transmit speed to serial port
		HAL_UART_Transmit(huart2, (uint8_t *)uart_buf1, uart_buf_len1, 100);	//

		b++;
	}
}
