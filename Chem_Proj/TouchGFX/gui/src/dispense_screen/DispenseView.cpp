#include <gui/dispense_screen/DispenseView.hpp>
#include <cstring>

DispenseView::DispenseView() :
currentPageIndex(0)
{

}

void DispenseView::setupScreen()
{
    DispenseViewBase::setupScreen();
    currentPageIndex = swipeContainer1.getSelectedPage();
}

void DispenseView::tearDownScreen()
{
    DispenseViewBase::tearDownScreen();
}


touchgfx::Container* DispenseView::getPageContainerForIndex(int index)
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


void DispenseView::updateVisiblePages()
{
    // --- BUFFER VISIBILITY ---
    WidgetBuffer_Center.setVisible(true);
    WidgetBuffer_Left.setVisible(currentPageIndex > 0);
    WidgetBuffer_Right.setVisible(currentPageIndex < NUM_CHEMICAL_RECIPES - 1);

    // --- LOGIC FOR THE CENTER PAGE ---
    {
        const ChemicalRecipe_t& centerData = presenter->getRecipeDataForPage(currentPageIndex);
        Container* parentPage = getPageContainerForIndex(currentPageIndex);
        if (parentPage) {
            // Move the buffer to the correct page container
            if (WidgetBuffer_Center.getParent() != parentPage) {
                // --- THIS IS THE FIX ---
                if(WidgetBuffer_Center.getParent()) {
                    static_cast<Container*>(WidgetBuffer_Center.getParent())->remove(WidgetBuffer_Center);
                }
                parentPage->add(WidgetBuffer_Center);
            }

            // Populate the Center widgets
            // --- THIS IS THE FIX ---
            Unicode::strncpy(ChemicalLabel_CenterBuffer, centerData.name, CHEMICALLABEL_CENTER_SIZE);
            if (centerData.is_enabled) {
                EnabledText_Center.setVisible(true);
                DisabledText_Center.setVisible(false);
                DisabledOverlays_Center.setVisible(false);
            } else {
                EnabledText_Center.setVisible(false);
                DisabledText_Center.setVisible(true);
                DisabledOverlays_Center.setVisible(true);
            }
            parentPage->invalidate();
        }
    }

    // --- LOGIC FOR THE LEFT PAGE ---
    if (currentPageIndex > 0) {
        const ChemicalRecipe_t& leftData = presenter->getRecipeDataForPage(currentPageIndex - 1);
        Container* parentPage = getPageContainerForIndex(currentPageIndex - 1);
        if (parentPage) {
            if (WidgetBuffer_Left.getParent() != parentPage) {
                // --- THIS IS THE FIX ---
                if(WidgetBuffer_Left.getParent()) {
                    static_cast<Container*>(WidgetBuffer_Left.getParent())->remove(WidgetBuffer_Left);
                }
                parentPage->add(WidgetBuffer_Left);
            }

            // --- THIS IS THE FIX ---
            Unicode::strncpy(ChemicalLabel_LeftBuffer, leftData.name, CHEMICALLABEL_LEFT_SIZE);
            if (leftData.is_enabled) {
                EnabledText_Left.setVisible(true);
                DisabledText_Left.setVisible(false);
                DisabledOverlays_Left.setVisible(false);
            } else {
                EnabledText_Left.setVisible(false);
                DisabledText_Left.setVisible(true);
                DisabledOverlays_Left.setVisible(true);
            }
            parentPage->invalidate();
        }
    }

    // --- LOGIC FOR THE RIGHT PAGE ---
    if (currentPageIndex < NUM_CHEMICAL_RECIPES - 1) {
        const ChemicalRecipe_t& rightData = presenter->getRecipeDataForPage(currentPageIndex + 1);
        Container* parentPage = getPageContainerForIndex(currentPageIndex + 1);
        if (parentPage) {
            if (WidgetBuffer_Right.getParent() != parentPage) {
                // --- THIS IS THE FIX ---
                if(WidgetBuffer_Right.getParent()) {
                    static_cast<Container*>(WidgetBuffer_Right.getParent())->remove(WidgetBuffer_Right);
                }
                parentPage->add(WidgetBuffer_Right);
            }

            // --- THIS IS THE FIX ---
            Unicode::strncpy(ChemicalLabel_RightBuffer, rightData.name, CHEMICALLABEL_RIGHT_SIZE);
            if (rightData.is_enabled) {
                EnabledText_Right.setVisible(true);
                DisabledText_Right.setVisible(false);
                DisabledOverlays_Right.setVisible(false);
            } else {
                EnabledText_Right.setVisible(false);
                DisabledText_Right.setVisible(true);
                DisabledOverlays_Right.setVisible(true);
            }
            parentPage->invalidate();
        }
    }
}

void DispenseView::populateWidgetBuffer(int pageIndex,
                                      touchgfx::TextAreaWithOneWildcard& chemLabel,
                                      touchgfx::TextArea& enabledText,
                                      touchgfx::TextArea& disabledText,
                                      touchgfx::Container& overlay)
{
    const ChemicalRecipe_t& data = presenter->getRecipeDataForPage(pageIndex);

//    Unicode::strncpy(chemLabel.getWildcard1Buffer(), data.name, chemLabel.getWildcard1BufferSize());
    chemLabel.invalidate();

    if (data.is_enabled) {
        enabledText.setVisible(true);
        disabledText.setVisible(false);
        overlay.setVisible(false);
    } else {
        enabledText.setVisible(false);
        disabledText.setVisible(true);
        overlay.setVisible(true);
    }
    enabledText.invalidate();
    disabledText.invalidate();
    overlay.invalidate();
}

