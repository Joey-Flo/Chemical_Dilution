// In Core/Inc/DispenseManager_shared_types.h

#ifndef INC_DISPENSEMANAGER_SHARED_TYPES_H_
#define INC_DISPENSEMANAGER_SHARED_TYPES_H_

// This header is safe to include in both C and C++ files.
// It has NO other includes.

#include <stdint.h> // Good practice for fixed-width types

// The different states of the dispense process
typedef enum {
    DISPENSE_STATE_IDLE,
    DISPENSE_STATE_WAITING_FOR_CONTAINER,
    DISPENSE_STATE_TARING,
    DISPENSE_STATE_START_PUMP,
    DISPENSE_STATE_DISPENSING_FAST,
    DISPENSE_STATE_DISPENSING_SLOW,
    DISPENSE_STATE_START_WATER,
    DISPENSE_STATE_WATER_FILL,
    DISPENSE_STATE_DONE,
    DISPENSE_STATE_CANCELLED
} DispenseState_e;

// A struct to hold all information about the current job
typedef struct {
    DispenseState_e state;
    int8_t   recipe_index;
    int8_t   current_pump_setup_index;
    int8_t   dispense_size; // 0=S, 1=M, 2=L
    float    total_target_weight_grams;
    float    current_target_weight_grams;
} DispenseJob_t;

#endif /* INC_DISPENSEMANAGER_SHARED_TYPES_H_ */
