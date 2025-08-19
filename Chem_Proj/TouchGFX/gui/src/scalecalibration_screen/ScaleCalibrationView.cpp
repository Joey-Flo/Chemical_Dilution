#include <gui/scalecalibration_screen/ScaleCalibrationView.hpp>
#include <cstring>

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
    // To avoid updating the screen too fast, we'll use a simple counter.
    static int tickCounter = 0;
    if (++tickCounter % 6 == 0) // Update 10 times per second (60 / 6 = 10)
    {
        float weight = presenter->getLiveWeight();
        Unicode::snprintfFloat(WeightTextBuffer, WEIGHTTEXT_SIZE, "%.1f", weight);
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

