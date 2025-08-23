#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>
#include <gui/common/FieldIDs.hpp>
#include <cstdlib>
#include "main.h"

ChemicalsSetupPresenter::ChemicalsSetupPresenter(ChemicalsSetupView& v)
    : view(v),
      currentlyEditingFieldID(FIELD_NONE),
      activePageIndex(0),
      activeSetupIndex(0),
      activeFieldIndex(0)
{
}

void ChemicalsSetupPresenter::activate()
{
    // When the screen first opens, tell the View to trigger a data load for its initial page.
    view.requestDataLoad();
}

void ChemicalsSetupPresenter::deactivate()
{
}

void ChemicalsSetupPresenter::loadScreenData(int page_index)
{
    // Update the Presenter's internal state to remember the active page.
    activePageIndex = page_index;

    // Get all necessary data from the Model.
    const ChemicalRecipe_t& recipe = model->getRecipeData(activePageIndex);
    std::vector<int> enabled_pumps = model->getEnabledPumpIndices();
    int8_t unit = model->getVolumeUnit();

    // Pass the data to the View for display.
    view.displayData(recipe, enabled_pumps, unit);
}

void ChemicalsSetupPresenter::savePumpSetupData(int page_index, int setup_index, const PumpSetup_t& data)
{
    model->updatePumpSetup(page_index, setup_index, data);
}

void ChemicalsSetupPresenter::editField(int fieldID)
{
    currentlyEditingFieldID = fieldID;

    // If it's a pump setup field, calculate the specific indices.
    if (fieldID >= FIELD_PUMP_SETUP_START) {
        int relative_id = fieldID - FIELD_PUMP_SETUP_START;
        activeSetupIndex = relative_id / 10;
        activeFieldIndex = relative_id % 10;
    }

    view.showKeyboard();
}

void ChemicalsSetupPresenter::ActiveFieldIndexUpdate(uint8_t index)
{
    activePageIndex = index;
}

