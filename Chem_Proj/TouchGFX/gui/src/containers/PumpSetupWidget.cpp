#include <gui/containers/PumpSetupWidget.hpp>
#include <cstdio>   // For snprintf
#include <algorithm> // For std::find
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>

PumpSetupWidget::PumpSetupWidget() :
	presenter(nullptr),
    volumeEditClickedCallback(nullptr),
    saveDataCallback(nullptr),
    selectPumpCallback(this, &PumpSetupWidget::selectPumpButtonClickHandler),
    dropdownCallback(this, &PumpSetupWidget::dropdownPumpSelectedHandler),
    editVolumeCallback(this, &PumpSetupWidget::editVolumeButtonHandler)
{
    // Connect Internal Widgets to their Handlers
    SelectPumpButton.setAction(selectPumpCallback);
    PumpDropDown.setOptionSelectedCallback(dropdownCallback);
    SEditButton.setAction(editVolumeCallback);
    MEditButton.setAction(editVolumeCallback);
    LEditButton.setAction(editVolumeCallback);

    // Z-Order Fix to prevent the dropdown from blocking clicks
    remove(PumpDropDown);
    add(PumpDropDown);
}


void PumpSetupWidget::setup(int setup_index, const PumpSetup_t& data, int8_t unit)
{
    setupIndex = setup_index;
    currentData = data; // Make a local copy

    // --- REFINED LOGIC: Check if the saved pump is still valid ---
    bool is_current_pump_valid = false;
    if (currentData.pump_index != -1) {
        for (int available_pump : availablePumps) {
            if (available_pump == currentData.pump_index) {
                is_current_pump_valid = true;
                break;
            }
        }
    }

    // This block is now for UI display only. It does NOT trigger a save.
    if (is_current_pump_valid) {
        // The saved pump is valid, so we display it.
        Unicode::snprintf(PumpSelectTextBuffer, PUMPSELECTTEXT_SIZE, "Pump %d", currentData.pump_index + 1);
    } else {
        // The saved pump is not valid OR no pump was ever selected.
        // Display a prompt for the user to make a choice.
        Unicode::strncpy(PumpSelectTextBuffer, "Select Pump", PUMPSELECTTEXT_SIZE);
    }
    PumpSelectText.invalidate();


    // Update the three volume text fields with units
    const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
    char volume_buffer[20];

    snprintf(volume_buffer, 20, "%.1f %s", currentData.dispense_small, unit_suffix);
    Unicode::strncpy(STextEditBuffer, volume_buffer, STEXTEDIT_SIZE);
    STextEdit.invalidate();

    snprintf(volume_buffer, 20, "%.1f %s", currentData.dispense_medium, unit_suffix);
    Unicode::strncpy(MTextEditBuffer, volume_buffer, MTEXTEDIT_SIZE);
    MTextEdit.invalidate();

    snprintf(volume_buffer, 20, "%.1f %s", currentData.dispense_large, unit_suffix);
    Unicode::strncpy(LTextEditBuffer, volume_buffer, LTEXTEDIT_SIZE);
    LTextEdit.invalidate();

    // Ensure dropdown is hidden initially
    PumpDropDown.setVisible(false);
    SelectPumpButton.forceState(false);
    SelectPumpButton.invalidate();
}


void PumpSetupWidget::setAvailablePumps(const std::vector<int>& enabled_pumps)
{
    availablePumps = enabled_pumps;
}


void PumpSetupWidget::selectPumpButtonClickHandler(const touchgfx::AbstractButton& src)
{
    bool showDropdown = SelectPumpButton.getState();
    if (showDropdown) {
        if (!availablePumps.empty()) {
            PumpDropDown.buildList(availablePumps);
            PumpDropDown.setVisible(true);
        } else {
            SelectPumpButton.forceState(false);
        }
    } else {
        PumpDropDown.setVisible(false);
    }
    PumpDropDown.invalidate();
}


// --- THIS IS WHERE THE SAVE IS TRIGGERED ---
void PumpSetupWidget::dropdownPumpSelectedHandler(int pump_index)
{
    // Hide the dropdown and untoggle the button
    PumpDropDown.setVisible(false);
    PumpDropDown.invalidate();
    SelectPumpButton.forceState(false);
    SelectPumpButton.invalidate();

    // --- Update our local data copy with the NEW selection ---
    currentData.pump_index = pump_index;

    // Update the button's text to show the new selection
    Unicode::snprintf(PumpSelectTextBuffer, PUMPSELECTTEXT_SIZE, "Pump %d", currentData.pump_index + 1);
    PumpSelectText.invalidate();

    // --- NOW, signal to the main screen that our data has changed ---
    // The main screen's Presenter/Model will handle the actual save to flash.
    if (saveDataCallback && saveDataCallback->isValid()) {
        // We pass our index (0, 1, or 2) and our entire, updated local data.
        saveDataCallback->execute(setupIndex, currentData);
    }
}


void PumpSetupWidget::editVolumeButtonHandler(const touchgfx::AbstractButton& src)
{
    int field_index = -1;
    if (&src == &SEditButton) { field_index = 0; }
    else if (&src == &MEditButton) { field_index = 1; }
    else if (&src == &LEditButton) { field_index = 2; }

    if (field_index != -1 && volumeEditClickedCallback && volumeEditClickedCallback->isValid()) {
        volumeEditClickedCallback->execute(setupIndex, field_index);
    }
}

void PumpSetupWidget::setPresenter(ChemicalsSetupPresenter* p)
{
    presenter = p;
}

// ... your existing setup() and other functions ...


// --- NEW: Implementation of the Public API functions ---

void PumpSetupWidget::s_edit_button_clicked()
{
    // Check if the presenter exists, then call its function
    if (presenter)
    {
        // Pass our own index (setupIndex) and the field index for 'S' (0)
        presenter->editPumpVolume(setupIndex, 0);
    }
}

void PumpSetupWidget::m_edit_button_clicked()
{
    if (presenter)
    {
        presenter->editPumpVolume(setupIndex, 1); // 1 for Medium
    }
}

void PumpSetupWidget::l_edit_button_clicked()
{
    if (presenter)
    {
        presenter->editPumpVolume(setupIndex, 2); // 2 for Large
    }
}
