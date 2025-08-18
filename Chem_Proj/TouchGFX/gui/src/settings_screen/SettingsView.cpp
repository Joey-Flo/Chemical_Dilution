#include <gui/settings_screen/SettingsView.hpp>
#include "device_config.h"
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
	Config_SetDefaults(&myDeviceConfig);
	Config_Save(&myDeviceConfig);
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
