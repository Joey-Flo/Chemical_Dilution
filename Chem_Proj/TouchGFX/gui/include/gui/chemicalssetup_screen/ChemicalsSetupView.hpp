#ifndef CHEMICALSSETUPVIEW_HPP
#define CHEMICALSSETUPVIEW_HPP

#include <gui_generated/chemicalssetup_screen/ChemicalsSetupViewBase.hpp>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>
#include "shared_types.h"
#include <vector>

// We need to include this to use the Callback template
#include <touchgfx/Callback.hpp>


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

protected:
    // --- CALLBACK HANDLERS ---
    // These are the functions that get called by our child widgets (SwipeContainer, PumpSetupWidgets)

    // Called when the user swipes to a new page
    void swipeContainer1PageChangedCallback(int newPageIndex);

    // Called by a PumpSetupWidget when its data needs to be saved
    void pumpSetupSaveDataCallbackHandler(int setupIndex, const PumpSetup_t& data);

    // Called by a PumpSetupWidget when one of its volume edit buttons is clicked
    void pumpSetupVolumeEditCallbackHandler(int setupIndex, int fieldIndex);

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
