#ifndef PRIMEPUMPSVIEW_HPP
#define PRIMEPUMPSVIEW_HPP

#include <gui_generated/primepumps_screen/PrimePumpsViewBase.hpp>
#include <gui/primepumps_screen/PrimePumpsPresenter.hpp>

namespace touchgfx { class ClickEvent; }


class PrimePumpsView : public PrimePumpsViewBase
{
public:
    PrimePumpsView();
    virtual ~PrimePumpsView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleClickEvent(const touchgfx::ClickEvent& event);
    touchgfx::Drawable* runButtons[8];
    touchgfx::Container* disableOverlays[8];
    // A state variable to remember which pump is running
    int currentlyPrimingPump;
    bool pump_is_enabled[8];
protected:
};

#endif // PRIMEPUMPSVIEW_HPP
