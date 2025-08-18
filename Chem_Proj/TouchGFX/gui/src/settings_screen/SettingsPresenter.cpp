#include <gui/settings_screen/SettingsView.hpp>
#include <gui/settings_screen/SettingsPresenter.hpp>

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
    model->toggleVolumeUnit();

    // 2. After the model is updated, tell the view to show the new value
    updateUnitText();
}
