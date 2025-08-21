// In Core/Src/PumpManager.c

#include "PumpManager.h"
#include "MuxDriver.h"   // For PumpChannelSelect()
#include "PWMDriver.h"   // For DutyCycleAdjust()

// We need access to the timer handle that drives the pump PWM.
// Based on your previous files, htim4 is a likely candidate.
// Verify this is the correct timer for your pump control.
extern TIM_HandleTypeDef htim4;

/**
 * @brief Selects and activates a specific pump at 100% duty cycle.
 * @param pump_index The index of the pump to turn on (0-7).
 */
void Prime_Pump_Start(uint8_t pump_index)
{
    // 1. Safety check for valid pump index.
    if (pump_index > 8) {
        return;
    }

    // 2. Use the MuxDriver to select the correct output channel for this pump.
    PumpChannelSelect(pump_index);

    // 3. Use the PWMDriver to set the duty cycle to 100%.
    //    We use PWM_GRANULARITY_STEPS (which should be 1000) for 100%.
    //    We assume the PWM is on TIM_CHANNEL_1 of htim4. Verify this.
    DutyCycleAdjust(&htim5, TIM_CHANNEL_2, PWM_GRANULARITY_STEPS);
}


/**
 * @brief Deactivates the currently selected pump by setting its duty cycle to 0%.
 * @param pump_index The index of the pump to turn off (0-7).
 */
void Prime_Pump_Stop(uint8_t pump_index)
{
    // 1. Safety check for valid pump index.
    if (pump_index > 7) {
        return;
    }

    // It's good practice to ensure the correct pump is still selected,
    // though not strictly necessary if you only prime one at a time.
    PumpChannelSelect(pump_index);

    // 2. Use the PWMDriver to set the duty cycle to 0%.
    DutyCycleAdjust(&htim5, TIM_CHANNEL_2, 0);
}
