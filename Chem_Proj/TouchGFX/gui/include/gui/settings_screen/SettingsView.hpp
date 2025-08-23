#ifndef SETTINGSVIEW_HPP
#define SETTINGSVIEW_HPP

#include <gui_generated/settings_screen/SettingsViewBase.hpp>
#include <gui/settings_screen/SettingsPresenter.hpp>

class SettingsView : public SettingsViewBase
{
public:
    SettingsView();
    virtual ~SettingsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void Reset_Defaults();
    void setUnitText(int8_t unit);
    void unitButtonClicked();
    void setSliderValue(int value);
    void brightnessSliderChanged(int value);
    void DefaultsSummon();
    void DefaultsExit();
    void updateAllSettings();
protected:
};

#endif // SETTINGSVIEW_HPP
