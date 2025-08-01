#ifndef PUMPSETUPSCREENVIEW_HPP
#define PUMPSETUPSCREENVIEW_HPP

#include <gui_generated/pumpsetupscreen_screen/PumpSetupScreenViewBase.hpp>
#include <gui/pumpsetupscreen_screen/PumpSetupScreenPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>

class PumpSetupScreenView : public PumpSetupScreenViewBase
{
public:
    PumpSetupScreenView();
    virtual ~PumpSetupScreenView() {} // Virtual destructor is good practice

    // This function runs every time the screen is entered
    virtual void setupScreen();
    virtual void tearDownScreen();

    // --- CONSOLIDATED VIRTUAL ACTION HANDLERS ---
    // All 8 density text fields will call this single handler.
    // The 'index' parameter (1-8) will be set in the TouchGFX Designer interaction.

    // All 8 toggle buttons will call this single handler.
    // The 'index' parameter (1-8) will be set in the TouchGFX Designer interaction.

    // --- Keyboard Action Handlers ---
    virtual void EnterPressed();
    virtual void ExitPressed();
    void HandleKeyboard(int8_t index);

protected:
    // This is a private helper function, not meant to be called from outside.

    CustomKeyboard keyboard;
    int8_t KeyboardCallFlags[8];

private:
    // --- MEMBER VARIABLE ARRAYS FOR UI WIDGETS ---
    // By storing pointers to our widgets in arrays, we can easily manage them in loops.

    static const int NUM_PUMPS = 8;

    // Array to hold the 8 toggle buttons for enabling/disabling pumps
    touchgfx::ToggleButton* pumpToggleButtons[NUM_PUMPS];

    // Arrays to hold the 8 text areas (and their buffers) for pump density
    touchgfx::TextAreaWithOneWildcard* densityTextAreas[NUM_PUMPS];
    touchgfx::Unicode::UnicodeChar* densityBuffers[NUM_PUMPS];
    uint16_t densityBufferSizes[NUM_PUMPS];
};

#endif // PUMPSETUPSCREENVIEW_HPP
