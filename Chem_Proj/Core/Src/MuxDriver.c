/*
 * MuxDriver.c
 *
 * Created on: Jul 26, 2025
 * Author: joeyf
 */
#include "MuxDriver.h" // This should include stm32l4xx_hal.h and main.h

// Make arrays static if they are only used within this .c file.
// This limits their scope and prevents name collisions.
static const int8_t BinaryLookup[8][3] = {
//   C B A
    {0,0,0}, // Row 0
    {0,0,1}, // Row 1
    {0,1,0}, // Row 2
    {0,1,1}, // Row 3
    {1,0,0}, // Row 4
    {1,0,1}, // Row 5
    {1,1,0}, // Row 6
    {1,1,1}  // Row 7
};

static const uint16_t PinLookup[3] = {MUX_CTRL_C_Pin, MUX_CTRL_B_Pin, MUX_CTRL_A_Pin};
// Changed to uint16_t to match GPIO_Pin type, and added 'const' as it's read-only.


// Driving the CD4051 Mux Chip
// Drives outputs 0-7
void PumpChannelSelect(int8_t outputnum){
    // Basic input validation
    if (outputnum < 0 || outputnum > 7) {
        // Optionally, handle error or set to a known safe state (e.g., all control pins low)
        // For example, deselect all channels:
        HAL_GPIO_WritePin(GPIOC, MUX_CTRL_C_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, MUX_CTRL_B_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOC, MUX_CTRL_A_Pin, GPIO_PIN_RESET);
        return; // Exit if invalid input
    }

	for (int i = 0; i < 3; i++){
        // Explicitly cast the int8_t value to GPIO_PinState enum for type safety.
        // GPIO_PIN_RESET (0) and GPIO_PIN_SET (1) are the typical values.
		HAL_GPIO_WritePin(GPIOC, PinLookup[i], (GPIO_PinState)BinaryLookup[outputnum][i]);
	}
}
