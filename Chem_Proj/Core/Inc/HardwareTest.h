/*
 * BuzzerTest.h
 *
 *  Created on: Jul 25, 2025
 *      Author: joeyf
 */

#ifndef INC_HARDWARETEST_H_
#define INC_HARDWARETEST_H_

#include <HardwareTest.h>
#include "main.h"
#include "PWMDriver.h"

void PlayHappyBirthday(TIM_HandleTypeDef *htim, uint32_t Channel);
void WeightTest(int min, int max, TIM_HandleTypeDef *htim, uint32_t Channel);
void MultiplexerTest(void);

#endif /* INC_HARDWARETEST_H_ */
