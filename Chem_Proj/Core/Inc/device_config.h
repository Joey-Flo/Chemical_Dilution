// Core/Inc/device_config.h

#ifndef INC_DEVICE_CONFIG_H_
#define INC_DEVICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "shared_types.h"

// --- Configuration Constants ---
#define CONFIG_MAGIC_NUMBER 0xFEEDC0DE
#define CONFIG_VERSION      2 // <-- Increment version again due to new layout




// --- The Main Device Configuration Structure ---
typedef struct __attribute__((packed))
{
    // --- Header (8 bytes) ---
    uint32_t magic_number;
    uint16_t config_version;
    uint16_t _header_padding;

    // --- System-wide Pump Physical Configuration (40 bytes) ---
    int8_t PumpEnable[NUM_PUMPS];
    float  PumpDensity[NUM_PUMPS];

    // --- YOUR ADDITION IS NOW A GLOBAL SETTING (1 byte) ---
    int8_t VolumeUnit; // 0 for milliliters (mL), 1 for ounces (Oz)

    // --- Chemical Recipe Definitions (504 bytes) ---
    ChemicalRecipe_t recipes[NUM_CHEMICAL_RECIPES];

    // --- Data Integrity and Padding (7 bytes) ---

    // NEW PADDING: Ensures the data block for CRC is a multiple of 4.
    // Total data size before this is 8+40+1+504 = 553 bytes.
    // 553 + 3 = 556, which is divisible by 4.
    uint8_t _internal_crc_padding[3];

    // The CRC32 checksum of all data *before* this field (556 bytes).
    uint32_t crc32_checksum;

    // NO FINAL PADDING NEEDED:
    // The total size is now 556 (CRC data) + 4 (CRC) = 560 bytes.
    // 560 is a perfect multiple of 8, so no final padding is required.

} DeviceConfiguration_t;


// --- Function Prototypes ---
HAL_StatusTypeDef Config_Save(const DeviceConfiguration_t* config);
HAL_StatusTypeDef Config_Load(DeviceConfiguration_t* config);


#ifdef __cplusplus
}
#endif

#endif /* INC_DEVICE_CONFIG_H_ */
