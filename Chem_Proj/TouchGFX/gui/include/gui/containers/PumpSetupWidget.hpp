#ifndef PUMPSETUPWIDGET_HPP
#define PUMPSETUPWIDGET_HPP

#include <gui_generated/containers/PumpSetupWidgetBase.hpp>
#include <gui/containers/PumpDropdownList.hpp> // We need to know about our child container
#include <vector>
#include "shared_types.h"
#include <touchgfx/widgets/AbstractButton.hpp> // Needed for button callback signatures

class PumpSetupWidget : public PumpSetupWidgetBase
{
public:
    PumpSetupWidget();
    virtual ~PumpSetupWidget() {}

    // --- PUBLIC INTERFACE ---
    // These functions are called by the parent screen (ChemicalsSetupView)

    void setup(int setup_index, const PumpSetup_t& data, int8_t unit);
    void setAvailablePumps(const std::vector<int>& enabled_pumps);

    // --- PUBLIC CALLBACKS ---
    // These are the "phone jacks" that the parent screen plugs into to listen to us.

    // Signals that the user wants to edit one of the volume fields (S, M, or L)
    touchgfx::GenericCallback<int, int>* volumeEditClickedCallback;

    // Signals that our internal data has changed and needs to be saved
    touchgfx::GenericCallback<int, const PumpSetup_t&>* saveDataCallback;

protected:
    // --- INTERNAL EVENT HANDLERS ---
    // These functions are called by our own child widgets.

    // Called when the main "SelectPumpButton" toggle button is clicked
    void selectPumpButtonClickHandler(const touchgfx::AbstractButton& src);

    // Called when a pump is chosen from our internal dropdown list
    void dropdownPumpSelectedHandler(int pump_index);

    // A single, consolidated handler for all three S/M/L edit buttons
    void editVolumeButtonHandler(const touchgfx::AbstractButton& src);

private:
    // --- PRIVATE MEMBER VARIABLES ---

    int setupIndex; // Remembers if we are setup #0, #1, or #2 in the list of three
    std::vector<int> availablePumps; // Stores the list of pumps for the dropdown
    PumpSetup_t currentData; // A local copy of our own data

    // Permanent callback objects for connecting our internal widgets
    touchgfx::Callback<PumpSetupWidget, const touchgfx::AbstractButton&> selectPumpCallback;
    touchgfx::Callback<PumpSetupWidget, int> dropdownCallback;
    touchgfx::Callback<PumpSetupWidget, const touchgfx::AbstractButton&> editVolumeCallback;
};

#endif // PUMPSETUPWIDGET_HPP
