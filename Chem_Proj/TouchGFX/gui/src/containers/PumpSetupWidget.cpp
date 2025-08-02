#include <gui/containers/PumpSetupWidget.hpp>

PumpSetupWidget::PumpSetupWidget() :
    volumeEditClickedCallback(nullptr),
    saveDataCallback(nullptr),
    // Initialize our internal callback objects
    selectPumpCallback(this, &PumpSetupWidget::selectPumpButtonClickHandler),
    dropdownCallback(this, &PumpSetupWidget::dropdownPumpSelectedHandler),
    editVolumeCallback(this, &PumpSetupWidget::editVolumeButtonHandler)
{
    // Connect internal widgets to their handlers
    SelectPumpButton.setAction(selectPumpCallback);
    pumpDropDownList1.setOptionSelectedCallback(dropdownCallback);

    // --- Connect all three edit buttons to the SAME handler ---
    SEditButton.setAction(editVolumeCallback);
    MEditButton.setAction(editVolumeCallback);
    LEditButton.setAction(editVolumeCallback);
}

// setup() function remains the same...
void PumpSetupWidget::setup(int setup_index, const PumpSetup_t& data)
{
    // ... (This code was correct)
}

// setAvailablePumps() remains the same...
void PumpSetupWidget::setAvailablePumps(const std::vector<int>& enabled_pumps)
{
    // ... (This code was correct)
}

// selectPumpButtonClickHandler() remains the same...
void PumpSetupWidget::selectPumpButtonClickHandler(const touchgfx::AbstractButton& src)
{
    // ... (This code was correct)
}

// dropdownPumpSelectedHandler() is now simplified
void PumpSetupWidget::dropdownPumpSelectedHandler(int pump_index)
{
    pumpDropDownList1.setVisible(false);
    pumpDropDownList1.invalidate();
    SelectPumpButton.forceState(false);
    SelectPumpButton.invalidate();

    currentData.pump_index = pump_index;

    // --- Update the UI text ---
    // IMPORTANT: Your Designer screenshot shows many text widgets.
    // You MUST make sure you are updating the correct one.
    // I am assuming the text for the select pump button is LTextEdit
    if (currentData.pump_index != -1) {
        Unicode::snprintf(LTextEditBuffer, LTEXTEDIT_SIZE, "Pump %d", currentData.pump_index + 1);
    } else {
        Unicode::strncpy(LTextEditBuffer, "Select Pump", LTEXTEDIT_SIZE);
    }
    LTextEdit.invalidate();

    // Signal to the main screen that data has changed
    if (saveDataCallback && saveDataCallback->isValid()) {
        saveDataCallback->execute(setupIndex, currentData);
    }
}

// --- NEW CONSOLIDATED HANDLER FOR S/M/L BUTTONS ---
void PumpSetupWidget::editVolumeButtonHandler(const touchgfx::AbstractButton& src)
{
    int field_index = -1;
    if (&src == &SEditButton) {
        field_index = 0; // 0 for Small
    } else if (&src == &MEditButton) {
        field_index = 1; // 1 for Medium
    } else if (&src == &LEditButton) {
        field_index = 2; // 2 for Large
    }

    if (field_index != -1 && volumeEditClickedCallback && volumeEditClickedCallback->isValid()) {
        // Signal which field needs editing
        volumeEditClickedCallback->execute(setupIndex, field_index);
    }
}
