#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstdint>
#include "shared_types.h"
#include <vector>

class ModelListener;


class Model
{
public:

    int8_t getPumpEnableState(int8_t index);
    void setPumpEnableState(int8_t index, bool is_enabled);

    float getPumpDensity(int8_t index);
    void setPumpDensity(int8_t index, float new_density);

    void addPumpToRecipe(int recipe_index);
    void removePumpFromRecipe(int recipe_index);

    void setChemicalEnableState(int recipe_index, bool is_enabled);

    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    const ChemicalRecipe_t& getRecipeData(int recipe_index) const;
    std::vector<int> getEnabledPumpIndices() const;

    // Returns the global volume unit (0=mL, 1=Oz)
    int8_t getVolumeUnit() const;


    // --- SETTERS (for saving data - we will implement these later) ---

    void updatePumpSetup(int recipe_index, int setup_index, const PumpSetup_t& data);
    void updateChemicalName(int recipe_index, const char* name);
    void updateTotalVolume(int recipe_index, float volume);
    void updateVolumeUnit(int8_t unit); // Corresponds to the global setting
    void updateSinglePumpVolume(int recipeIndex, int setupIndex, int fieldIndex, float newVolume);
    void toggleVolumeUnit();
    void updateBrightness(uint8_t brightness);
    void updateScaleCalibrationFactor(float newFactor);
    float getScaleCalibrationFactor() const;
    uint8_t getBrightness() const;
    void setSelectedRecipeIndex(int8_t index);
    int8_t getSelectedRecipeIndex() const;
    void setSelectedDispenseSize(int8_t size);
    int8_t getSelectedDispenseSize() const;


protected:
    ModelListener* modelListener;
    int8_t selectedRecipeIndex;
    int8_t selectedDispenseSize;
};

#endif // MODEL_HPP
