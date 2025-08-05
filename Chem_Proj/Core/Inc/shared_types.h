// In Core/Inc/shared_types.h

#ifndef SHARED_TYPES_H_
#define SHARED_TYPES_H_

// This header is safe to be included by both C and C++ files.
// It uses C headers that are compatible with C++.
#include <stdint.h>

// --- Configuration Constants ---
#define MAX_PUMP_SETUPS_PER_CHEMICAL 3
#define NUM_CHEMICAL_RECIPES 8
#define NUM_PUMPS 8

// --- Data Structure Blueprints ---
// These are standard C structs. The __attribute__ is a GCC extension
// recognized by both the C and C++ compilers.

typedef struct __attribute__((packed)) {
    int8_t  pump_index;
    float   dispense_small;
    float   dispense_medium;
    float   dispense_large;
} PumpSetup_t;

typedef struct __attribute__((packed)) {
    char  name[20];
    float total_dispense_volume;
    PumpSetup_t pump_setups[MAX_PUMP_SETUPS_PER_CHEMICAL];
} ChemicalRecipe_t;


#endif /* SHARED_TYPES_H_ */
