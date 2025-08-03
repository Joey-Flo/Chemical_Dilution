// Filename: TouchGFX/gui/src/gui/containers/PumpOption.cpp

#include <gui/containers/PumpOption.hpp>

PumpOption::PumpOption() :
    clickCallback(nullptr),
    // --- ADD THIS ---
    // Initialize our new member variable here. This creates the permanent object.
    buttonCallback(this, &PumpOption::buttonClickedCallback)
{
    // --- CHANGE THIS ---
    // Now, pass the named, permanent callback object to setAction.
    // This is like giving the function your real keys instead of a photocopy.
    OptionButton.setAction(buttonCallback);
}

void PumpOption::setup(int pump_index, const char* pump_name)
{
    // Remember which pump we are.
    pumpIndex = pump_index;

    // Set the text for the wildcard. The wildcard buffer is named "textArea1Buffer"
    // in the Base file you provided.
    Unicode::strncpy(textArea1Buffer, pump_name, TEXTAREA1_SIZE);

    // Invalidate the text area to force it to redraw with the new text.
    textArea1.invalidate();
}

void PumpOption::setClickCallback(touchgfx::GenericCallback<int>& callback)
{
    // Store the "phone number" so we can call it later.
    clickCallback = &callback;
}

void PumpOption::buttonClickedCallback(const touchgfx::AbstractButtonContainer& button)
{
    // The logic inside remains the same.
    if (clickCallback && clickCallback->isValid())
    {
        clickCallback->execute(pumpIndex);
    }
}
