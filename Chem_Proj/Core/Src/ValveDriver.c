// In Core/Src/ValveDriver.c
#include "ValveDriver.h"

void Valve_Open(void)
{
    // These pin/port names come from your main.h file
    HAL_GPIO_WritePin(VALVE_CTRL_GPIO_Port, VALVE_CTRL_Pin, GPIO_PIN_SET);
}

void Valve_Close(void)
{
    HAL_GPIO_WritePin(VALVE_CTRL_GPIO_Port, VALVE_CTRL_Pin, GPIO_PIN_RESET);
}
