// In Core/Inc/PumpManager.h

#ifndef INC_PUMPMANAGER_H_
#define INC_PUMPMANAGER_H_

#include "main.h" // Includes all the necessary HAL and peripheral headers

// --- PUBLIC FUNCTION PROTOTYPES ---

/**
 * @brief Selects and activates a specific pump at 100% duty cycle.
 * @param pump_index The index of the pump to turn on (0-7).
 */
void Prime_Pump_Start(uint8_t pump_index);

/**
 * @brief Deactivates a specific pump by setting its duty cycle to 0%.
 * @param pump_index The index of the pump to turn off (0-7).
 *        Note: This parameter is for future-proofing, in case different pumps
 *        use different timers. For now, it's not strictly necessary but good practice.
 */
void Prime_Pump_Stop(uint8_t pump_index);


#endif /* INC_PUMPMANAGER_H_ */
