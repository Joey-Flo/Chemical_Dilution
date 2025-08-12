#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>
#include <cstdlib>

ChemicalsSetupPresenter::ChemicalsSetupPresenter(ChemicalsSetupView& v)
    : view(v)
{

}

void ChemicalsSetupPresenter::activate()
{

}

void ChemicalsSetupPresenter::deactivate()
{

}

void ChemicalsSetupPresenter::loadScreenData(int page_index)
{
    // *** CRITICAL FIX: The Presenter now remembers the active page. ***
    activePageIndex = page_index;

    const ChemicalRecipe_t& recipe = model->getRecipeData(activePageIndex);
    std::vector<int> enabled_pumps = model->getEnabledPumpIndices();
    int8_t unit = model->getVolumeUnit();

    view.displayData(recipe, enabled_pumps, unit);
}

const ChemicalRecipe_t& ChemicalsSetupPresenter::getRecipeDataForPage(int page_index) const
{
    // The presenter gets the data from its model and passes it back to the view.
    return model->getRecipeData(page_index);
}

void ChemicalsSetupPresenter::savePumpSetupData(int page_index, int setup_index, const PumpSetup_t& data)
{
    // The Presenter's job is simple: just pass the command to the Model.
    model->updatePumpSetup(page_index, setup_index, data);
}

void ChemicalsSetupPresenter::editField(int fieldID)
{
    // 1. Remember which field the user wants to edit.
    currentlyEditingFieldID = fieldID;

    // 2. Extract the context if it's a pump setup field.
    if (fieldID >= FIELD_PUMP_SETUP_START) {
        int relative_id = fieldID - FIELD_PUMP_SETUP_START;
        activeSetupIndex = relative_id / 10;
        activeFieldIndex = relative_id % 10;
    }

    // 3. Command the view to show the keyboard.
    view.showKeyboard();
}

void ChemicalsSetupPresenter::ActiveFieldIndexUpdate(uint8_t index){
	activePageIndex = index;
}

void ChemicalsSetupPresenter::newValueEntered(const char* text)
{
    // --- THIS IS THE CORRECTED IMPLEMENTATION ---

    // 1. Parse the text from the keyboard.
    char* end_ptr;
    float new_float_value = strtof(text, &end_ptr);
    bool is_valid_float = (*end_ptr == '\0' && end_ptr != text);

    // 2. Use a switch on the stored state to decide what to do.
    switch (currentlyEditingFieldID)
    {
        case FIELD_CHEM_NAME:
            // The name is a string, so we don't need to validate it as a float.
            model->updateChemicalName(activePageIndex, text);
            break;

        case FIELD_TOTAL_VOLUME:
            // This field requires a valid float.
            if (is_valid_float) {
                model->updateTotalVolume(activePageIndex, new_float_value);
            }
            break;

        case FIELD_PUMP1_S: // These all fall through to the same logic block
        case FIELD_PUMP1_M:
        case FIELD_PUMP1_L:
        case FIELD_PUMP2_S:
        case FIELD_PUMP2_M:
        case FIELD_PUMP2_L:
        case FIELD_PUMP3_S:
        case FIELD_PUMP3_M:
        case FIELD_PUMP3_L:
            // These fields also require a valid float.
            if (is_valid_float) {
                model->updateSinglePumpVolume(activePageIndex, activeSetupIndex, activeFieldIndex, new_float_value);
            }
            break;

        default:
            // If currentlyEditingFieldID was NONE or something else, do nothing.
            break;
    }

    // 3. Reset the editing state.
    currentlyEditingFieldID = FIELD_NONE;

    // 4. Reload the screen to show the final, saved value (or the old value if input was invalid).
    loadScreenData(activePageIndex);
}

void ChemicalsSetupPresenter::editPumpVolume(int setupIndex, int fieldIndex)
{
    // The View or a child widget has told us the user wants to edit a field.

    // 1. Store the context of which specific field is being edited.
    //    We will need this later when the user presses "Enter".
    activeSetupIndex = setupIndex;
    activeFieldIndex = fieldIndex;

    // We also need to set a general state flag so our 'newValueEntered'
    // function knows what kind of data to expect.
    currentlyEditing = FIELD_PUMP_VOLUME; // 'FIELD_PUMP_VOLUME' is from your enum

    // 2. Command the View to perform the UI action (show the keyboard).
    view.showKeyboard();
}

void ChemicalsSetupPresenter::addPumpSetup(int page_index)
{
    // 1. Command the model to perform the action
    model->addPumpToRecipe(page_index);

    // 2. Command the View to reload all its data to show the change
    loadScreenData(page_index);
}

void ChemicalsSetupPresenter::removePumpSetup(int page_index)
{
    // 1. Command the model to perform the action
    model->removePumpFromRecipe(page_index);

    // 2. Command the View to reload all its data to show the change
    loadScreenData(page_index);
}

void ChemicalsSetupView::addPumpClicked()
{
    // The View's only job is to report the event to the Presenter,
    // providing the necessary context (the current page).
    presenter->addPumpSetup(currentPageIndex);
}

void ChemicalsSetupView::removePumpClicked()
{
    presenter->removePumpSetup(currentPageIndex);
}

void ChemicalsSetupPresenter::chemicalEnableToggled(uint8_t PageIndex)
{
    // 1. Get the current state from the Model
    const ChemicalRecipe_t& recipe = model->getRecipeData(PageIndex - 1);
    bool current_state_is_on = (recipe.is_enabled == 1);

    // 2. Flip the state
    bool new_state_is_on = !current_state_is_on;

    // 3. Command the Model to update the data and save
    model->setChemicalEnableState(PageIndex - 1, new_state_is_on);

    // 4. No need to reload the whole screen, the button handles its own visual state.
    // If other elements needed to change (e.g., grey out), you would call loadScreenData() here.
}
