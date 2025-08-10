#ifndef CHEMICALSSETUPVIEW_HPP
#define CHEMICALSSETUPVIEW_HPP

#include <gui_generated/chemicalssetup_screen/ChemicalsSetupViewBase.hpp>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>
#include "shared_types.h"
#include <vector>
#include <gui/common/CustomKeyboard.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/containers/ScrollableContainer.hpp>
#include <gui/common/FieldIDs.hpp>

class ChemicalsSetupView : public ChemicalsSetupViewBase
{
public:
    ChemicalsSetupView();
    virtual ~ChemicalsSetupView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void displayData(const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit);

    void addPumpClicked();
    void removePumpClicked();
    void nameEditClicked();
    void volumeEditClicked();
    void unitToggleButtonClicked();

    void editChemicalNameClicked();
    void editTotalVolumeClicked();
    void pumpSetupVolumeEditCallbackHandler(int setupIndex, int fieldIndex);
    void showKeyboard();
    void EnterPressed();
    void ExitPressed();
    void chemicalEnableButtonClicked();
    void LoadPageData(uint8_t Page, const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit);

protected:
    void swipeContainer1PageChangedCallback(int newPageIndex);
    void pumpSetupSaveDataCallbackHandler(int setupIndex, const PumpSetup_t& data);

    // --- NEW CALLBACK HANDLER ---
    void dropdownStateCallbackHandler(bool isOpen);

    int currentlyEditingField;
    CustomKeyboard keyboard;

private:
    // --- CORRECTED ORDER TO MATCH INITIALIZER LIST AND PREVENT WARNINGS ---
    touchgfx::Callback<ChemicalsSetupView, int, const PumpSetup_t&> pumpSetupSaveCallback;
    touchgfx::Callback<ChemicalsSetupView, int, int> pumpSetupVolumeEditCallback;
    touchgfx::Callback<ChemicalsSetupView, bool> dropdownStateCallback;
    touchgfx::ScrollableContainer* getScrollableContainerForPage(int pageIndex);
    int currentPageIndex;
    PumpSetupWidget* pumpSetupWidgets[MAX_PUMP_SETUPS_PER_CHEMICAL];

    touchgfx::ScrollableContainer* pageContainers[NUM_CHEMICAL_RECIPES];
    PumpSetupWidget* activeDropdownWidget;
};

#endif // CHEMICALSSETUPVIEW_HPP
