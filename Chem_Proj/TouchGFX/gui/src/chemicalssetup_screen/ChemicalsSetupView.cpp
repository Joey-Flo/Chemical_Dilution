#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <cstdio>
#include <cstring>
#include <gui/common/FieldIDs.hpp>

ChemicalsSetupView::ChemicalsSetupView() :
    pumpSetupSaveCallback(this, &ChemicalsSetupView::pumpSetupSaveDataCallbackHandler),
    pumpSetupVolumeEditCallback(this, &ChemicalsSetupView::pumpSetupVolumeEditCallbackHandler),
    dropdownStateCallback(this, &ChemicalsSetupView::dropdownStateCallbackHandler),
    activeDropdownWidget(nullptr),
    currentlyEditingField(FIELD_NONE) // Initialize this member
{
    // Initialize the helper array with pointers to your widgets.
    pumpSetupWidgets[0] = &pumpSetupWidget1;
    pumpSetupWidgets[1] = &pumpSetupWidget2;
    pumpSetupWidgets[2] = &pumpSetupWidget3;

    // --- FIX: Use our new getter function to safely populate the array ---
    for (int i = 0; i < NUM_CHEMICAL_RECIPES; ++i)
    {
        pageContainers[i] = getScrollableContainerForPage(i);
    }

    // The rest of your constructor code remains the same.
    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        pumpSetupWidgets[i]->saveDataCallback = &pumpSetupSaveCallback;
        pumpSetupWidgets[i]->volumeEditClickedCallback = &pumpSetupVolumeEditCallback;
    }

    keyboard.setPosition(0, 115, 240, 180);
    add(keyboard);
    keyboard.setVisible(false);
}

// --- NEW: Implementation of the getter function ---
touchgfx::ScrollableContainer* ChemicalsSetupView::getScrollableContainerForPage(int pageIndex)
{
    // This function has access to the protected members from the base class.
    // It returns a pointer to the correct scrollable container based on the index.
    // The names (scrollableContainer1, etc.) must match your Designer widget names.
    switch (pageIndex)
    {
        case 0: return &scrollableContainer1;
//        case 1: return &scrollableContainer2;
//        case 2: return &scrollableContainer3;
//        case 3: return &scrollableContainer4;
//        case 4: return &scrollableContainer5;
//        case 5: return &scrollableContainer6;
//        case 6: return &scrollableContainer7;
//        case 7: return &scrollableContainer8;
        default: return nullptr; // Return null for safety
    }
}


// NO OTHER CHANGES ARE NEEDED BELOW THIS LINE
// The rest of your ChemicalsSetupView.cpp file remains the same.
// Just ensure the constructor and the new getter function above are correct.

void ChemicalsSetupView::setupScreen()
{
    ChemicalsSetupViewBase::setupScreen();

    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        // pumpSetupWidgets[i]->setPresenter(presenter); // This line is likely not needed
        pumpSetupWidgets[i]->dropdownStateChangedCallback = &dropdownStateCallback;
    }

    currentPageIndex = swipeContainer1.getSelectedPage();
    presenter->loadScreenData(currentPageIndex);
}

void ChemicalsSetupView::tearDownScreen()
{
    ChemicalsSetupViewBase::tearDownScreen();
}

void ChemicalsSetupView::displayData(const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit)
{
    Chemical1EnableButton.forceState(data.is_enabled == 1);
    Chemical1EnableButton.invalidate();

    memset(NameEditTextBuffer, 0, NAMEEDITTEXT_SIZE * sizeof(touchgfx::Unicode::UnicodeChar));
    Unicode::strncpy(NameEditTextBuffer, data.name, NAMEEDITTEXT_SIZE);
    NameEditText.invalidate();

    const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
    char volBuffer[20];
    snprintf(volBuffer, 20, "%.2f %s", data.total_dispense_volume, unit_suffix);
    memset(VolumeEditTextBuffer, 0, VOLUMEEDITTEXT_SIZE * sizeof(touchgfx::Unicode::UnicodeChar));
    Unicode::strncpy(VolumeEditTextBuffer, volBuffer, VOLUMEEDITTEXT_SIZE);
    VolumeEditText.invalidate();

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

    AddPump.setVisible(visiblePumpSetups < MAX_PUMP_SETUPS_PER_CHEMICAL);
    RemovePump.setVisible(visiblePumpSetups > 1);
    AddPump.invalidate();
    RemovePump.invalidate();
}

void ChemicalsSetupView::swipeContainer1PageChangedCallback(int newPageIndex)
{
    if (currentPageIndex >= 0 && currentPageIndex < NUM_CHEMICAL_RECIPES) {
        ScrollableContainer* oldPage = pageContainers[currentPageIndex];
        if (oldPage) oldPage->setTouchable(true);
    }

    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        pumpSetupWidgets[i]->closeDropdown();
    }

    currentPageIndex = newPageIndex;
    presenter->loadScreenData(currentPageIndex);
}

void ChemicalsSetupView::pumpSetupSaveDataCallbackHandler(int setupIndex, const PumpSetup_t& data)
{
    presenter->savePumpSetupData(currentPageIndex, setupIndex, data);
}

void ChemicalsSetupView::pumpSetupVolumeEditCallbackHandler(int setupIndex, int fieldIndex)
{
    int fieldID = FIELD_PUMP_SETUP_START + (setupIndex * 10) + fieldIndex;
    presenter->editField(fieldID);
}

void ChemicalsSetupView::dropdownStateCallbackHandler(bool isOpen)
{
    if (currentPageIndex >= 0 && currentPageIndex < NUM_CHEMICAL_RECIPES) {
        ScrollableContainer* currentPage = pageContainers[currentPageIndex];
        if (currentPage) {
            currentPage->setTouchable(!isOpen);
        }
    }
}

void ChemicalsSetupView::showKeyboard()
{
    keyboard.setVisible(true);
     KeyboardExtras;
     KeyboardExtras.setVisible(true);
    keyboard.invalidate();
     KeyboardExtras.invalidate();
}

void ChemicalsSetupView::EnterPressed()
{
    keyboard.setVisible(false);
    keyboard.invalidate();
     KeyboardExtras.setVisible(false);
     KeyboardExtras.invalidate();

    char utf8_buffer[20];
    memset(utf8_buffer, 0, 20);
    Unicode::toUTF8(keyboard.getBuffer(), (uint8_t*)utf8_buffer, 20);

    presenter->newValueEntered(utf8_buffer);

    keyboard.clearBuffer();
    currentlyEditingField = FIELD_NONE;
}

void ChemicalsSetupView::ExitPressed()
{
    keyboard.setVisible(false);
    keyboard.invalidate();
     KeyboardExtras.setVisible(false);
     KeyboardExtras.invalidate();
    keyboard.clearBuffer();
    currentlyEditingField = FIELD_NONE;
}

void ChemicalsSetupView::chemicalEnableButtonClicked()
{
    presenter->chemicalEnableToggled();
}

//void ChemicalsSetupView::addPumpClicked()
//{
//    presenter->addPumpSetup(currentPageIndex);
//}
//
//void ChemicalsSetupView::removePumpClicked()
//{
//    presenter->removePumpSetup(currentPageIndex);
//}

void ChemicalsSetupView::nameEditClicked() { /* Not yet implemented */ }
void ChemicalsSetupView::volumeEditClicked() { /* Not yet implemented */ }
void ChemicalsSetupView::unitToggleButtonClicked() { /* Not yet implemented */ }
void ChemicalsSetupView::editChemicalNameClicked() { /* Not yet implemented */ }
void ChemicalsSetupView::editTotalVolumeClicked() { /* Not yet implemented */ }
