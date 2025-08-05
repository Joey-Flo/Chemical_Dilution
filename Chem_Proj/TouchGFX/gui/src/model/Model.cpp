#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "device_config.h"
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

void Model::addPumpToRecipe(int recipe_index)
{
    // Future logic will go here
}

void Model::removePumpFromRecipe(int recipe_index)
{
    // Future logic will go here
}

void Model::updateChemicalName(int recipe_index, const char* name)
{
    // Future logic will go here
}

void Model::updateTotalVolume(int recipe_index, float volume)
{
    // Future logic will go here
}

void Model::updateVolumeUnit(int8_t unit)
{
    // Future logic will go here
}
