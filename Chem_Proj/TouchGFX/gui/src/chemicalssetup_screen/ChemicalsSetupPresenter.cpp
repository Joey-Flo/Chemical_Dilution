#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>
#include <gui/common/FieldIDs.hpp>
#include <cstdlib>

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
    char* end_ptr;
    float new_float_value = strtof(text, &end_ptr);
    bool is_valid_float = (*end_ptr == '\0' && end_ptr != text);

    // Use the stored state and the reliable activePageIndex to update the model.
    switch (currentlyEditingFieldID)
    {
        case FIELD_CHEM_NAME:
            model->updateChemicalName(activePageIndex, text);
            break;

        case FIELD_TOTAL_VOLUME:
            if (is_valid_float) {
                model->updateTotalVolume(activePageIndex, new_float_value);
            }
            break;

        case FIELD_PUMP1_S: case FIELD_PUMP1_M: case FIELD_PUMP1_L:
        case FIELD_PUMP2_S: case FIELD_PUMP2_M: case FIELD_PUMP2_L:
        case FIELD_PUMP3_S: case FIELD_PUMP3_M: case FIELD_PUMP3_L:
            if (is_valid_float) {
                model->updateSinglePumpVolume(activePageIndex, activeSetupIndex, activeFieldIndex, new_float_value);
            }
            break;

        default:
            break;
    }

    // After updating, reload the data for the current page to show the change.
    loadScreenData(activePageIndex);
}

void ChemicalsSetupPresenter::addPumpSetup(int page_index)
{
    model->addPumpToRecipe(page_index);
    loadScreenData(page_index);
}

void ChemicalsSetupPresenter::removePumpSetup(int page_index)
{
    model->removePumpFromRecipe(page_index);
    loadScreenData(page_index);
}

void ChemicalsSetupPresenter::chemicalEnableToggled()
{
    // This function correctly uses the reliable activePageIndex
    const ChemicalRecipe_t& recipe = model->getRecipeData(activePageIndex);
    bool new_state_is_on = !(recipe.is_enabled == 1);
    model->setChemicalEnableState(activePageIndex, new_state_is_on);
    view.invalidateEnableButton();
}
