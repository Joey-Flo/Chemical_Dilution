#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <cstdio> // For snprintf
#include <cstring>

// --- MODIFIED: The constructor now initializes the callback object ---
ChemicalsSetupView::ChemicalsSetupView() :
    // Initialize our internal callback object, linking it to our new handler function.
    pumpSetupSaveCallback(this, &ChemicalsSetupView::pumpSetupSaveDataCallbackHandler),
    // We can also prepare the volume edit callback for later.
    pumpSetupVolumeEditCallback(this, &ChemicalsSetupView::pumpSetupVolumeEditCallbackHandler)
{
    // Initialize the helper array with pointers to your widgets.
    // VERIFY these names against your TouchGFX Designer project.
    pumpSetupWidgets[0] = &pumpSetupWidget1;
    pumpSetupWidgets[1] = &pumpSetupWidget2;
    pumpSetupWidgets[2] = &pumpSetupWidget3;

    // --- NEW: Connect the callbacks to the child widgets ---
    // This loop "plugs in" our handler to each widget's "saveDataCallback" jack.
    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        pumpSetupWidgets[i]->saveDataCallback = &pumpSetupSaveCallback;
        pumpSetupWidgets[i]->volumeEditClickedCallback = &pumpSetupVolumeEditCallback;
    }
}


void ChemicalsSetupView::setupScreen()
{
    ChemicalsSetupViewBase::setupScreen();

    // Store the current page index when the screen is entered.
    currentPageIndex = swipeContainer1.getSelectedPage();

    // Tell the Presenter to load all the data for this page.
    presenter->loadScreenData(currentPageIndex);
}


void ChemicalsSetupView::tearDownScreen()
{
    ChemicalsSetupViewBase::tearDownScreen();
}


void ChemicalsSetupView::displayData(const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit)
{
    // This function populates the UI and is mostly unchanged.

    // Update Chemical Name
    memset(NameEditTextBuffer, 0, NAMEEDITTEXT_SIZE * sizeof(touchgfx::Unicode::UnicodeChar));
    Unicode::strncpy(NameEditTextBuffer, data.name, NAMEEDITTEXT_SIZE);
    NameEditText.invalidate();

    // Update Total Volume and Units
    const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
    toggleButton1.forceState(unit == 1);
    toggleButton1.invalidate();

    char volBuffer[20];
    snprintf(volBuffer, 20, "%.2f %s", data.total_dispense_volume, unit_suffix);
    memset(VolumeEditTextBuffer, 0, VOLUMEEDITTEXT_SIZE * sizeof(touchgfx::Unicode::UnicodeChar));
    Unicode::strncpy(VolumeEditTextBuffer, volBuffer, VOLUMEEDITTEXT_SIZE);
    VolumeEditText.invalidate();

    // Configure the PumpSetupWidgets
    int visiblePumpSetups = 0;
    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        pumpSetupWidgets[i]->setAvailablePumps(enabled_pumps);
        pumpSetupWidgets[i]->setup(i, data.pump_setups[i], unit);

        if (data.pump_setups[i].pump_index != -1) {
            pumpSetupWidgets[i]->setVisible(true);
            visiblePumpSetups++;
        } else {
            pumpSetupWidgets[i]->setVisible(false);
        }
        pumpSetupWidgets[i]->invalidate();
    }

    // Manage Add/Remove button visibility
    AddPump.setVisible(visiblePumpSetups < MAX_PUMP_SETUPS_PER_CHEMICAL);
    RemovePump.setVisible(visiblePumpSetups > 0);
    AddPump.invalidate();
    RemovePump.invalidate();
}


/*
 * ======================================================
 *               NEW HANDLER FUNCTIONS
 * ======================================================
 */

// This handler is called by the swipe container whenever the user swipes to a new page.
void ChemicalsSetupView::swipeContainer1PageChangedCallback(int newPageIndex)
{
    // Update our stored page index and tell the presenter to load the new page's data.
    currentPageIndex = newPageIndex;
    presenter->loadScreenData(currentPageIndex);
}


// This handler is called by ANY of the three PumpSetupWidgets when their data has changed.
void ChemicalsSetupView::pumpSetupSaveDataCallbackHandler(int setupIndex, const PumpSetup_t& data)
{
    // The View's job is to simply pass the message on to the Presenter.
    // We already know the current page, so we pass that along too.
    presenter->savePumpSetupData(currentPageIndex, setupIndex, data);
}

// This handler is called by ANY of the three PumpSetupWidgets when an edit button is clicked.
void ChemicalsSetupView::pumpSetupVolumeEditCallbackHandler(int setupIndex, int fieldIndex)
{
    // TODO: Implement keyboard logic here.
    // For now, it's an empty placeholder.
    // Example: presenter->editPumpVolume(currentPageIndex, setupIndex, fieldIndex);
}


// --- Placeholder implementations for your button clicks ---
// You will wire these up in the TouchGFX Designer using "Execute C++ code".

void ChemicalsSetupView::addPumpClicked()
{
    // presenter->addPumpSetup(currentPageIndex);
}

void ChemicalsSetupView::removePumpClicked()
{
    // presenter->removePumpSetup(currentPageIndex);
}

void ChemicalsSetupView::nameEditClicked()
{
    // presenter->editChemicalName(currentPageIndex);
}

void ChemicalsSetupView::volumeEditClicked()
{
    // presenter->editTotalVolume(currentPageIndex);
}

void ChemicalsSetupView::unitToggleButtonClicked()
{
    // presenter->unitChanged(currentPageIndex);
}
