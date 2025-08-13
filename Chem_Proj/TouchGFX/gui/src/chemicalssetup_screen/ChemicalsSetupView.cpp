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

void ChemicalsSetupView::displayData(const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit)
{

    WidgetBuffer_Center.setVisible(true);
    WidgetBuffer_Left.setVisible(currentPageIndex > 0);
    WidgetBuffer_Right.setVisible(currentPageIndex < NUM_CHEMICAL_RECIPES - 1);


    if (currentPageIndex > 0) {

        // *** CHANGE #1: Get the data for the PREVIOUS page ***
        int visiblePumpSetups = 0;
        const ChemicalRecipe_t& leftData = presenter->getRecipeDataForPage(currentPageIndex - 1);
        const std::vector<int> leftEnabledPumps = presenter->getEnabledPumpIndices(); // This is shared, but good practice to re-get
        int8_t leftUnit = presenter->getVolumeUnit(); // Also shared

    	Container* leftParentPage = getPageContainerForIndex(currentPageIndex - 1);
        ScrollableContainer* leftScrollParent = getScrollableContainerForPage(currentPageIndex - 1);
        if (!leftParentPage || !leftScrollParent) return;

        // 2. A helper lambda to safely move a widget
        auto moveWidget = [&](Drawable& widget, Container& leftParent) {
            if (widget.getParent()) {
                static_cast<Container*>(widget.getParent())->remove(widget);
            }
            leftParent.add(widget);
        };

        // 3. Move all reusable widgets to the new page
        moveWidget(chemicalLabel_Left, *leftParentPage);
        moveWidget(ChemicalEnableButton_Left, *leftParentPage);
        moveWidget(ChemNameText_Left, *leftParentPage);
        moveWidget(ChemVolumeText_Left, *leftParentPage);
        moveWidget(NameBox_Left, *leftParentPage);
        moveWidget(NameEditText_Left, *leftParentPage);
        moveWidget(NameEdit_Left, *leftParentPage);
        moveWidget(VolumeBox_Left, *leftParentPage);
        moveWidget(VolumeEditText_Left, *leftParentPage);
        moveWidget(VolumeEdit_Left, *leftParentPage);
        moveWidget(AddPumpBox_Left, *leftParentPage);
        moveWidget(AddPumpText_Left, *leftParentPage);
        moveWidget(AddPump_Left, *leftParentPage);
        moveWidget(RemovePump_Left, *leftParentPage);
        moveWidget(LoadingText_Left, *leftParentPage);

        if (currentPageIndex > 0) {

            LoadingText_Left.setVisible(false);
            leftParentPage->invalidate();
        }

        // NOTE: Moving the same pumpSetupWidgets to multiple containers is logically incorrect
        // and will cause problems. This should be addressed later.
        // For now, per your request, the syntax is preserved.
        for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
        {
            moveWidget(*pumpSetupWidgets[i], *leftScrollParent);
        }

        // *** CHANGE #2: Populate the widgets with the correct leftData ***
        Unicode::snprintf(chemicalLabel_LeftBuffer, CHEMICALLABEL_LEFT_SIZE, "Chemical %d:", (currentPageIndex - 1) + 1);
        ChemicalEnableButton_Left.forceState(leftData.is_enabled == 1);
        Unicode::strncpy(NameEditText_LeftBuffer, leftData.name, NAMEEDITTEXT_LEFT_SIZE);

        char volAnsiBuffer[20];
        const char* unit_suffix = (leftUnit == 1) ? "Oz" : "mL";
        snprintf(volAnsiBuffer, 20, "%.2f %s", leftData.total_dispense_volume, unit_suffix);
        Unicode::strncpy(VolumeEditText_LeftBuffer, volAnsiBuffer, VOLUMEEDITTEXT_LEFT_SIZE);

        // 5. Configure pump widgets and Add/Remove button visibility using leftData
        // 6. Force a redraw of the entire page to prevent a blank screen

        LoadingText_Left.setVisible(visiblePumpSetups == 0);
        leftParentPage->invalidate();
    }

    // --- LOGIC FOR THE RIGHT PAGE ---
    if (currentPageIndex < NUM_CHEMICAL_RECIPES - 1) {

        // *** Get the data for the NEXT page ***
        int visiblePumpSetups = 0;
        const ChemicalRecipe_t& rightData = presenter->getRecipeDataForPage(currentPageIndex + 1);
        const std::vector<int> rightEnabledPumps = presenter->getEnabledPumpIndices();
        int8_t rightUnit = presenter->getVolumeUnit();

    	Container* rightParentPage = getPageContainerForIndex(currentPageIndex + 1);
        ScrollableContainer* rightScrollParent = getScrollableContainerForPage(currentPageIndex + 1);
        if (!rightParentPage || !rightScrollParent) return;

        // 2. A helper lambda to safely move a widget
        auto moveWidget = [&](Drawable& widget, Container& rightParent) {
            if (widget.getParent()) {
                static_cast<Container*>(widget.getParent())->remove(widget);
            }
            rightParent.add(widget);
        };

        // 3. Move all reusable widgets to the new page
        moveWidget(chemicalLabel_Right, *rightParentPage);
        moveWidget(ChemicalEnableButton_Right, *rightParentPage);
        moveWidget(ChemNameText_Right, *rightParentPage);
        moveWidget(ChemVolumeText_Right, *rightParentPage);
        moveWidget(NameBox_Right, *rightParentPage);
        moveWidget(NameEditText_Right, *rightParentPage);
        moveWidget(NameEdit_Right, *rightParentPage);
        moveWidget(VolumeBox_Right, *rightParentPage);
        moveWidget(VolumeEditText_Right, *rightParentPage);
        moveWidget(VolumeEdit_Right, *rightParentPage);
        moveWidget(AddPumpBox_Right, *rightParentPage);
        moveWidget(AddPumpText_Right, *rightParentPage);
        moveWidget(AddPump_Right, *rightParentPage);
        moveWidget(RemovePump_Right, *rightParentPage);
        moveWidget(LoadingText_Right, *rightParentPage);



        // NOTE: Moving the same pumpSetupWidgets to multiple containers is logically incorrect.
        for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
        {
            moveWidget(*pumpSetupWidgets[i], *rightScrollParent);
        }

        // 4. Populate the widgets with the correct rightData
        Unicode::snprintf(chemicalLabel_RightBuffer, CHEMICALLABEL_RIGHT_SIZE, "Chemical %d:", (currentPageIndex + 1) + 1);
        ChemicalEnableButton_Right.forceState(rightData.is_enabled == 1);
        Unicode::strncpy(NameEditText_RightBuffer, rightData.name, NAMEEDITTEXT_RIGHT_SIZE);

        char volAnsiBuffer[20];
        const char* unit_suffix = (rightUnit == 1) ? "Oz" : "mL";
        snprintf(volAnsiBuffer, 20, "%.2f %s", rightData.total_dispense_volume, unit_suffix);
        Unicode::strncpy(VolumeEditText_RightBuffer, volAnsiBuffer, VOLUMEEDITTEXT_RIGHT_SIZE);
        LoadingText_Right.setVisible(visiblePumpSetups == 0);

        // 6. Force a redraw of the entire page to prevent a blank screen
    }
    // 1. Get the parent containers for the current page
    WidgetBuffer_Center.setVisible(true);
    Container* newParentPage = getPageContainerForIndex(currentPageIndex);
    ScrollableContainer* newScrollParent = getScrollableContainerForPage(currentPageIndex);
    if (!newParentPage || !newScrollParent) return;

    // 2. A helper lambda to safely move a widget
    auto moveWidget = [&](Drawable& widget, Container& newParent) {
        if (widget.getParent()) {
            static_cast<Container*>(widget.getParent())->remove(widget);
        }
        newParent.add(widget);
    };

    // 3. Move all reusable widgets to the new page
    moveWidget(chemicalLabel_Center, *newParentPage);
    moveWidget(ChemicalEnableButton_Center, *newParentPage);
    moveWidget(ChemNameText_Center, *newParentPage);
    moveWidget(ChemVolumeText_Center, *newParentPage);
    moveWidget(NameBox_Center, *newParentPage);
    moveWidget(NameEditText_Center, *newParentPage);
    moveWidget(NameEdit_Center, *newParentPage);
    moveWidget(VolumeBox_Center, *newParentPage);
    moveWidget(VolumeEditText_Center, *newParentPage);
    moveWidget(VolumeEdit_Center, *newParentPage);
    moveWidget(AddPumpBox_Center, *newParentPage);
    moveWidget(AddPumpText_Center, *newParentPage);
    moveWidget(AddPump_Center, *newParentPage);
    moveWidget(RemovePump_Center, *newParentPage);

    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        moveWidget(*pumpSetupWidgets[i], *newScrollParent);
    }

    // 4. Populate the widgets with new data
    Unicode::snprintf(chemicalLabel_CenterBuffer, CHEMICALLABEL_CENTER_SIZE, "Chemical %d:", currentPageIndex + 1);
    ChemicalEnableButton_Center.forceState(data.is_enabled == 1);
    Unicode::strncpy(NameEditText_CenterBuffer, data.name, NAMEEDITTEXT_CENTER_SIZE);

    char volAnsiBuffer[20];
    const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
    snprintf(volAnsiBuffer, 20, "%.2f %s", data.total_dispense_volume, unit_suffix);
    Unicode::strncpy(VolumeEditText_CenterBuffer, volAnsiBuffer, VOLUMEEDITTEXT_CENTER_SIZE);

    // 5. Configure pump widgets and Add/Remove button visibility
    int visiblePumpSetups = 0;
    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        pumpSetupWidgets[i]->setAvailablePumps(enabled_pumps);
        pumpSetupWidgets[i]->setup(i, data.pump_setups[i], unit);
        pumpSetupWidgets[i]->setVisible(data.pump_setups[i].pump_index != -1);
        if(pumpSetupWidgets[i]->isVisible()) visiblePumpSetups++;
    }
    scrollableContainer1_Center.setVisible(visiblePumpSetups > 0);
    AddPump_Center.setVisible(visiblePumpSetups < MAX_PUMP_SETUPS_PER_CHEMICAL);
    RemovePump_Center.setVisible(visiblePumpSetups > 1);
    // 6. Force a redraw of the entire page to prevent a blank screen

    if (currentPageIndex > 6){
    	LoadingText_Right.setVisible(false);
    }

    if (currentPageIndex < 1){
    	LoadingText_Left.setVisible(false);
    }

    newParentPage->invalidate();

    // --- LOGIC FOR THE LEFT PAGE ---

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
