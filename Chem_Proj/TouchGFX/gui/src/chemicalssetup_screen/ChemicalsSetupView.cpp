#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <cstdio>
#include <cstring>
#include <gui/common/FieldIDs.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/events/ClickEvent.hpp>

ChemicalsSetupView::ChemicalsSetupView() :
	dragStartX(0),
    pumpSetupSaveCallback(this, &ChemicalsSetupView::pumpSetupSaveDataCallbackHandler),
    pumpSetupVolumeEditCallback(this, &ChemicalsSetupView::pumpSetupVolumeEditCallbackHandler),
    dropdownStateCallback(this, &ChemicalsSetupView::dropdownStateCallbackHandler),
    currentPageIndex(0)
{
    // Initialize the array of reusable pump widgets.
    // NOTE: These names must match the widgets placed on Page1 in the Designer.
    pumpSetupWidgets[0] = &pumpSetupWidget_Center_1;
    pumpSetupWidgets[1] = &pumpSetupWidget_Center_2;
    pumpSetupWidgets[2] = &pumpSetupWidget_Center_3;

    // Set up keyboard position and add it to the view, initially hidden.
    keyboard.setPosition(0, 115, 240, 180);
    add(keyboard);
    keyboard.setVisible(false);
}

touchgfx::Container* ChemicalsSetupView::getPageContainerForIndex(int index)
{
    // NOTE: These names must match the Page containers in the Designer.
    switch(index)
    {
        case 0: return &Page1;
        case 1: return &Page2;
        case 2: return &Page3;
        case 3: return &Page4;
        case 4: return &Page5;
        case 5: return &Page6;
        case 6: return &Page7;
        case 7: return &Page8;
        default: return nullptr;
    }
}

touchgfx::ScrollableContainer* ChemicalsSetupView::getScrollableContainerForPage(int pageIndex)
{
    // NOTE: These names must match the scrollable containers in the Designer.
    switch (pageIndex)
    {
        case 0: return &scrollableContainer1_Center;
        case 1: return &scrollableContainer2;
        case 2: return &scrollableContainer3;
        case 3: return &scrollableContainer4;
        case 4: return &scrollableContainer5;
        case 5: return &scrollableContainer6;
        case 6: return &scrollableContainer7;
        case 7: return &scrollableContainer8;
        default: return nullptr;
    }
}

void ChemicalsSetupView::setupScreen()
{
    ChemicalsSetupViewBase::setupScreen();

    // Connect callbacks to the reusable pump widgets
    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        pumpSetupWidgets[i]->saveDataCallback = &pumpSetupSaveCallback;
        pumpSetupWidgets[i]->volumeEditClickedCallback = &pumpSetupVolumeEditCallback;
        pumpSetupWidgets[i]->dropdownStateChangedCallback = &dropdownStateCallback;
    }

    // Trigger the initial data load for the starting page.
    currentPageIndex = swipeContainer1.getSelectedPage();
    presenter->loadScreenData(currentPageIndex);
}

// In ChemicalsSetupView.cpp

