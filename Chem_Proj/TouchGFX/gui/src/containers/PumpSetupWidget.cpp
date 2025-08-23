#include <gui/containers/PumpSetupWidget.hpp>
#include <cstdio>
#include <algorithm>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>
#include "main.h"

PumpSetupWidget::PumpSetupWidget() :
    presenter(nullptr),
    volumeEditClickedCallback(nullptr),
    saveDataCallback(nullptr),
    // --- ADD THE NEW CALLBACK POINTER TO THE INITIALIZER LIST ---
    dropdownStateChangedCallback(nullptr),
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
    // This is good practice to ensure the dropdown draws on top of everything else in this widget.
    remove(PumpDropDown);
    add(PumpDropDown);
}


void PumpSetupWidget::setup(int setup_index, const PumpSetup_t& data, int8_t unit)
{
    setupIndex = setup_index;
    currentData = data; // Make a local copy

    bool is_current_pump_valid = false;
    if (currentData.pump_index != -1) {
        for (int available_pump : availablePumps) {
            if (available_pump == currentData.pump_index) {
                is_current_pump_valid = true;
                break;
            }
        }
    }

    if (is_current_pump_valid) {
        Unicode::snprintf(PumpSelectTextBuffer, PUMPSELECTTEXT_SIZE, "Pump %d", currentData.pump_index + 1);
    } else {
        Unicode::strncpy(PumpSelectTextBuffer, "Select Pump", PUMPSELECTTEXT_SIZE);
    }
    PumpSelectText.invalidate();

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

    // Ensure dropdown is hidden initially when the widget is first set up
    closeDropdown(); // Use our new function to ensure state is consistent
}


void PumpSetupWidget::setAvailablePumps(const std::vector<int>& enabled_pumps)
{
    availablePumps = enabled_pumps;
}


// --- MODIFIED: This handler now signals the parent ---
void PumpSetupWidget::selectPumpButtonClickHandler(const touchgfx::AbstractButton& src)
{
    // Determine the new desired state of the dropdown.
    bool showDropdown = SelectPumpButton.getState();

    // If we want to show it, first check if there's anything TO show.
    if (showDropdown && availablePumps.empty()) {
        SelectPumpButton.forceState(false); // Can't open an empty list, so revert button state.
        showDropdown = false; // Update our desired state.
    }

    // Now, apply the final state.
    if (showDropdown) {
        PumpDropDown.buildList(availablePumps);
    }
    PumpDropDown.setVisible(showDropdown);
    PumpDropDown.invalidate();

    // --- TRIGGER THE CALLBACK ---
    // Inform the parent view about the change in the dropdown's visibility state.
    if (dropdownStateChangedCallback && dropdownStateChangedCallback->isValid())
    {
        dropdownStateChangedCallback->execute(showDropdown); // true if showing, false if hiding
    }
}


// --- MODIFIED: This handler also signals the parent ---
void PumpSetupWidget::dropdownPumpSelectedHandler(int pump_index)
{
    // Hide the dropdown and untoggle the button
    closeDropdown(); // Use the new function to hide and signal in one step.

    // Update our local data copy with the NEW selection
    currentData.pump_index = pump_index;

    // Update the button's text to show the new selection
    Unicode::snprintf(PumpSelectTextBuffer, PUMPSELECTTEXT_SIZE, "Pump %d", currentData.pump_index + 1);
    PumpSelectText.invalidate();
    PlaySound(1);
    // Signal to the main screen that our data has changed
    if (saveDataCallback && saveDataCallback->isValid()) {
        saveDataCallback->execute(setupIndex, currentData);
    }
}


// --- NEW: Implementation of the Public API function ---
void PumpSetupWidget::closeDropdown()
{
    // Check if the dropdown is currently visible before doing anything
    if (PumpDropDown.isVisible())
    {
        // Hide the UI elements
        PumpDropDown.setVisible(false);
        PumpDropDown.invalidate();
        SelectPumpButton.forceState(false);
        SelectPumpButton.invalidate();

        // Important: Also notify the parent that the dropdown has closed.
        if (dropdownStateChangedCallback && dropdownStateChangedCallback->isValid())
        {
            dropdownStateChangedCallback->execute(false);
        }
    }
}


// This handler remains unchanged
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

// These functions can now be removed as their logic is consolidated in editVolumeButtonHandler
/*
void PumpSetupWidget::s_edit_button_clicked() { ... }
void PumpSetupWidget::m_edit_button_clicked() { ... }
void PumpSetupWidget::l_edit_button_clicked() { ... }
*/

// This can also likely be removed, as the editVolumeButtonHandler provides a more direct mechanism
void PumpSetupWidget::setPresenter(ChemicalsSetupPresenter* p)
{
    presenter = p;
}
