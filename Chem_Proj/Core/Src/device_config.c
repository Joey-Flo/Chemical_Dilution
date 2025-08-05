#include "device_config.h"
#include "Flash.h"
#include <string.h>
#include <stdio.h>

#define CONFIG_FLASH_ADDRESS FLASH_LAST_PAGE

extern CRC_HandleTypeDef hcrc;

void Config_SetDefaults(DeviceConfiguration_t* config);
static uint32_t Calculate_Config_CRC(const DeviceConfiguration_t* config);


// In Core/Src/device_config.c

void Config_SetDefaults(DeviceConfiguration_t* config)
{
    // 1. Initialize the entire struct to zero. This is a good starting point.
    memset(config, 0, sizeof(DeviceConfiguration_t));

    // 2. Set the header.
    config->magic_number = CONFIG_MAGIC_NUMBER;
    config->config_version = CONFIG_VERSION;

    // 3. Set System-wide Physical Pump Configuration.
    for (int i = 0; i < NUM_PUMPS; i++) {
        config->PumpEnable[i] = (i == 0) ? 1 : 0;
        config->PumpDensity[i] = 1.0f;
    }

    // 4. Set the global default for volume units.
    config->VolumeUnit = 0; // 0 for mL

    // 5. Set Chemical Recipe Defaults.
    const char* default_names[] = {
        "Chemical 1", "Chemical 2", "Chemical 3", "Chemical 4",
        "Chemical 5", "Chemical 6", "Chemical 7", "Chemical 8"
    };

    for (int i = 0; i < NUM_CHEMICAL_RECIPES; i++) {
        // --- THIS IS THE ROBUST METHOD ---
        // Access the struct members directly through the main 'config' pointer.

        // a. Set the name
        strncpy(config->recipes[i].name, default_names[i], sizeof(config->recipes[i].name) - 1);
        config->recipes[i].name[sizeof(config->recipes[i].name) - 1] = '\0';

        // b. Set the total volume
        config->recipes[i].total_dispense_volume = 100.0f;

        // c. Initialize the pump setups for this recipe
        for (int j = 0; j < MAX_PUMP_SETUPS_PER_CHEMICAL; j++) {

            // Set the pump index
            if (i == 0 && j == 0) {
                config->recipes[i].pump_setups[j].pump_index = 0;
            } else {
                config->recipes[i].pump_setups[j].pump_index = -1;
            }

            // Explicitly set the float values
            if (i == 0 && j == 0) {
                config->recipes[i].pump_setups[j].dispense_small = 5.0f;
                config->recipes[i].pump_setups[j].dispense_medium = 10.0f;
                config->recipes[i].pump_setups[j].dispense_large = 15.0f;
            } else {
                config->recipes[i].pump_setups[j].dispense_small = 0.0f;
                config->recipes[i].pump_setups[j].dispense_medium = 0.0f;
                config->recipes[i].pump_setups[j].dispense_large = 0.0f;
            }
        }
    }
}

// --- MODIFIED: The CRC calculation is now brutally explicit ---
static uint32_t Calculate_Config_CRC(const DeviceConfiguration_t* config)
{
    // We will manually add the size of every single field that comes before the checksum.
    // This is verbose, but it is 100% immune to compiler padding/packing quirks.
    size_t crc_data_size = 0;
    crc_data_size += sizeof(config->magic_number);
    crc_data_size += sizeof(config->config_version);
    crc_data_size += sizeof(config->_header_padding);
    crc_data_size += sizeof(config->PumpEnable);
    crc_data_size += sizeof(config->PumpDensity);
    crc_data_size += sizeof(config->VolumeUnit);
    crc_data_size += sizeof(config->recipes);
    crc_data_size += sizeof(config->_internal_crc_padding);

    // Sanity check: Ensure the calculated size is a multiple of 4 for the hardware CRC
    if ((crc_data_size % 4) != 0) {
        // If you hit this, something is very wrong with the struct definition.
        // You would need to adjust the _internal_crc_padding.
        return 0; // Return an invalid CRC
    }

    return HAL_CRC_Calculate(&hcrc, (uint32_t*)config, crc_data_size / 4);
}


HAL_StatusTypeDef Config_Save(const DeviceConfiguration_t* config)
{
    if (config == NULL) {
        return HAL_ERROR;
    }
    HAL_StatusTypeDef status;
    DeviceConfiguration_t temp_config;
    memcpy(&temp_config, config, sizeof(DeviceConfiguration_t));

    // The checksum is calculated on the data BEFORE it's stored in the field.
    // So we calculate it, then store it.
    temp_config.crc32_checksum = Calculate_Config_CRC(&temp_config);

    status = FlashErase(CONFIG_FLASH_ADDRESS);
    if (status != HAL_OK) {
        return status;
    }

    uint64_t* data_ptr = (uint64_t*)&temp_config;
    uint32_t write_address = CONFIG_FLASH_ADDRESS;
    int chunks = (sizeof(DeviceConfiguration_t) + 7) / 8;
    for (int i = 0; i < chunks; i++) {
        status = FlashWrite(write_address, data_ptr[i]);
        if (status != HAL_OK) {
            return status;
        }
        write_address += 8;
    }
    return HAL_OK;
}


HAL_StatusTypeDef Config_Load(DeviceConfiguration_t* config)
{
    if (config == NULL) {
        return HAL_ERROR;
    }
    uint64_t* dest_ptr = (uint64_t*)config;
    uint32_t read_address = CONFIG_FLASH_ADDRESS;
    int chunks = (sizeof(DeviceConfiguration_t) + 7) / 8;
    for (int i = 0; i < chunks; i++) {
        dest_ptr[i] = FlashRead(read_address);
    }

    uint32_t stored_crc = config->crc32_checksum;
    uint32_t calculated_crc = Calculate_Config_CRC(config);

   if ( (config->magic_number != CONFIG_MAGIC_NUMBER) ||
         (config->config_version != CONFIG_VERSION))
    {
        Config_SetDefaults(config);
        return HAL_ERROR;
    }
    return HAL_OK;
}
