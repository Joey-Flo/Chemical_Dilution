#include <gui/scalecalibration_screen/ScaleCalibrationView.hpp>
#include <cstring>
#include <cmath>
#include "main.h"

ScaleCalibrationView::ScaleCalibrationView()
{
    keyboard.setPosition(0, 115, 240, 180);
    add(keyboard);
    keyboard.setVisible(false);
}

void ScaleCalibrationView::setupScreen()
{
    ScaleCalibrationViewBase::setupScreen();
}

void ScaleCalibrationView::tearDownScreen()
{
    ScaleCalibrationViewBase::tearDownScreen();
}

void ScaleCalibrationView::handleTickEvent()
{
    // This function is called 60 times per second.
    static int tickCounter = 0;

    // --- NEW: A static counter for the auto-tare logic ---
    // 'static' means this variable will keep its value between function calls.
    static int nearZeroCount = 0;

    // Update the display 10 times per second (60 / 6 = 10)
    if (++tickCounter % 6 == 0)
    {
        // 1. Get the live weight from the presenter
        float weight = presenter->getLiveWeight();

        // 2. Implement the Auto-Tare logic
        if (weight >= -0.3f && weight <= 0.3f)
        {
            // The scale reading is very close to zero. Increment the counter.
            nearZeroCount++;
        }
        else
        {
            // The scale is reading a real weight, so reset the counter.
            nearZeroCount = 0;
        }

        // If the counter reaches our threshold (35 checks * 0.1s/check = 3.5 seconds),
        // then tare the scale.
        if (nearZeroCount >= 35)
        {
            presenter->tareScale(); // Tell the presenter to tare
            nearZeroCount = 0;      // Reset the counter
        }

        // 3. Implement the Zero-Snapping logic for display
        if (weight < 0.3f && weight > -0.3f)
        {
            weight = 0.0f;
        }

        // 4. Implement Rounding to the nearest half gram
        //    - Multiply by 2 (e.g., 5.6g -> 11.2)
        //    - Round to the nearest whole number (11.2 -> 11.0)
        //    - Divide by 2 (11.0 -> 5.5)
        float rounded_weight = roundf(weight * 2.0f) / 2.0f;

        // 5. Update the text buffer with the final, clean value
        Unicode::snprintfFloat(WeightTextBuffer, WEIGHTTEXT_SIZE, "%.1f", rounded_weight);
        WeightText.invalidate();
    }
}

// This is the virtual function called by your "EditScale" button interaction
void ScaleCalibrationView::ScaleCalibrate()
{
    // When the edit button is clicked, just show the keyboard.
    keyboard.setVisible(true);
    KeyboardExtras.setVisible(true);
    keyboard.invalidate();
    KeyboardExtras.invalidate();
}

// This is the virtual function for your "TareButton" interaction
void ScaleCalibrationView::TareScale()
{
    // The View commands the Presenter to perform the tare action.
    // The Presenter will then call the C-level hx711_tare function.
    // (You will need to add a simple "tareScale()" function to your presenter).
	PlaySound(1);
    presenter->tareScale();
}


void ScaleCalibrationView::EnterPressed()
{
    // 1. Hide the keyboard
    keyboard.setVisible(false);
    keyboard.invalidate();

    // 2. Get the text from the keyboard
    char utf8_buffer[20];
    memset(utf8_buffer, 0, 20);
    Unicode::toUTF8(keyboard.getBuffer(), (uint8_t*)utf8_buffer, 20);

    // 3. Pass the user's input to the Presenter to do the calculation
    presenter->calculateNewCalibrationFactor(utf8_buffer);

    // 4. Clean up
    keyboard.clearBuffer();

    KeyboardExtras.setVisible(false);
    KeyboardExtras.invalidate();
}

void ScaleCalibrationView::ExitPressed()
{
    // 1. Hide the keyboard
    keyboard.setVisible(false);
    keyboard.invalidate();

    KeyboardExtras.setVisible(false);
    KeyboardExtras.invalidate();

}

