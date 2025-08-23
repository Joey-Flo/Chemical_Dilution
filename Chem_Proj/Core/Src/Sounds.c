/*
 * Sounds.c
 *
 *  Created on: Aug 22, 2025
 *      Author: joeyf
 */
#include "main.h"

void PlaySound(uint8_t sound)
{
    // A 50% duty cycle is good for a buzzer. 500 out of 1000 steps.
    const uint16_t BUZZER_VOLUME = 500;

    switch (sound) {
        case 1: // Click
            FrequencyAdjust(&htim4, TIM_CHANNEL_1, 2500);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME);
            HAL_Delay(50);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Sound off
            break; // <-- ADD THIS

        case 2: // Success
            FrequencyAdjust(&htim4, TIM_CHANNEL_1, 1500);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME);
            HAL_Delay(150);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Brief pause
            HAL_Delay(50);
            FrequencyAdjust(&htim4, TIM_CHANNEL_1, 2500);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME); // Second tone
            HAL_Delay(150);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Sound off
            break; // <-- ADD THIS

        case 3: // Error
            FrequencyAdjust(&htim4, TIM_CHANNEL_1, 800);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME);
            HAL_Delay(200);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Brief pause
            HAL_Delay(75);
            FrequencyAdjust(&htim4, TIM_CHANNEL_1, 600);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME); // Second tone
            HAL_Delay(200);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Sound off
            break; // <-- ADD THIS

        case 4: // Warning
            FrequencyAdjust(&htim4, TIM_CHANNEL_1, 2000);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME);
            HAL_Delay(100);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Sound off
            HAL_Delay(75);
            // No need to change frequency again if it's the same
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME); // Sound back on
            HAL_Delay(100);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Sound off
            break; // <-- ADD THIS

        case 5: // Startup
            FrequencyAdjust(&htim4, TIM_CHANNEL_1, 1800);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME);
            HAL_Delay(100);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Brief pause
            HAL_Delay(50);
            FrequencyAdjust(&htim4, TIM_CHANNEL_1, 2400);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME);
            HAL_Delay(100);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Brief pause
            HAL_Delay(50);
            FrequencyAdjust(&htim4, TIM_CHANNEL_1, 3000);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, BUZZER_VOLUME);
            HAL_Delay(150);
            DutyCycleAdjust(&htim4, TIM_CHANNEL_1, 0); // Sound off
            break; // <-- ADD THIS

        default:
            // It's good practice to have a default case
            // in case an invalid sound number is passed.
            break;
    }
}


