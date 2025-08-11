#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <cstdio>
#include <cstring>
#include <gui/common/FieldIDs.hpp>

ChemicalsSetupView::ChemicalsSetupView() :
	dragStartX(0), isDragging(false),
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

void ChemicalsSetupView::displayData(const std::vector<int>& enabled_pumps, int8_t unit)
{
    // This function is now just a dispatcher.
    // It loops through all 8 recipes and calls LoadPageData for each one.
    for (int i = 0; i < NUM_CHEMICAL_RECIPES; i++)
    {
        // Get the specific data for the recipe we are loading
        const ChemicalRecipe_t& recipeDataForPage = presenter->getRecipeDataForPage(i);

        // Call our helper function to update the UI for that page
        LoadPageData(i, recipeDataForPage, enabled_pumps, unit);
    }
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

void ChemicalsSetupView::LoadPageData(uint8_t Page, const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit) {
    // A temporary buffer for the volume string
    char volBuffer[20];
    const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
    snprintf(volBuffer, 20, "%.2f %s", data.total_dispense_volume, unit_suffix);

    // This switch updates the correct widgets based on the Page number
    switch (Page)
    {
        case 0: // Page 1
            Chemical1EnableButton.forceState(data.is_enabled == 1);
            Unicode::strncpy(NameEditText1Buffer, data.name, NAMEEDITTEXT1_SIZE);
            Unicode::strncpy(VolumeEditText1Buffer, volBuffer, VOLUMEEDITTEXT1_SIZE);
            NameEditText1.invalidate();
            VolumeEditText1.invalidate();
            Chemical1EnableButton.invalidate();
            break;

        case 1: // Page 2
            Chemical2EnableButton.forceState(data.is_enabled == 1);
            Unicode::strncpy(NameEditText2Buffer, data.name, NAMEEDITTEXT2_SIZE);
            Unicode::strncpy(VolumeEditText2Buffer, volBuffer, VOLUMEEDITTEXT2_SIZE);
            NameEditText2.invalidate();
            VolumeEditText2.invalidate();
            Chemical2EnableButton.invalidate();
            break;

        case 2: // Page 3
            Chemical3EnableButton.forceState(data.is_enabled == 1);
            Unicode::strncpy(NameEditText3Buffer, data.name, NAMEEDITTEXT3_SIZE);
            Unicode::strncpy(VolumeEditText3Buffer, volBuffer, VOLUMEEDITTEXT3_SIZE);
            NameEditText3.invalidate();
            VolumeEditText3.invalidate();
            Chemical3EnableButton.invalidate();
            break;

        case 3: // Page 4
            Chemical4EnableButton.forceState(data.is_enabled == 1);
            Unicode::strncpy(NameEditText4Buffer, data.name, NAMEEDITTEXT4_SIZE);
            Unicode::strncpy(VolumeEditText4Buffer, volBuffer, VOLUMEEDITTEXT4_SIZE);
            NameEditText4.invalidate();
            VolumeEditText4.invalidate();
            Chemical4EnableButton.invalidate();
            break;

        case 4: // Page 5
            Chemical5EnableButton.forceState(data.is_enabled == 1);
            Unicode::strncpy(NameEditText5Buffer, data.name, NAMEEDITTEXT5_SIZE);
            Unicode::strncpy(VolumeEditText5Buffer, volBuffer, VOLUMEEDITTEXT5_SIZE);
            NameEditText5.invalidate();
            VolumeEditText5.invalidate();
            Chemical5EnableButton.invalidate();
            break;

        case 5: // Page 6
            Chemical6EnableButton.forceState(data.is_enabled == 1);
            Unicode::strncpy(NameEditText6Buffer, data.name, NAMEEDITTEXT6_SIZE);
            Unicode::strncpy(VolumeEditText6Buffer, volBuffer, VOLUMEEDITTEXT6_SIZE);
            NameEditText6.invalidate();
            VolumeEditText6.invalidate();
            Chemical6EnableButton.invalidate();
            break;

        case 6: // Page 7
            Chemical7EnableButton.forceState(data.is_enabled == 1);
            Unicode::strncpy(NameEditText7Buffer, data.name, NAMEEDITTEXT7_SIZE);
            Unicode::strncpy(VolumeEditText7Buffer, volBuffer, VOLUMEEDITTEXT7_SIZE);
            NameEditText7.invalidate();
            VolumeEditText7.invalidate();
            Chemical7EnableButton.invalidate();
            break;

        case 7: // Page 8
            Chemical8EnableButton.forceState(data.is_enabled == 1);
            Unicode::strncpy(NameEditText8Buffer, data.name, NAMEEDITTEXT8_SIZE);
            Unicode::strncpy(VolumeEditText8Buffer, volBuffer, VOLUMEEDITTEXT8_SIZE);
            NameEditText8.invalidate();
            VolumeEditText8.invalidate();
            Chemical8EnableButton.invalidate();
            break;
    }

    // --- Pump Widget and Add/Remove Button Logic ---
    // This part is more complex because the widgets themselves might be unique to each page
    // or they might be reused. For now, this logic will only affect the widgets on the
    // currently visible page. We will address making this work for all pages in the next step.
    if (Page == currentPageIndex) {
        int visiblePumpSetups = 0;
        for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
        {
            pumpSetupWidgets[i]->setAvailablePumps(enabled_pumps);
            pumpSetupWidgets[i]->setup(i, data.pump_setups[i], unit);
            pumpSetupWidgets[i]->setVisible(data.pump_setups[i].pump_index != -1);
            if (pumpSetupWidgets[i]->isVisible()) {
                visiblePumpSetups++;
            }
            pumpSetupWidgets[i]->invalidate();
        }

        // We also need to target the correct Add/Remove buttons
        switch(Page)
        {
            case 0:
                AddPump1.setVisible(visiblePumpSetups < MAX_PUMP_SETUPS_PER_CHEMICAL);
                RemovePump1.setVisible(visiblePumpSetups > 1);
                AddPump1.invalidate();
                RemovePump1.invalidate();
                break;
            case 1:
                AddPump2.setVisible(visiblePumpSetups < MAX_PUMP_SETUPS_PER_CHEMICAL);
                RemovePump2.setVisible(visiblePumpSetups > 1);
                AddPump2.invalidate();
                RemovePump2.invalidate();
                break;
            // Add cases for pages 2-7 here
        }
    }
}

void ChemicalsSetupView::handleClickEvent(const touchgfx::ClickEvent& event)
{
    // For a SwipeContainer, we only need to detect the END of the interaction.
    if (event.getType() == touchgfx::ClickEvent::RELEASED)
    {
        // The user lifted their finger. The swipe container will have finished
        // its animation and selected a new page.
        int newPageIndex = swipeContainer1.getSelectedPage();

        if (newPageIndex != currentPageIndex)
        {
            currentPageIndex = newPageIndex;
            presenter->ActiveFieldIndexUpdate(currentPageIndex);
        }
    }

    // Always pass the event to the base class so it can handle button clicks etc.
    ChemicalsSetupViewBase::handleClickEvent(event);
}

// We don't need custom drag logic if the SwipeContainer handles it for us.
// Just pass it to the base class.
void ChemicalsSetupView::handleDragEvent(const touchgfx::DragEvent& event)
{
    ChemicalsSetupViewBase::handleDragEvent(event);
}