void DispenseView::handleTickEvent()
{
    // On every tick, check if the swipe container's page has changed.
    int newPageIndex = swipeContainer1.getSelectedPage();
    if (newPageIndex != currentPageIndex)
    {
        // The page has changed.
        currentPageIndex = newPageIndex;

        // Command the Presenter to load the data for the new page.
        presenter->loadScreenData(currentPageIndex);
    }
}

void DispenseView::requestDataLoad()
{
    // This is called by the Presenter's activate() to load the initial page.
    presenter->loadScreenData(currentPageIndex);
}

// --- THIS IS THE NEW DISPLAY FUNCTION ---
void DispenseView::displayData(const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit)
{
    // --- BUFFER VISIBILITY (Identical logic) ---
    WidgetBuffer_Center.setVisible(true);
    WidgetBuffer_Left.setVisible(currentPageIndex > 0);
    WidgetBuffer_Right.setVisible(currentPageIndex < NUM_CHEMICAL_RECIPES - 1);

    // --- LOGIC FOR THE LEFT PAGE (Identical logic) ---
    if (currentPageIndex > 0) {
        const ChemicalRecipe_t& leftData = presenter->getRecipeDataForPage(currentPageIndex - 1);
        Container* parentPage = getPageContainerForIndex(currentPageIndex - 1);
        if (parentPage) {
            // Move the Left widget buffer to the previous page
            if (WidgetBuffer_Left.getParent() != parentPage) {
                if(WidgetBuffer_Left.getParent()) static_cast<Container*>(WidgetBuffer_Left.getParent())->remove(WidgetBuffer_Left);
                parentPage->add(WidgetBuffer_Left);
            }

            // Populate the Left widgets
            Unicode::strncpy(ChemicalLabel_LeftBuffer, leftData.name, CHEMICALLABEL_LEFT_SIZE);
            if (leftData.is_enabled) {
                EnabledText_Left.setVisible(true);
                DisabledText_Left.setVisible(false);
                DisabledOverlays_Left.setVisible(false);
            } else {
                EnabledText_Left.setVisible(false);
                DisabledText_Left.setVisible(true);
                DisabledOverlays_Left.setVisible(true);
            }
            parentPage->invalidate();
        }
    }

    // --- LOGIC FOR THE RIGHT PAGE (Identical logic) ---
    if (currentPageIndex < NUM_CHEMICAL_RECIPES - 1) {
        const ChemicalRecipe_t& rightData = presenter->getRecipeDataForPage(currentPageIndex + 1);
        Container* parentPage = getPageContainerForIndex(currentPageIndex + 1);
        if (parentPage) {
            // Move the Right widget buffer to the next page
            if (WidgetBuffer_Right.getParent() != parentPage) {
                if(WidgetBuffer_Right.getParent()) static_cast<Container*>(WidgetBuffer_Right.getParent())->remove(WidgetBuffer_Right);
                parentPage->add(WidgetBuffer_Right);
            }

            // Populate the Right widgets
            Unicode::strncpy(ChemicalLabel_RightBuffer, rightData.name, CHEMICALLABEL_RIGHT_SIZE);
            if (rightData.is_enabled) {
                EnabledText_Right.setVisible(true);
                DisabledText_Right.setVisible(false);
                DisabledOverlays_Right.setVisible(false);
            } else {
                EnabledText_Right.setVisible(false);
                DisabledText_Right.setVisible(true);
                DisabledOverlays_Right.setVisible(true);
            }
            parentPage->invalidate();
        }
    }

    // --- LOGIC FOR THE CENTER PAGE (Identical logic) ---
    Container* parentPage = getPageContainerForIndex(currentPageIndex);
    if (parentPage) {
        // Move the Center widget buffer to the current page
        if (WidgetBuffer_Center.getParent() != parentPage) {
            if(WidgetBuffer_Center.getParent()) static_cast<Container*>(WidgetBuffer_Center.getParent())->remove(WidgetBuffer_Center);
            parentPage->add(WidgetBuffer_Center);
        }

        // Populate the Center widgets with the data passed into the function
        Unicode::strncpy(ChemicalLabel_CenterBuffer, data.name, CHEMICALLABEL_CENTER_SIZE);
        if (data.is_enabled) {
            EnabledText_Center.setVisible(true);
            DisabledText_Center.setVisible(false);
            DisabledOverlays_Center.setVisible(false);
        } else {
            EnabledText_Center.setVisible(false);
            DisabledText_Center.setVisible(true);
            DisabledOverlays_Center.setVisible(true);
        }
        parentPage->invalidate();
    }
}

void DispenseView::lightButtonClicked()  { presenter->startDispense(0); }
void DispenseView::mediumButtonClicked() { presenter->startDispense(1); }
void DispenseView::heavyButtonClicked()  { presenter->startDispense(2); }
