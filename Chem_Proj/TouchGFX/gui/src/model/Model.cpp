#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "device_config.h"
#include <cstring>

extern DeviceConfiguration_t myDeviceConfig; // The model has access to the real data

Model::Model() : modelListener(0)
{

}

void Model::tick()
{

}


int8_t Model::getPumpEnableState(int8_t index)
{
    if (index >= 0 && index < 8) {
        return myDeviceConfig.PumpEnable[index];
    }
    return 0; // Return a safe default
}

void Model::setPumpEnableState(int8_t index, bool is_enabled)
{
    if (index >= 0 && index < 8) {
        myDeviceConfig.PumpEnable[index] = is_enabled ? 1 : 0;

        // The save logic now lives in the model, where it belongs.
        #ifndef SIMULATOR
            Config_Save(&myDeviceConfig);
        #endif
    }
}

float Model::getPumpDensity(int8_t index)
{
    if (index >= 0 && index < 8) {
        return myDeviceConfig.PumpDensity[index];
    }
    return 1.0f; // Return a safe default
}

void Model::setPumpDensity(int8_t index, float new_density)
{
    if (index >= 0 && index < 8) {
        // Here you can add clamping/validation if you want
        if (new_density > 3.0f) { new_density = 3.0f; }
        if (new_density < 0.1f) { new_density = 0.1f; }

        myDeviceConfig.PumpDensity[index] = new_density;

        // The save logic lives in the Model
        #ifndef SIMULATOR
            Config_Save(&myDeviceConfig);
        #endif
    }
}

const ChemicalRecipe_t& Model::getRecipeData(int recipe_index) const
{
    // Safety check to prevent reading out of the array bounds
    if (recipe_index < 0 || recipe_index >= NUM_CHEMICAL_RECIPES) {
        recipe_index = 0;
    }
    return myDeviceConfig.recipes[recipe_index];
}

std::vector<int> Model::getEnabledPumpIndices() const
{
    std::vector<int> enabled_pumps;
    for (int i = 0; i < NUM_PUMPS; ++i) {
        if (myDeviceConfig.PumpEnable[i] == 1) {
            enabled_pumps.push_back(i);
        }
    }
    return enabled_pumps;
}

int8_t Model::getVolumeUnit() const
{
    return myDeviceConfig.VolumeUnit;
}


// --- SETTER IMPLEMENTATIONS (Placeholders for now) ---
// We will fill these in later when we wire up the buttons.
// For now, they just need to exist so the program can compile.

void Model::updatePumpSetup(int recipe_index, int setup_index, const PumpSetup_t& data)
{
    // A safety check
    if (recipe_index >= 0 && recipe_index < NUM_CHEMICAL_RECIPES &&
        setup_index >= 0 && setup_index < MAX_PUMP_SETUPS_PER_CHEMICAL) {

        // 1. Update the master data structure in RAM
        myDeviceConfig.recipes[recipe_index].pump_setups[setup_index] = data;

        // 2. Save the entire configuration to flash
        #ifndef SIMULATOR
        Config_Save(&myDeviceConfig);
        #endif
    }
}

void Model::updateSinglePumpVolume(int recipeIndex, int setupIndex, int fieldIndex, float newVolume)
{
    // 1. --- Safety Checks ---
    // It's crucial to ensure the indices provided are valid to prevent memory corruption.
    if (recipeIndex < 0 || recipeIndex >= NUM_CHEMICAL_RECIPES ||
        setupIndex < 0 || setupIndex >= MAX_PUMP_SETUPS_PER_CHEMICAL ||
        fieldIndex < 0 || fieldIndex > 2) // fieldIndex can only be 0, 1, or 2
    {
        // If any index is out of bounds, do nothing to protect the system.
        return;
    }

    // 2. --- Locate the Specific Data ---
    // Get a direct pointer to the pump setup struct we need to modify.
    // This makes the following code cleaner and easier to read.
    PumpSetup_t* setup = &myDeviceConfig.recipes[recipeIndex].pump_setups[setupIndex];

    // 3. --- Update the Correct Field ---
    // Use a switch statement on the fieldIndex to modify the correct variable.
    switch (fieldIndex)
    {
        case 0: // 0 corresponds to the "Small" field
            setup->dispense_small = newVolume;
            break;
        case 1: // 1 corresponds to the "Medium" field
            setup->dispense_medium = newVolume;
            break;
        case 2: // 2 corresponds to the "Large" field
            setup->dispense_large = newVolume;
            break;
    }

    // 4. --- Save to Flash ---
    // After updating the value in RAM, save the entire configuration
    // back to flash memory to make the change persistent.
    #ifndef SIMULATOR
        Config_Save(&myDeviceConfig);
    #endif
}

