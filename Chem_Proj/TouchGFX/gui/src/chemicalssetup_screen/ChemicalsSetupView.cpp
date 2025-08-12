#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <cstdio>
#include <cstring>
#include <gui/common/FieldIDs.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/events/ClickEvent.hpp>

ChemicalsSetupView::ChemicalsSetupView() :
    pumpSetupSaveCallback(this, &ChemicalsSetupView::pumpSetupSaveDataCallbackHandler),
    pumpSetupVolumeEditCallback(this, &ChemicalsSetupView::pumpSetupVolumeEditCallbackHandler),
    dropdownStateCallback(this, &ChemicalsSetupView::dropdownStateCallbackHandler),
    currentPageIndex(0)
{
    // Initialize the array of reusable pump widgets.
    // NOTE: These names must match the widgets placed on Page1 in the Designer.
//    pumpSetupWidgets[0] = &pumpSetupWidget1;
//    pumpSetupWidgets[1] = &pumpSetupWidget2;
//    pumpSetupWidgets[2] = &pumpSetupWidget3;

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
//    switch (pageIndex)
//    {
//        case 0: return &scrollableContainer1;
//        case 1: return &scrollableContainer2;
//        case 2: return &scrollableContainer3;
//        case 3: return &scrollableContainer4;
//        case 4: return &scrollableContainer5;
//        case 5: return &scrollableContainer6;
//        case 6: return &scrollableContainer7;
//        case 7: return &scrollableContainer8;
//        default: return nullptr;
//    }
}

void ChemicalsSetupView::setupScreen()
{
    ChemicalsSetupViewBase::setupScreen();
    currentPageIndex = swipeContainer1.getSelectedPage();
    presenter->ActiveFieldIndexUpdate(currentPageIndex);
    updateVisiblePages();
}


void ChemicalsSetupView::updateVisiblePages()
{
    int centerIndex = currentPageIndex;
    int leftIndex = centerIndex - 1;
    int rightIndex = centerIndex + 1;

    // --- Center Buffer ---
    PumpSetupWidget* centerPumpWidgets[] = { &pumpSetupWidget_Center_1, &pumpSetupWidget_Center_2, &pumpSetupWidget_Center_3 };
    populateWidgetBuffer(centerIndex, WidgetBuffer_Center, ChemicalEnableButton_Center,
                         NameEditText_Center, NameEditText_CenterBuffer, NAMEEDITTEXT_CENTER_SIZE,
                         VolumeEditText_Center, VolumeEditText_CenterBuffer, VOLUMEEDITTEXT_CENTER_SIZE,
                         AddPump_Center, RemovePump_Center, centerPumpWidgets);

    // --- Left Buffer ---
    if (leftIndex >= 0) {
        WidgetBuffer_Left.setVisible(true);
        PumpSetupWidget* leftPumpWidgets[] = { &pumpSetupWidget_Left_1, &pumpSetupWidget_Left_2, &pumpSetupWidget_Left_3 };
        populateWidgetBuffer(leftIndex, WidgetBuffer_Left, ChemicalEnableButton_Left,
                             NameEditText_Left, NameEditText_LeftBuffer, NAMEEDITTEXT_LEFT_SIZE,
                             VolumeEditText_Left, VolumeEditText_LeftBuffer, VOLUMEEDITTEXT_LEFT_SIZE,
                             AddPump_Left, RemovePump_Left, leftPumpWidgets);
    } else {
        WidgetBuffer_Left.setVisible(false);
    }

    // --- Right Buffer ---
    if (rightIndex < NUM_CHEMICAL_RECIPES) {
        WidgetBuffer_Right.setVisible(true);
        PumpSetupWidget* rightPumpWidgets[] = { &pumpSetupWidget_Right_1, &pumpSetupWidget_Right_2, &pumpSetupWidget_Right_3 };
        populateWidgetBuffer(rightIndex, WidgetBuffer_Right, ChemicalEnableButton_Right,
                             NameEditText_Right, NameEditText_RightBuffer, NAMEEDITTEXT_RIGHT_SIZE,
                             VolumeEditText_Right, VolumeEditText_RightBuffer, VOLUMEEDITTEXT_RIGHT_SIZE,
                             AddPump_Right, RemovePump_Right, rightPumpWidgets);
    } else {
        WidgetBuffer_Right.setVisible(false);
    }
}

// This helper function populates one of the three complete sets of widgets
void ChemicalsSetupView::populateWidgetBuffer(int pageIndex,
                                            touchgfx::Container& bufferContainer,
                                            touchgfx::ToggleButton& enableButton,
                                            touchgfx::TextAreaWithOneWildcard& nameField,
                                            touchgfx::Unicode::UnicodeChar* nameBuffer, uint16_t nameBufferSize,
                                            touchgfx::TextAreaWithOneWildcard& volumeField,
                                            touchgfx::Unicode::UnicodeChar* volumeBuffer, uint16_t volumeBufferSize,
                                            touchgfx::Button& addButton, touchgfx::Button& removeButton,
                                            PumpSetupWidget** pumpWidgets)
{
    const ChemicalRecipe_t& data = presenter->getRecipeDataForPage(pageIndex);
    const std::vector<int>& enabled_pumps = presenter->getEnabledPumpIndices();
    int8_t unit = presenter->getVolumeUnit();

    enableButton.forceState(data.is_enabled == 1);

    Unicode::strncpy(nameBuffer, data.name, nameBufferSize);
    nameField.invalidate();

    char volAnsiBuffer[20];
    snprintf(volAnsiBuffer, 20, "%.2f %s", data.total_dispense_volume, (unit == 1 ? "Oz" : "mL"));
    Unicode::strncpy(volumeBuffer, volAnsiBuffer, volumeBufferSize);
    volumeField.invalidate();

    int visiblePumpSetups = 0;
    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i) {
        pumpWidgets[i]->setAvailablePumps(enabled_pumps);
        pumpWidgets[i]->setup(i, data.pump_setups[i], unit);
        pumpWidgets[i]->setVisible(data.pump_setups[i].pump_index != -1);
        if(pumpWidgets[i]->isVisible()) visiblePumpSetups++;
    }

    addButton.setVisible(visiblePumpSetups < MAX_PUMP_SETUPS_PER_CHEMICAL);
    removeButton.setVisible(visiblePumpSetups > 1);

    bufferContainer.invalidate();
}

void ChemicalsSetupView::handleClickEvent(const touchgfx::ClickEvent& event)
{
    ChemicalsSetupViewBase::handleClickEvent(event);
    if (event.getType() == touchgfx::ClickEvent::RELEASED) {
        int newPageIndex = swipeContainer1.getSelectedPage();
        if (newPageIndex != currentPageIndex) {
            currentPageIndex = newPageIndex;
            presenter->ActiveFieldIndexUpdate(currentPageIndex);
            updateVisiblePages();
        }
    }
}

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
//    // This function forces the single, reusable toggle button to redraw itself,
//    // ensuring its visual state (on/off) is up to date.
//    ChemicalEnableButton.invalidate();
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
