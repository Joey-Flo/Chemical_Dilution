#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <cstdio> // For snprintf
#include <cstring>


// --- Use an enum to create a unique ID for every editable field ---



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

    keyboard.setPosition(0, 115, 240, 180);
    add(keyboard);
    keyboard.setVisible(false);
}


void ChemicalsSetupView::setupScreen()
{

    ChemicalsSetupViewBase::setupScreen();

    // --- THIS IS THE CRITICAL CONNECTION ---
    // Give each child widget a pointer to the presenter.
    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        pumpSetupWidgets[i]->setPresenter(presenter);
    }

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

    Chemical1EnableButton.forceState(data.is_enabled == 1); // Use the correct button name
    Chemical1EnableButton.invalidate();

    // Update Chemical Name
    memset(NameEditTextBuffer, 0, NAMEEDITTEXT_SIZE * sizeof(touchgfx::Unicode::UnicodeChar));
    Unicode::strncpy(NameEditTextBuffer, data.name, NAMEEDITTEXT_SIZE);
    NameEditText.invalidate();

    // Update Total Volume and Units
    const char* unit_suffix = (unit == 1) ? "Oz" : "mL";

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
    RemovePump.setVisible(visiblePumpSetups > 1);
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

void ChemicalsSetupView::pumpSetupVolumeEditCallbackHandler(int setupIndex, int fieldIndex)
{
    // Calculate the unique ID and pass it to the presenter.
    int fieldID = FIELD_PUMP_SETUP_START + (setupIndex * 10) + fieldIndex;
    presenter->editField(fieldID);
}


// The showKeyboard() function doesn't need to change at all.
void ChemicalsSetupView::showKeyboard()
{
    // Make the keyboard and its extra buttons visible
    keyboard.setVisible(true);
    KeyboardExtras.setVisible(true);

    // Invalidate them to force a redraw
    keyboard.invalidate();
    KeyboardExtras.invalidate();
}

void ChemicalsSetupView::EnterPressed()
{
    // 1. Hide the keyboard and its extra buttons.
    keyboard.setVisible(false);
    keyboard.invalidate();
    KeyboardExtras.setVisible(false);
    KeyboardExtras.invalidate();

    // 2. Convert the keyboard's 16-bit Unicode buffer to a standard 8-bit C-string.
    char utf8_buffer[20];
    memset(utf8_buffer, 0, 20); // Clear the buffer to be safe
    Unicode::toUTF8(keyboard.getBuffer(), (uint8_t*)utf8_buffer, 20);

    // 3. The View's only job is to report the event to the Presenter.
    //    It passes the unique ID of the field that was being edited and the new text.
    presenter->newValueEntered(utf8_buffer);

    // 4. Clean up the state.
    keyboard.clearBuffer();
    currentlyEditingField = FIELD_NONE; // Reset the "what am I editing?" state
}


// --- VIRTUAL HANDLER FOR "EXIT" BUTTON CALLBACK ---
void ChemicalsSetupView::ExitPressed()
{
    // If the user presses exit, we just hide the keyboard and discard any input.

    // 1. Hide the keyboard and its extra buttons.
    keyboard.setVisible(false);
    keyboard.invalidate();
    KeyboardExtras.setVisible(false);
    KeyboardExtras.invalidate();

    // 2. Clean up the state.
    keyboard.clearBuffer();
    currentlyEditingField = FIELD_NONE; // Reset the "what am I editing?" state
}

void ChemicalsSetupView::chemicalEnableButtonClicked()
{
    // The View's only job is to report the event to the Presenter.
    presenter->chemicalEnableToggled();
}

//// This handler is called by ANY of the three PumpSetupWidgets when an edit button is clicked.
//void ChemicalsSetupView::pumpSetupVolumeEditCallbackHandler(int setupIndex, int fieldIndex)
//{
//    // TODO: Implement keyboard logic here.
//    // For now, it's an empty placeholder.
//    // Example: presenter->editPumpVolume(currentPageIndex, setupIndex, fieldIndex);
//}


// --- Placeholder implementations for your button clicks ---
// You will wire these up in the TouchGFX Designer using "Execute C++ code".


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
