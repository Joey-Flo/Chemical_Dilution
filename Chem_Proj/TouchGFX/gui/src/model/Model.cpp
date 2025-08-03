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