void ChemicalsSetupPresenter::newValueEntered(const char* text)
{
    // Define the conversion factor at the top for easy reference.
    const float ML_PER_OZ = 29.5735f;

    // --- 1. PREPARE FOR VALIDATION ---
    char* end_ptr;
    float new_float_value = strtof(text, &end_ptr);
    bool is_valid_float = (*end_ptr == '\0' && end_ptr != text);

    // Get the current recipe data and the active unit from the model.
    const ChemicalRecipe_t& currentRecipe = model->getRecipeData(activePageIndex);
    int8_t currentUnit = model->getVolumeUnit();


    // --- 2. APPLY VALIDATION AND CONVERSION LOGIC ---
    switch (currentlyEditingFieldID)
    {
        case FIELD_CHEM_NAME:
        {
            char truncated_name[20];
            strncpy(truncated_name, text, 19);
            truncated_name[19] = '\0';
            model->updateChemicalName(activePageIndex, truncated_name);
            break;
        }

        case FIELD_TOTAL_VOLUME:
        {
            if (is_valid_float) {
                float new_total_volume_input = new_float_value;

                // --- NEW LOGIC: CONVERT IF NECESSARY ---
                // If the user is in Oz mode, convert their input to mL before proceeding.
                if (currentUnit == 1) { // 1 = Oz
                    new_total_volume_input *= ML_PER_OZ;
                }

                // Now, all subsequent logic uses the value in mL.
                float old_total_volume_mL = currentRecipe.total_dispense_volume;
                if (old_total_volume_mL <= 0.0f) old_total_volume_mL = 1.0f;

                // Validate and cap the NEW total volume (in mL).
                float new_total_volume_mL = new_total_volume_input;
                if (new_total_volume_mL > 946.4f) new_total_volume_mL = 946.4f; // Max is ~32 Oz
                if (new_total_volume_mL < 0.0f) new_total_volume_mL = 0.0f;

                float scaling_factor = new_total_volume_mL / old_total_volume_mL;

                for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i) {
                    PumpSetup_t pump_setup = currentRecipe.pump_setups[i];
                    pump_setup.dispense_small *= scaling_factor;
                    pump_setup.dispense_medium *= scaling_factor;
                    pump_setup.dispense_large *= scaling_factor;
                    model->updatePumpSetup(activePageIndex, i, pump_setup);
                }
                model->updateTotalVolume(activePageIndex, new_total_volume_mL);
            }
            break;
        }

        case FIELD_PUMP1_S: case FIELD_PUMP1_M: case FIELD_PUMP1_L:
        case FIELD_PUMP2_S: case FIELD_PUMP2_M: case FIELD_PUMP2_L:
        case FIELD_PUMP3_S: case FIELD_PUMP3_M: case FIELD_PUMP3_L:
        {
            if (is_valid_float) {
                float new_pump_volume_input = new_float_value;

                // --- NEW LOGIC: CONVERT IF NECESSARY ---
                // If the user is in Oz mode, convert their input to mL.
                if (currentUnit == 1) { // 1 = Oz
                    new_pump_volume_input *= ML_PER_OZ;
                }

                // All subsequent validation is now performed on the mL value.
                float total_volume_limit_mL = currentRecipe.total_dispense_volume;
                float sum_of_other_pumps_mL = 0.0f;
                for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i) {
                    if (i == activeSetupIndex || currentRecipe.pump_setups[i].pump_index == -1) continue;
                    if (activeFieldIndex == 0) sum_of_other_pumps_mL += currentRecipe.pump_setups[i].dispense_small;
                    else if (activeFieldIndex == 1) sum_of_other_pumps_mL += currentRecipe.pump_setups[i].dispense_medium;
                    else sum_of_other_pumps_mL += currentRecipe.pump_setups[i].dispense_large;
                }

                float max_allowed_value_mL = total_volume_limit_mL - sum_of_other_pumps_mL;
                if (max_allowed_value_mL < 0.0f) max_allowed_value_mL = 0.0f;
                if (new_pump_volume_input > max_allowed_value_mL) new_pump_volume_input = max_allowed_value_mL;
                if (new_pump_volume_input < 0.0f) new_pump_volume_input = 0.0f;

                // Pass the final, validated mL value to the model.
                model->updateSinglePumpVolume(activePageIndex, activeSetupIndex, activeFieldIndex, new_pump_volume_input);
            }
            break;
        }

        default:
            break;
    }

    // --- 3. RELOAD THE SCREEN ---
    PlaySound(1);
    loadScreenData(activePageIndex);
}

void ChemicalsSetupPresenter::addPumpSetup(int page_index)
{
    model->addPumpToRecipe(page_index);
    PlaySound(1);
    loadScreenData(page_index);
}

void ChemicalsSetupPresenter::removePumpSetup(int page_index)
{
    model->removePumpFromRecipe(page_index);
    PlaySound(1);
    loadScreenData(page_index);
}

void ChemicalsSetupPresenter::chemicalEnableToggled()
{
    // This function correctly uses the reliable activePageIndex
    const ChemicalRecipe_t& recipe = model->getRecipeData(activePageIndex);
    bool new_state_is_on = !(recipe.is_enabled == 1);
    model->setChemicalEnableState(activePageIndex, new_state_is_on);
    view.invalidateEnableButton();
    PlaySound(1);
}

const ChemicalRecipe_t& ChemicalsSetupPresenter::getRecipeDataForPage(int page_index) const
{
    // The presenter gets the data from its model and passes it back to the view.
    return model->getRecipeData(page_index);
}

const std::vector<int> ChemicalsSetupPresenter::getEnabledPumpIndices() const
{
    // The presenter gets the data from its model and passes it back to the view.
    return model->getEnabledPumpIndices();
}

int8_t ChemicalsSetupPresenter::getVolumeUnit() const
{
    // The presenter gets the data from its model and passes it back to the view.
    return model->getVolumeUnit();
}
