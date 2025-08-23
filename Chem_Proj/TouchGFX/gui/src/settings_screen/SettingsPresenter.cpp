#include <gui/settings_screen/SettingsView.hpp>
#include <gui/settings_screen/SettingsPresenter.hpp>
extern "C" {
#include "z_displ_ILI9XXX.h"
}

SettingsPresenter::SettingsPresenter(SettingsView& v)
    : view(v)
{

}


void SettingsPresenter::deactivate()
{

}

void SettingsPresenter::activate()
{
    // When the screen is entered, tell the view to update the unit text
    updateUnitText();
    setInitialBrightness();
}

void SettingsPresenter::updateUnitText()
{
    // 1. Get the current unit from the Model
    int8_t currentUnit = model->getVolumeUnit();

    // 2. Pass this state to the View to be displayed
    view.setUnitText(currentUnit);
}

void SettingsPresenter::unitButtonClicked()
{
    // 1. Command the Model to flip the setting
	PlaySound(1);
    model->toggleVolumeUnit();

    // 2. After the model is updated, tell the view to show the new value
    updateUnitText();
}

void SettingsPresenter::setInitialBrightness()
{
    // 'Q' (Query) gets the current backlight level (which should be 0-100).
    uint32_t currentLevel = Displ_BackLight('Q');
    view.setSliderValue(currentLevel);
}

void SettingsPresenter::saveNewBrightnessValue(int value)
{
    // 1. Command the hardware to change the brightness immediately.
    BKLIT_TIMER->BKLIT_CCR = value;

    // 2. Command the Model to update its data structure and save to flash.
    //    (We will create this function in the Model next).
    model->updateBrightness(value);
}
