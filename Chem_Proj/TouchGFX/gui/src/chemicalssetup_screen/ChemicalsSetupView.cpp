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
    pumpSetupWidgets[0] = &pumpSetupWidget1;
    pumpSetupWidgets[1] = &pumpSetupWidget2;
    pumpSetupWidgets[2] = &pumpSetupWidget3;

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
        case 0: return &scrollableContainer1;
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
    // 1. Get the parent containers for the current page
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
    moveWidget(chemicalLabel, *newParentPage);
    moveWidget(ChemicalEnableButton, *newParentPage);
    moveWidget(ChemNameText, *newParentPage);
    moveWidget(ChemVolumeText, *newParentPage);
    moveWidget(NameBox, *newParentPage);
    moveWidget(NameEditText, *newParentPage);
    moveWidget(NameEdit, *newParentPage);
    moveWidget(VolumeBox, *newParentPage);
    moveWidget(VolumeEditText, *newParentPage);
    moveWidget(VolumeEdit, *newParentPage);
    moveWidget(AddPumpBox, *newParentPage);
    moveWidget(AddPumpText, *newParentPage);
    moveWidget(AddPump, *newParentPage);
    moveWidget(RemovePump, *newParentPage);

    for (int i = 0; i < MAX_PUMP_SETUPS_PER_CHEMICAL; ++i)
    {
        moveWidget(*pumpSetupWidgets[i], *newScrollParent);
    }

    // 4. Populate the widgets with new data
    Unicode::snprintf(chemicalLabelBuffer, CHEMICALLABEL_SIZE, "Chemical %d:", currentPageIndex + 1);
    ChemicalEnableButton.forceState(data.is_enabled == 1);
    Unicode::strncpy(NameEditTextBuffer, data.name, NAMEEDITTEXT_SIZE);

    char volAnsiBuffer[20];
    const char* unit_suffix = (unit == 1) ? "Oz" : "mL";
    snprintf(volAnsiBuffer, 20, "%.2f %s", data.total_dispense_volume, unit_suffix);
    Unicode::strncpy(VolumeEditTextBuffer, volAnsiBuffer, VOLUMEEDITTEXT_SIZE);

    // 5. Configure pump widgets and Add/Remove button visibility
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

    // 6. Force a redraw of the entire page to prevent a blank screen
    newParentPage->invalidate();
}

void ChemicalsSetupView::handleClickEvent(const touchgfx::ClickEvent& event)
{
    // Let the base class handle button clicks first
    ChemicalsSetupViewBase::handleClickEvent(event);

    // After a swipe gesture, the user's finger is released, triggering this event.
    if (event.getType() == touchgfx::ClickEvent::RELEASED)
    {
        int newPageIndex = swipeContainer1.getSelectedPage();
        if (newPageIndex != currentPageIndex)
        {
            // The page has changed. Update our state and trigger a data reload.
            currentPageIndex = newPageIndex;
            presenter->loadScreenData(currentPageIndex);
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
    // This function forces the single, reusable toggle button to redraw itself,
    // ensuring its visual state (on/off) is up to date.
    ChemicalEnableButton.invalidate();
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
