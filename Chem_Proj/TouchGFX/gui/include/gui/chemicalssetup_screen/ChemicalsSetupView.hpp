#ifndef CHEMICALSSETUPVIEW_HPP
#define CHEMICALSSETUPVIEW_HPP

#include <gui_generated/chemicalssetup_screen/ChemicalsSetupViewBase.hpp>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>
#include "shared_types.h"
#include <vector>
#include <gui/common/CustomKeyboard.hpp>
#include <touchgfx/Callback.hpp>
#include <gui/common/FieldIDs.hpp>

// Forward declare event types
namespace touchgfx { class ClickEvent; }

class ChemicalsSetupView : public ChemicalsSetupViewBase
{
public:
    ChemicalsSetupView();
    virtual ~ChemicalsSetupView() {}

    /**
     * @brief Called once when the screen is entered.
     */
    virtual void setupScreen();

    /**
     * @brief Called by the Presenter to update the screen's visual content.
     * @param data The recipe data for the currently active page.
     * @param enabled_pumps A list of globally enabled pump indices.
     * @param unit The current volume unit (0=mL, 1=Oz).
     */

    /**
     * @brief Commands the View to show the on-screen keyboard.
     */
    void showKeyboard();

    /**
     * @brief Called by the Presenter during activation to trigger the initial data load.
     */
    void requestDataLoad();

    // --- Virtual handlers called by Designer Interactions ---
    void addPumpClicked();
    void removePumpClicked();
    void nameEditClicked();
    void volumeEditClicked();
    void chemicalEnableButtonClicked();
    void EnterPressed();
    void ExitPressed();
    void invalidateEnableButton();
    void updateVisiblePages();

    /**
     * @brief Overridden event handler to detect the end of a swipe gesture.
     */
    virtual void handleClickEvent(const touchgfx::ClickEvent& event);

protected:
    // --- Callback handlers for child widgets ---
    void pumpSetupSaveDataCallbackHandler(int setupIndex, const PumpSetup_t& data);
    void pumpSetupVolumeEditCallbackHandler(int setupIndex, int fieldIndex);
    void dropdownStateCallbackHandler(bool isOpen);

private:

    void populateWidgetBuffer(int pageIndex,
                              touchgfx::Container& bufferContainer,
                              touchgfx::ToggleButton& enableButton,
                              touchgfx::TextAreaWithOneWildcard& nameField,
                              touchgfx::Unicode::UnicodeChar* nameBuffer, uint16_t nameBufferSize,
                              touchgfx::TextAreaWithOneWildcard& volumeField,
                              touchgfx::Unicode::UnicodeChar* volumeBuffer, uint16_t volumeBufferSize,
                              touchgfx::Button& addButton, touchgfx::Button& removeButton,
                              PumpSetupWidget** pumpWidgets);
    /**
     * @brief Helper function to get a pointer to the correct Page container.
     */
    touchgfx::Container* getPageContainerForIndex(int index);

    /**
     * @brief Helper function to get a pointer to the correct Scrollable Container.
     */
    touchgfx::ScrollableContainer* getScrollableContainerForPage(int pageIndex);

    // Callback objects for connecting to child widgets
    touchgfx::Callback<ChemicalsSetupView, int, const PumpSetup_t&> pumpSetupSaveCallback;
    touchgfx::Callback<ChemicalsSetupView, int, int> pumpSetupVolumeEditCallback;
    touchgfx::Callback<ChemicalsSetupView, bool> dropdownStateCallback;

    // State variables and widget pointers
    int currentPageIndex;
    PumpSetupWidget* pumpSetupWidgets[MAX_PUMP_SETUPS_PER_CHEMICAL];
    CustomKeyboard keyboard;
};

#endif // CHEMICALSSETUPVIEW_HPP
