/*
 * PWMDriver.h
 *
 *  Created on: Jul 25, 2025
 *      Author: joeyf
 */

#ifndef INC_PWMDRIVER_H_
#define INC_PWMDRIVER_H_

#include "main.h"
#include "HAL_Init.h"

// Define the total number of steps to represent the 0-100% range with 0.1% granularity
#define PWM_GRANULARITY_STEPS 1000

void DutyCycleAdjust(TIM_HandleTypeDef* htim, uint32_t channel, uint16_t duty_cycle_value);
void FrequencyAdjust(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t frequency_hz);
uint32_t get_tim_clock_freq(TIM_HandleTypeDef* htim);


#endif /* INC_PWMDRIVER_H_ */
