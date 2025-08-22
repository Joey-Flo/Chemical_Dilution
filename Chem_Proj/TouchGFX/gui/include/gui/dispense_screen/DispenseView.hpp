#ifndef DISPENSEVIEW_HPP
#define DISPENSEVIEW_HPP

#include <gui_generated/dispense_screen/DispenseViewBase.hpp>
#include <gui/dispense_screen/DispensePresenter.hpp>

namespace touchgfx { class ClickEvent; }

class DispenseView : public DispenseViewBase
{
public:
    DispenseView();
    virtual ~DispenseView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void updateVisiblePages();
    void lightButtonClicked();
    void mediumButtonClicked();
    void heavyButtonClicked();
    virtual void handleTickEvent();
    void requestDataLoad();
    void displayData(const ChemicalRecipe_t& data, const std::vector<int>& enabled_pumps, int8_t unit);
    void populateWidgetBuffer(int pageIndex,
                              touchgfx::TextAreaWithOneWildcard& chemLabel,
                              touchgfx::TextArea& enabledText,
                              touchgfx::TextArea& disabledText,
                              touchgfx::Container& overlay);
    Container* getPageContainerForIndex(int index);
    int currentPageIndex;
protected:
};

#endif // DISPENSEVIEW_HPP
