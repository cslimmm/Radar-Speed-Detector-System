/*
 * ADC.cpp
 *
 *  Created on: Nov 29, 2024
 *      Author: cslim
 */

#include "main.h"
#include "FFT.h"
#include "arm_math.h"
#include <math.h>
#include <stdio.h>
#include "LCD.h"
#include "ADC.h"
#include "intToBcd.h"

int flag=0; //To make sure thh DMA is stop and FFT is allowed to perform.
float velocity_ADC=0.0;

int uart_buf_len;
char uart_buf[50];

float fwave=10525000000.0;
float frequency_ADC=0.0;

//Function to stop the sampling process.
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc1) {
	flag=1;
	HAL_ADC_Stop_DMA(hadc1);
}

//Function to start the ADC-based system.
void ADC(ADC_HandleTypeDef* hadc1, ADC_HandleTypeDef* hadc2, UART_HandleTypeDef* huart2, UART_HandleTypeDef* huart3){

	uart_buf_len = sprintf(uart_buf,"\nIn ADC\n\n");
	HAL_UART_Transmit(huart2, (uint8_t *)uart_buf, uart_buf_len, 100);


	//Parameter for performing FFT.
	struct ADC_param ADC_val={0};
	struct FFT_res FFT_val={0};

	ADC_val.bit=12;
	ADC_val.prescaler=64;
	ADC_val.sampling_time=247.5;
	ADC_val.speed=64000000;					//clock speed on .ioc file
	ADC_val.adc_buf_len=4096;				//buffer length
	uint32_t adc_buf[ADC_val.adc_buf_len];	//buffer array
	ADC_val.adc_buf=adc_buf;				//store the value of buffer array pointer

	HAL_ADC_Start_DMA(hadc1, (uint32_t*)adc_buf,ADC_val.adc_buf_len);

	//Loop for 10 times to show the results clearly.
	int a=0;
	while(a<10){
		if (flag==1){
			start_FFT(&flag,&ADC_val,&FFT_val);
			get_velocity(FFT_val.fdominant,fwave,&velocity_ADC);

			frequency_ADC=FFT_val.fdominant;

			updateUnit(frequency_ADC, velocity_ADC, hadc2, huart2, huart3,0);
			HAL_ADC_Start_DMA(hadc1, adc_buf,ADC_val.adc_buf_len);

			a++;
		}
	}
}


