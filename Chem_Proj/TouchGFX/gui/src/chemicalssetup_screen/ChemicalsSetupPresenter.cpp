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
    // 1. Get the specific recipe data from the Model.
    //    These are the functions we just wrote in the Model.
    const ChemicalRecipe_t& recipe = model->getRecipeData(page_index);
    std::vector<int> enabled_pumps = model->getEnabledPumpIndices();
    int8_t unit = model->getVolumeUnit();

    // 2. Pass all three pieces of information to the View to be displayed.
    //    We will create the 'displayData' function in the View in the next step.
    view.displayData(recipe, enabled_pumps, unit);
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
