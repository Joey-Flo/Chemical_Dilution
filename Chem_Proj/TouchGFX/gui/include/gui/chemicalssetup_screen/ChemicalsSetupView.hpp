#ifndef CHEMICALSSETUPVIEW_HPP
#define CHEMICALSSETUPVIEW_HPP

#include <gui_generated/chemicalssetup_screen/ChemicalsSetupViewBase.hpp>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>
#include "shared_types.h"
#include <vector>
#include <gui/common/CustomKeyboard.hpp>

// We need to include this to use the Callback template
#include <touchgfx/Callback.hpp>

enum FieldID {
    FIELD_NONE = 0,
    FIELD_CHEM_NAME,      // ID = 1
    FIELD_TOTAL_VOLUME,   // ID = 2

    // We can create a predictable block of IDs for the pump setups.
    // This formula leaves space for up to 10 fields per widget (S, M, L, etc.)
    FIELD_PUMP_SETUP_START = 10,
    FIELD_PUMP1_S = FIELD_PUMP_SETUP_START + (0 * 10) + 0, // ID = 10
    FIELD_PUMP1_M = FIELD_PUMP_SETUP_START + (0 * 10) + 1, // ID = 11
    FIELD_PUMP1_L = FIELD_PUMP_SETUP_START + (0 * 10) + 2, // ID = 12

    FIELD_PUMP2_S = FIELD_PUMP_SETUP_START + (1 * 10) + 0, // ID = 20
    FIELD_PUMP2_M = FIELD_PUMP_SETUP_START + (1 * 10) + 1, // ID = 21
    FIELD_PUMP2_L = FIELD_PUMP_SETUP_START + (1 * 10) + 2, // ID = 22

    FIELD_PUMP3_S = FIELD_PUMP_SETUP_START + (2 * 10) + 0, // ID = 30
    FIELD_PUMP3_M = FIELD_PUMP_SETUP_START + (2 * 10) + 1, // ID = 31
    FIELD_PUMP3_L = FIELD_PUMP_SETUP_START + (2 * 10) + 2  // ID = 32
};


class ChemicalsSetupView : public ChemicalsSetupViewBase
{
public:
    ChemicalsSetupView();
    virtual ~ChemicalsSetupView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // Public function for the Presenter to call to populate the UI
    void displayData(const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit);

    // --- Virtual Action Handlers for buttons on this screen ---
    // These will be connected in the Designer using "Execute C++ code"
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

protected:
    // --- CALLBACK HANDLERS ---
    // These are the functions that get called by our child widgets (SwipeContainer, PumpSetupWidgets)

    // Called when the user swipes to a new page
    void swipeContainer1PageChangedCallback(int newPageIndex);

    // Called by a PumpSetupWidget when its data needs to be saved
    void pumpSetupSaveDataCallbackHandler(int setupIndex, const PumpSetup_t& data);

    int currentlyEditingField;

    CustomKeyboard keyboard;



private:
    // --- MEMBER VARIABLES ---

    int currentPageIndex; // Stores the currently visible page (0-7)

    // An array of pointers to the PumpSetupWidgets for easy access
    PumpSetupWidget* pumpSetupWidgets[MAX_PUMP_SETUPS_PER_CHEMICAL];

    // --- Permanent Callback Objects ---
    // These are the "wires" that we plug into our child widgets.

    // Callback for the SwipeContainer's page change event
    touchgfx::Callback<ChemicalsSetupView, int> swipeCallback;

    // Callback for the 'save' signal from any of the PumpSetupWidgets
    touchgfx::Callback<ChemicalsSetupView, int, const PumpSetup_t&> pumpSetupSaveCallback;

    // Callback for the 'edit volume' signal from any of the PumpSetupWidgets
    touchgfx::Callback<ChemicalsSetupView, int, int> pumpSetupVolumeEditCallback;
};

#endif // CHEMICALSSETUPVIEW_HPP
