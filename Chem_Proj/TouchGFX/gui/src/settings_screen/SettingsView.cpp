#include <gui/settings_screen/SettingsView.hpp>
#include "device_config.h"
#include "main.h"
extern DeviceConfiguration_t myDeviceConfig;

SettingsView::SettingsView()
{

}

void SettingsView::setupScreen()
{
    SettingsViewBase::setupScreen();
}

void SettingsView::tearDownScreen()
{
    SettingsViewBase::tearDownScreen();
}

void SettingsView::Reset_Defaults()
{
	PlaySound(1);
	Config_SetDefaults(&myDeviceConfig);
	Config_Save(&myDeviceConfig);
	DefaultsContainer.setVisible(false);
	DefaultsContainer.invalidate();
	updateAllSettings();
}

void SettingsView::setUnitText(int8_t unit)
{
    // Clear the buffer first to prevent leftover characters
    Unicode::snprintf(VolumeUnitTextBuffer, VOLUMEUNITTEXT_SIZE, "");

    if (unit == 1) // 1 for Oz
    {
        Unicode::snprintf(VolumeUnitTextBuffer, VOLUMEUNITTEXT_SIZE, "Oz");
    }
    else // 0 for mL
    {
        Unicode::snprintf(VolumeUnitTextBuffer, VOLUMEUNITTEXT_SIZE, "mL");
    }

    // Invalidate the text area to force it to redraw with the new text
    VolumeUnit.invalidate();
}

void SettingsView::unitButtonClicked()
{
    // The View's only job is to report the click to the Presenter.
    presenter->unitButtonClicked();
}

void SettingsView::setSliderValue(int value)
{
    // Update the slider's visual state without triggering the callback again.
    BrightnessSlider.setValue(value);
}

void SettingsView::brightnessSliderChanged(int value)
{
    // The user moved the slider. The View's only job is to
    // report this new value to the Presenter.
	presenter->saveNewBrightnessValue(value);
}

void SettingsView::DefaultsSummon()
{
	DefaultsContainer.setVisible(true);
	DefaultsContainer.invalidate();
}

void SettingsView::DefaultsExit()
{
	DefaultsContainer.setVisible(false);
	DefaultsContainer.invalidate();
}

void SettingsView::updateAllSettings()
{
    // --- 1. Update the Unit Text ---
    // Ask the presenter for the new, fresh default value.
    int8_t currentUnit = presenter->getVolumeUnit(); // Assuming you have a getVolumeUnit in your presenter

    // Now call your existing setUnitText function to update the display.
    setUnitText(currentUnit);

    // --- 2. Update the Brightness Slider ---
    // Ask the presenter for the new, fresh default brightness.
    uint8_t brightness = presenter->getBrightness(); // Assuming you have a getBrightness in your presenter

    // Now call your existing setSliderValue function.
    setSliderValue(brightness);

    // --- 3. Add any other settings controls here ---
    // If you have other controls on this page that depend on the config,
    // you would add their update logic here as well.
}
