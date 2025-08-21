// In Core/Inc/ValveDriver.h
#ifndef INC_VALVEDRIVER_H_
#define INC_VALVEDRIVER_H_

#include "main.h"

/**
 * @brief Opens the solenoid valve by setting the VALVE_CTRL pin HIGH.
 */
void Valve_Open(void);

/**
 * @brief Closes the solenoid valve by setting the VALVE_CTRL pin LOW.
 */
void Valve_Close(void);

#endif /* INC_VALVEDRIVER_H_ */
