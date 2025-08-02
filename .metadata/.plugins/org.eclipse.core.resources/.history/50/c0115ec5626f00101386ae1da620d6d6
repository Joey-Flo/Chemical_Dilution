// Core/Inc/device_config.h

#ifndef INC_DEVICE_CONFIG_H_
#define INC_DEVICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32l4xx_hal.h"

#define CONFIG_MAGIC_NUMBER 0xFEEDC0DE
#define CONFIG_VERSION      0
#define MAX_DISPENSE_OPTIONS_PER_CHEMICAL 3

typedef struct __attribute__((packed)) {
    int8_t  pump_index;
    float   amount;
} PumpDispenseInstruction_t;

// --- Main Device Configuration Structure (REVISED for ALIGNMENT) ---
typedef struct __attribute__((packed))
{
    // --- Header (8 bytes) ---
    uint32_t magic_number;
    uint16_t config_version;
    uint16_t _header_padding;

    // --- System-wide Pump Physical Configuration (40 bytes) ---
    int8_t PumpEnable[8];
    float  PumpDensity[8];

    // --- Chemical Recipe Definitions (288 bytes) ---
    char ChemicalName[8][20];
    int8_t PumpAmountPerChemical[8];
    PumpDispenseInstruction_t ChemicalDispenseInstructions[8][MAX_DISPENSE_OPTIONS_PER_CHEMICAL];

    // --- Data Integrity (4 bytes) ---
    // The CRC is now placed BEFORE the final padding.
    // The CRC will be calculated on all fields *before* this one.
    // The size of the data before this point (336 bytes) is a multiple of 4.
    uint32_t crc32_checksum;

    // --- Final Padding (4 bytes) ---
    // This padding makes the TOTAL struct size a multiple of 8 bytes for flash writing.
    // Total size: 336 (data) + 4 (CRC) + 4 (padding) = 344 bytes.
    // 344 is a multiple of 8, so our 64-bit flash writes will be perfect.
    uint8_t _final_padding[4];

} DeviceConfiguration_t;

HAL_StatusTypeDef Config_Save(const DeviceConfiguration_t* config);
HAL_StatusTypeDef Config_Load(DeviceConfiguration_t* config);

#ifdef __cplusplus
}
#endif

#endif /* INC_DEVICE_CONFIG_H_ */
