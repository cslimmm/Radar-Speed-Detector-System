/*
 * COMP.h
 *
 *  Created on: Nov 29, 2024
 *      Author: cslim
 */

#ifndef INC_COMP_H_
#define INC_COMP_H_
#endif /* INC_COMP_H_ */

void HAL_COMP_TriggerCallback(COMP_HandleTypeDef* hcomp);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
void COMP(COMP_HandleTypeDef* hcomp1, UART_HandleTypeDef* huart2, TIM_HandleTypeDef* htim16, ADC_HandleTypeDef* hadc2,UART_HandleTypeDef* huart3);