void ChemicalsSetupView::displayData(const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit)
{
    const float ML_PER_OZ = 29.5735f;

    // --- 1. SET BUFFER VISIBILITY ---
    WidgetBuffer_Center.setVisible(true);
    WidgetBuffer_Left.setVisible(currentPageIndex > 0);
    WidgetBuffer_Right.setVisible(currentPageIndex < NUM_CHEMICAL_RECIPES - 1);

    // --- 2. LOGIC FOR THE CENTER PAGE (Full Detail) ---
    {
        Container* newParentPage = getPageContainerForIndex(currentPageIndex);
        if (newParentPage) {
            if (WidgetBuffer_Center.getParent() != newParentPage) {
                if (WidgetBuffer_Center.getParent()) {
                    static_cast<Container*>(WidgetBuffer_Center.getParent())->remove(WidgetBuffer_Center);
                }
                newParentPage->add(WidgetBuffer_Center);
            }

            float display_total_volume = data.total_dispense_volume;
            if (unit == 1) {
                display_total_volume /= ML_PER_OZ;
            }

            Unicode::snprintf(chemicalLabel_CenterBuffer, CHEMICALLABEL_CENTER_SIZE, "Chemical %d:", currentPageIndex + 1);
            ChemicalEnableButton_Center.forceState(data.is_enabled == 1);
            Unicode::strncpy(NameEditText_CenterBuffer, data.name, NAMEEDITTEXT_CENTER_SIZE);

            char volAnsiBuffer[20];
            const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
            snprintf(volAnsiBuffer, 20, "%.2f %s", display_total_volume, unit_suffix);
            Unicode::strncpy(VolumeEditText_CenterBuffer, volAnsiBuffer, VOLUMEEDITTEXT_CENTER_SIZE);

            // Configure the fully interactive pump widgets for the Center buffer
            PumpSetupWidget* centerPumpWidgets[] = { &pumpSetupWidget_Center_1, &pumpSetupWidget_Center_2, &pumpSetupWidget_Center_3 };
            int visiblePumpSetups = 0;
            for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i) {
                PumpSetup_t display_pump_setup = data.pump_setups[i];
                if (unit == 1) {
                    display_pump_setup.dispense_small /= ML_PER_OZ;
                    display_pump_setup.dispense_medium /= ML_PER_OZ;
                    display_pump_setup.dispense_large /= ML_PER_OZ;
                }
                centerPumpWidgets[i]->setAvailablePumps(enabled_pumps);
                centerPumpWidgets[i]->setup(i, display_pump_setup, unit);
                centerPumpWidgets[i]->setVisible(data.pump_setups[i].pump_index != -1);
                if(centerPumpWidgets[i]->isVisible()) visiblePumpSetups++;
            }

            // Hide loading text and show the pump widgets for the Center page
            scrollableContainer1_Center.setVisible(true); // Always ensure the container is visible

            AddPump_Center.setVisible(visiblePumpSetups < MAX_PUMP_SETUPS_PER_CHEMICAL);
            RemovePump_Center.setVisible(visiblePumpSetups > 1);

            newParentPage->invalidate();
        }
    }


    // --- 3. LOGIC FOR THE LEFT PAGE (Preview Only) ---
    if (currentPageIndex > 0) {
        const ChemicalRecipe_t& leftData = presenter->getRecipeDataForPage(currentPageIndex - 1);
        Container* leftParentPage = getPageContainerForIndex(currentPageIndex - 1);
        if (leftParentPage) {
            if (WidgetBuffer_Left.getParent() != leftParentPage) {
                if (WidgetBuffer_Left.getParent()) {
                    static_cast<Container*>(WidgetBuffer_Left.getParent())->remove(WidgetBuffer_Left);
                }
                leftParentPage->add(WidgetBuffer_Left);
            }

            float display_total_volume_left = leftData.total_dispense_volume;
            if (unit == 1) {
                display_total_volume_left /= ML_PER_OZ;
            }

            Unicode::snprintf(chemicalLabel_LeftBuffer, CHEMICALLABEL_LEFT_SIZE, "Chemical %d:", (currentPageIndex - 1) + 1);
            ChemicalEnableButton_Left.forceState(leftData.is_enabled == 1);
            Unicode::strncpy(NameEditText_LeftBuffer, leftData.name, NAMEEDITTEXT_LEFT_SIZE);

            char volAnsiBuffer[20];
            const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
            snprintf(volAnsiBuffer, 20, "%.2f %s", display_total_volume_left, unit_suffix);
            Unicode::strncpy(VolumeEditText_LeftBuffer, volAnsiBuffer, VOLUMEEDITTEXT_LEFT_SIZE);

            // --- SIMPLIFIED: No pump widgets needed for the preview ---
            LoadingText_Left.setVisible(true);
            AddPump_Left.setVisible(false);
            RemovePump_Left.setVisible(false);

            leftParentPage->invalidate();
        }
    }

    // --- 4. LOGIC FOR THE RIGHT PAGE (Preview Only) ---
    if (currentPageIndex < NUM_CHEMICAL_RECIPES - 1) {
        const ChemicalRecipe_t& rightData = presenter->getRecipeDataForPage(currentPageIndex + 1);
        Container* rightParentPage = getPageContainerForIndex(currentPageIndex + 1);
        if (rightParentPage) {
            if (WidgetBuffer_Right.getParent() != rightParentPage) {
                if (WidgetBuffer_Right.getParent()) {
                    static_cast<Container*>(WidgetBuffer_Right.getParent())->remove(WidgetBuffer_Right);
                }
                rightParentPage->add(WidgetBuffer_Right);
            }

            float display_total_volume_right = rightData.total_dispense_volume;
            if (unit == 1) {
                display_total_volume_right /= ML_PER_OZ;
            }

            Unicode::snprintf(chemicalLabel_RightBuffer, CHEMICALLABEL_RIGHT_SIZE, "Chemical %d:", (currentPageIndex + 1) + 1);
            ChemicalEnableButton_Right.forceState(rightData.is_enabled == 1);
            Unicode::strncpy(NameEditText_RightBuffer, rightData.name, NAMEEDITTEXT_RIGHT_SIZE);

            char volAnsiBuffer[20];
            const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
            snprintf(volAnsiBuffer, 20, "%.2f %s", display_total_volume_right, unit_suffix);
            Unicode::strncpy(VolumeEditText_RightBuffer, volAnsiBuffer, VOLUMEEDITTEXT_RIGHT_SIZE);

            // --- SIMPLIFIED: No pump widgets needed for the preview ---
            LoadingText_Right.setVisible(true);
            AddPump_Right.setVisible(false);
            RemovePump_Right.setVisible(false);

            rightParentPage->invalidate();
        }
    }
}

