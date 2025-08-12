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
    pumpSetupWidgets[0] = &pumpSetupWidget1_1;
    pumpSetupWidgets[1] = &pumpSetupWidget1_2;
    pumpSetupWidgets[2] = &pumpSetupWidget1_3;

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

touchgfx::Container* ChemicalsSetupView::getPageContainerForIndex(int index)
{
    switch(index)
    {
        case 0: return &Page1; case 1: return &Page2;
        case 2: return &Page3; case 3: return &Page4;
        case 4: return &Page5; case 5: return &Page6;
        case 6: return &Page7; case 7: return &Page8;
        default: return nullptr;
    }
}

// --- NEW: Implementation of the getter function ---
touchgfx::ScrollableContainer* ChemicalsSetupView::getScrollableContainerForPage(int pageIndex)
{
    // This function has access to the protected members from the base class.
    // It returns a pointer to the correct scrollable container based on the index.
    // The names (scrollableContainer1, etc.) must match your Designer widget names.
    switch (pageIndex)
    {
        case 0: return &scrollableContainer;
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

// In ChemicalsSetupView.cpp

void ChemicalsSetupView::displayData(const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit)
{
    // --- 1. MOVE ALL WIDGETS TO THE CORRECT PAGE ---
    // (This is your working widget-moving logic from the baseline)
    Container* newParentPage = getPageContainerForIndex(currentPageIndex);
    if (!newParentPage) return; // Safety check

    auto moveWidget = [&](Drawable& widget, Container& newParent) {
        if (widget.getParent()) {
            static_cast<Container*>(widget.getParent())->remove(widget);
        }
        newParent.add(widget);
    };

    moveWidget(chemicalLabel, *newParentPage);
    moveWidget(ChemicalEnableButton, *newParentPage);
    moveWidget(NameBox, *newParentPage);
    moveWidget(NameEditText, *newParentPage);
    moveWidget(NameEdit, *newParentPage);
    moveWidget(VolumeBox, *newParentPage);
    moveWidget(VolumeEditText, *newParentPage);
    moveWidget(VolumeEdit, *newParentPage);
    moveWidget(AddPumpBox, *newParentPage);
    moveWidget(AddPumpText, *newParentPage); // Assuming you renamed this in the Designer
    moveWidget(AddPump, *newParentPage);
    moveWidget(RemovePump, *newParentPage);
    moveWidget(scrollableContainer, *newParentPage); // Move the scrollable container too

    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        moveWidget(*pumpSetupWidgets[i], scrollableContainer);
    }

    // --- 2. POPULATE THE WIDGETS WITH THE NEW DATA ---
    // (This is your working data population logic from the baseline)
    Unicode::snprintf(chemicalLabelBuffer, CHEMICALLABEL_SIZE, "Chemical %d:", currentPageIndex + 1);
    ChemicalEnableButton.forceState(data.is_enabled == 1);
    Unicode::strncpy(NameEditTextBuffer, data.name, NAMEEDITTEXT_SIZE);
    char volAnsiBuffer[20];
    const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
    snprintf(volAnsiBuffer, 20, "%.2f %s", data.total_dispense_volume, unit_suffix);
    Unicode::strncpy(VolumeEditTextBuffer, volAnsiBuffer, VOLUMEEDITTEXT_SIZE);

    int visiblePumpSetups = 0;
    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        pumpSetupWidgets[i]->setAvailablePumps(enabled_pumps);
        pumpSetupWidgets[i]->setup(i, data.pump_setups[i], unit);
        pumpSetupWidgets[i]->setVisible(data.pump_setups[i].pump_index != -1);
        if(pumpSetupWidgets[i]->isVisible()) visiblePumpSetups++;
    }

    AddPump.setVisible(visiblePumpSetups < MAX_PUMP_SETUPS_PER_CHEMICAL);
    RemovePump.setVisible(visiblePumpSetups > 1);

    // *** THIS IS THE CRITICAL FIX FOR THE BLANK SCREEN ***
    // After moving and updating everything, tell the entire page container to redraw itself.
    newParentPage->invalidate();
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

//void ChemicalsSetupView::chemicalEnableButtonClicked()
//{
//    presenter->chemicalEnableToggled();
//}

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






void ChemicalsSetupView::handleClickEvent(const touchgfx::ClickEvent& event)
{
    // First, let the base class handle any button clicks.
    ChemicalsSetupViewBase::handleClickEvent(event);

    // Now, add our logic to detect a swipe completion.
    if (event.getType() == touchgfx::ClickEvent::RELEASED)
    {
        int newPageIndex = swipeContainer1.getSelectedPage();
        if (newPageIndex != currentPageIndex)
        {
            // The page has changed.
            // 1. Update our own state.
            currentPageIndex = newPageIndex;

            // 2. Inform the presenter of the new active page index.
            presenter->ActiveFieldIndexUpdate(currentPageIndex);

            // *** THIS IS THE CRITICAL, MISSING STEP ***
            // 3. Command the Presenter to load and display the data for the new page.
            presenter->loadScreenData(currentPageIndex);
        }
    }
}

//// We don't need custom drag logic if the SwipeContainer handles it for us.
//// Just pass it to the base class.
//void ChemicalsSetupView::handleDragEvent(const touchgfx::DragEvent& event)
//{
//    // Always pass the event to the base class first.
//    ChemicalsSetupViewBase::handleDragEvent(event);
//
//    if (event.getType() == touchgfx::DragEvent::DRAG_START)
//    {
//        // Remember where the drag started if it's within the swipe container
//        if (swipeContainer1.getRect().intersect(event.getOldX(), event.getOldY()))
//        {
//            dragStartX = event.getOldX();
//        }
//    }
//    else if (event.getType() == touchgfx::DragEvent::DRAG_END && dragStartX != 0)
//    {
//        int16_t dragEnd = event.getNewX();
//        int16_t deltaX = dragEnd - dragStartX;
//        const int16_t swipeThreshold = 50;
//
//        int newPage = currentPageIndex;
//
//        if (deltaX < -swipeThreshold)
//        {
//            // Swipe Left
//            newPage = currentPageIndex + 1;
//            if (newPage >= NUM_CHEMICAL_RECIPES) newPage = NUM_CHEMICAL_RECIPES - 1;
//        }
//        else if (deltaX > swipeThreshold)
//        {
//            // Swipe Right
//            newPage = currentPageIndex - 1;
//            if (newPage < 0) newPage = 0;
//        }
//
//        if (newPage != currentPageIndex)
//        {
//            // If a valid swipe occurred, animate the container and load the new data
//            swipeContainer1.setSelectedPage(newPage);
//            currentPageIndex = newPage;
//            presenter->loadScreenData(currentPageIndex);
//        }
//
//        // Reset for the next drag
//        dragStartX = 0;
//    }
//}
