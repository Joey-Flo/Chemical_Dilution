// Filename: TouchGFX/gui/include/gui/containers/PumpOption.hpp

#ifndef PUMPOPTION_HPP
#define PUMPOPTION_HPP

#include <gui_generated/containers/PumpOptionBase.hpp> // Includes the Base class
#include <touchgfx/Callback.hpp>                       // Required for callbacks
#include <touchgfx/containers/buttons/AbstractButtonContainer.hpp>

// The PumpOption class inherits all the widgets from PumpOptionBase
class PumpOption : public PumpOptionBase
{
public:
    PumpOption();
    virtual ~PumpOption() {}

    // --- ADD YOUR PUBLIC FUNCTIONS HERE ---

    // This function will be called to configure the row with a pump's info.
    void setup(int pump_index, const char* pump_name);

    // This function sets up the "phone number" to call when this option is clicked.
    void setClickCallback(touchgfx::GenericCallback<int>& callback);

protected:
    // --- ADD YOUR INTERNAL HANDLER HERE ---

    // This is the internal function that gets called when our button is clicked.
    void buttonClickedCallback(const touchgfx::AbstractButtonContainer& button);

private:
    // --- ADD YOUR MEMBER VARIABLES HERE ---

    int pumpIndex; // A place to remember which pump this row represents.
    touchgfx::GenericCallback<int>* clickCallback; // A pointer to the callback function.

    touchgfx::Callback<PumpOption, const touchgfx::AbstractButtonContainer&> buttonCallback;
};

#endif // PUMPOPTION_HPP