void Model::updateChemicalName(int recipe_index, const char* name)
{
    if (recipe_index >= 0 && recipe_index < NUM_CHEMICAL_RECIPES) {
        strncpy(myDeviceConfig.recipes[recipe_index].name, name, sizeof(myDeviceConfig.recipes[recipe_index].name) - 1);
        myDeviceConfig.recipes[recipe_index].name[sizeof(myDeviceConfig.recipes[recipe_index].name) - 1] = '\0';
        Config_Save(&myDeviceConfig);
    }
}

void Model::updateTotalVolume(int recipe_index, float volume)
{
    if (recipe_index >= 0 && recipe_index < NUM_CHEMICAL_RECIPES) {
        myDeviceConfig.recipes[recipe_index].total_dispense_volume = volume;
        Config_Save(&myDeviceConfig);
    }
}

void Model::addPumpToRecipe(int recipe_index)
{
    // Safety check
    if (recipe_index < 0 || recipe_index >= NUM_CHEMICAL_RECIPES) {
        return;
    }

    // Find the first available "unused" slot in this recipe's pump setups
    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        if (myDeviceConfig.recipes[recipe_index].pump_setups[i].pump_index == -1)
        {
            // Found an empty slot!
            // Initialize it with safe default values.
            myDeviceConfig.recipes[recipe_index].pump_setups[i].pump_index = 0; // Default to Pump 1
            myDeviceConfig.recipes[recipe_index].pump_setups[i].dispense_small = 0.0f;
            myDeviceConfig.recipes[recipe_index].pump_setups[i].dispense_medium = 0.0f;
            myDeviceConfig.recipes[recipe_index].pump_setups[i].dispense_large = 0.0f;

            // Save the new configuration to flash
            Config_Save(&myDeviceConfig);

            // We're done, so we exit the loop and the function
            return;
        }
    }
    // If we get here, it means all 3 slots were already full. Do nothing.
}


void Model::removePumpFromRecipe(int recipe_index)
{
    // Safety check
    if (recipe_index < 0 || recipe_index >= NUM_CHEMICAL_RECIPES) {
        return;
    }

    // Find the LAST active pump setup to remove it. We search backwards.
    for (int i = MAX_PUMP_SETUPS_PER_CHEMICAL - 1; i >= 0; --i)
    {
        if (myDeviceConfig.recipes[recipe_index].pump_setups[i].pump_index != -1)
        {
            // Found the last active slot!
            // "Remove" it by setting its index back to -1.
            myDeviceConfig.recipes[recipe_index].pump_setups[i].pump_index = -1;

            // Save the new configuration to flash
            Config_Save(&myDeviceConfig);

            // We're done, so we exit the loop and the function
            return;
        }
    }
    // If we get here, it means there were no active setups to remove. Do nothing.
}

void Model::setChemicalEnableState(int recipe_index, bool is_enabled)
{
    if (recipe_index >= 0 && recipe_index < NUM_CHEMICAL_RECIPES) {
        myDeviceConfig.recipes[recipe_index].is_enabled = is_enabled ? 1 : 0;
        Config_Save(&myDeviceConfig);
    }
}



void Model::updateVolumeUnit(int8_t unit)
{
    // Future logic will go here
}

void Model::toggleVolumeUnit()
{
    // Get the current unit value (0 or 1)
    int8_t currentUnit = myDeviceConfig.VolumeUnit;

    // Flip the value: if it's 0 it becomes 1, if it's 1 it becomes 0.
    // A simple way to do this is with the NOT operator on a boolean conversion.
    myDeviceConfig.VolumeUnit = !currentUnit;

    // Save the entire updated configuration back to flash memory.
    Config_Save(&myDeviceConfig);
}

void Model::updateBrightness(uint8_t brightness)
{
    // 1. Validate the incoming value to be safe.
    if (brightness < 20) brightness = 20;
    if (brightness > 100) brightness = 100;

    // 2. Update the value in the master configuration struct in RAM.
    myDeviceConfig.brightness_level = brightness;

    // 3. Save the entire configuration to flash memory.
    Config_Save(&myDeviceConfig);
}

void Model::updateScaleCalibrationFactor(float newFactor)
{
    // Add a safety check to prevent saving zero or negative factors
    if (newFactor > 0.0f) {
        myDeviceConfig.scale_calibration_factor = newFactor;
        Config_Save(&myDeviceConfig);
    }
}

float Model::getScaleCalibrationFactor() const
{
    return myDeviceConfig.scale_calibration_factor;
}

uint8_t Model::getBrightness() const
{
    // Return the value directly from the master configuration struct.
    return myDeviceConfig.brightness_level;
}
