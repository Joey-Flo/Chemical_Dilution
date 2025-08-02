#ifndef PUMPSETUPWIDGET_HPP
#define PUMPSETUPWIDGET_HPP

#include <gui_generated/containers/PumpSetupWidgetBase.hpp>
#include <gui/containers/PumpDropdownList.hpp>
#include "device_config.h"
#include <vector>
#include <touchgfx/widgets/AbstractButton.hpp> // Include for button callbacks

class PumpSetupWidget : public PumpSetupWidgetBase
{
public:
    PumpSetupWidget();
    virtual ~PumpSetupWidget() {}

    void setup(int setup_index, const PumpSetup_t& data);
    void setAvailablePumps(const std::vector<int>& enabled_pumps);

    // --- CORRECTED CALLBACK DECLARATIONS ---
    // Use GenericCallback for public callbacks. This is the "phone jack"
    // that the main screen will plug its own callback into.
    touchgfx::GenericCallback<int, int>* volumeEditClickedCallback;
    touchgfx::GenericCallback<int, const PumpSetup_t&>* saveDataCallback;

protected:
    // --- CONSOLIDATED AND CORRECTED HANDLERS ---
    void selectPumpButtonClickHandler(const touchgfx::AbstractButton& src);
    void dropdownPumpSelectedHandler(int pump_index);

    // One handler for all three S/M/L edit buttons
    void editVolumeButtonHandler(const touchgfx::AbstractButton& src);

private:
    int setupIndex;
    std::vector<int> availablePumps;
    PumpSetup_t currentData;

    // --- Permanent callback objects for INTERNAL connections ---
    touchgfx::Callback<PumpSetupWidget, const touchgfx::AbstractButton&> selectPumpCallback;
    touchgfx::Callback<PumpSetupWidget, int> dropdownCallback;
    touchgfx::Callback<PumpSetupWidget, const touchgfx::AbstractButton&> editVolumeCallback;
};

#endif // PUMPSETUPWIDGET_HPP