void ChemicalsSetupView::handleTickEvent()
{
    int newPageIndex = swipeContainer1.getSelectedPage();
    if (newPageIndex != currentPageIndex)
    {
        // The page has changed. Update our state and trigger a data reload.
        int newPageIndex = swipeContainer1.getSelectedPage();
        currentPageIndex = newPageIndex;
        presenter->loadScreenData(currentPageIndex);
    }
}

//void ChemicalsSetupView::handleDragEvent(const touchgfx::DragEvent& event)
//{
//    // Let the SwipeContainer do its own dragging logic first
//    swipeContainer1.handleDragEvent(event);
//
//    // Now add our custom logic on top
//    int16_t deltaX = event.getDeltaX();
//
//    if (dragStartX == 0)
//    {
//        // This is the first drag event in a sequence. Remember the start position.
//        dragStartX = event.getOldX();
//    }
//
//    // Check if a swipe gesture has just been completed
//    if (dragStartX != 0)
//    {
//        // A swipe to the left is a negative delta
//        if (deltaX < 0 && swipeContainer1.getSelectedPage() > currentPageIndex)
//        {
//            // The swipe container has just snapped to the NEXT page
//            currentPageIndex = swipeContainer1.getSelectedPage();
//            presenter->ActiveFieldIndexUpdate(currentPageIndex);
//            int newPageIndex = swipeContainer1.getSelectedPage();
//            currentPageIndex = newPageIndex;
//            presenter->loadScreenData(currentPageIndex - 1);
//            dragStartX = 0; // Reset for the next gesture
//        }
//        // A swipe to the right is a positive delta
//        else if (deltaX > 0 && swipeContainer1.getSelectedPage() < currentPageIndex)
//        {
//            // The swipe container has just snapped to the PREVIOUS page
//            currentPageIndex = swipeContainer1.getSelectedPage();
//            presenter->ActiveFieldIndexUpdate(currentPageIndex);
//            int newPageIndex = swipeContainer1.getSelectedPage();
//            currentPageIndex = newPageIndex;
//            presenter->loadScreenData(currentPageIndex + 1);
//            dragStartX = 0; // Reset for the next gesture
//        }
//    }
//}
// --- Interaction Handlers ---
void ChemicalsSetupView::requestDataLoad() { presenter->loadScreenData(currentPageIndex); }
void ChemicalsSetupView::showKeyboard() { keyboard.setVisible(true); KeyboardExtras.setVisible(true); keyboard.invalidate(); KeyboardExtras.invalidate(); }
void ChemicalsSetupView::addPumpClicked() { presenter->addPumpSetup(currentPageIndex); }
void ChemicalsSetupView::removePumpClicked() { presenter->removePumpSetup(currentPageIndex); }
void ChemicalsSetupView::chemicalEnableButtonClicked() { presenter->chemicalEnableToggled(); }
void ChemicalsSetupView::nameEditClicked() { presenter->editField(FIELD_CHEM_NAME); }
void ChemicalsSetupView::volumeEditClicked() { presenter->editField(FIELD_TOTAL_VOLUME); }
void ChemicalsSetupView::EnterPressed()
{
    keyboard.setVisible(false);
    KeyboardExtras.setVisible(false);
    keyboard.invalidate();
    KeyboardExtras.invalidate();

    char utf8_buffer[20];
    memset(utf8_buffer, 0, 20);
    Unicode::toUTF8(keyboard.getBuffer(), (uint8_t*)utf8_buffer, 20);

    presenter->newValueEntered(utf8_buffer);
    keyboard.clearBuffer();
}
void ChemicalsSetupView::ExitPressed()
{
    keyboard.setVisible(false);
    KeyboardExtras.setVisible(false);
    keyboard.invalidate();
    KeyboardExtras.invalidate();
    keyboard.clearBuffer();
}

void ChemicalsSetupView::invalidateEnableButton()
{
    // This function forces the single, reusable toggle button to redraw itself,
    // ensuring its visual state (on/off) is up to date.
    ChemicalEnableButton_Center.invalidate();
}

// --- Child Widget Callback Handlers ---
void ChemicalsSetupView::pumpSetupSaveDataCallbackHandler(int setupIndex, const PumpSetup_t& data)
{
    presenter->savePumpSetupData(currentPageIndex, setupIndex, data);
}
void ChemicalsSetupView::pumpSetupVolumeEditCallbackHandler(int setupIndex, int fieldIndex)
{
    presenter->editField(FIELD_PUMP_SETUP_START + (setupIndex * 10) + fieldIndex);
}
void ChemicalsSetupView::dropdownStateCallbackHandler(bool isOpen)
{
    ScrollableContainer* currentScrollContainer = getScrollableContainerForPage(currentPageIndex);
    if (currentScrollContainer) {
        currentScrollContainer->setTouchable(!isOpen);
    }
}
