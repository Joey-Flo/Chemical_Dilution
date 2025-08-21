#include <gui/primepumps_screen/PrimePumpsView.hpp>
extern "C" {
#include "PumpManager.h"
}


PrimePumpsView::PrimePumpsView() :
    currentlyPrimingPump(-1)
{
    // These names MUST match your Designer names
    runButtons[0] = &RunButton_1;
    runButtons[1] = &RunButton_2;
    runButtons[2] = &RunButton_3;
    runButtons[3] = &RunButton_4;
    runButtons[4] = &RunButton_5;
    runButtons[5] = &RunButton_6;
    runButtons[6] = &RunButton_7;
    runButtons[7] = &RunButton_8;

    disableOverlays[0] = &disableOverlay_1;
    disableOverlays[1] = &disableOverlay_2;
    disableOverlays[2] = &disableOverlay_3;
    disableOverlays[3] = &disableOverlay_4;
    disableOverlays[4] = &disableOverlay_5;
    disableOverlays[5] = &disableOverlay_6;
    disableOverlays[6] = &disableOverlay_7;
    disableOverlays[7] = &disableOverlay_8;
}

void PrimePumpsView::setupScreen()
{
    PrimePumpsViewBase::setupScreen();

    for (int i = 0; i < 8; ++i)
    {
        pump_is_enabled[i] = presenter->isPumpEnabled(i);

        // --- THIS IS THE NEW, CLEAN LOGIC ---

        // The button's interactivity is based on the enabled state.
        runButtons[i]->setTouchable(pump_is_enabled[i]);

        // The overlay's visibility is the INVERSE of the enabled state.
        // If the pump is disabled, the overlay is visible.
        disableOverlays[i]->setVisible(!pump_is_enabled[i]);

        // Invalidate both to be safe and ensure they redraw correctly.
        runButtons[i]->invalidate();
        disableOverlays[i]->invalidate();
    }
}


void PrimePumpsView::tearDownScreen()
{
    PrimePumpsViewBase::tearDownScreen();
}



// Add this entire function to PrimePumpsView.cpp

// In PrimePumpsView.cpp

void PrimePumpsView::handleClickEvent(const touchgfx::ClickEvent& event)
{
    PrimePumpsViewBase::handleClickEvent(event);

    if (event.getType() == touchgfx::ClickEvent::PRESSED)
    {
        for (int i = 0; i < 8; ++i)
        {
            touchgfx::Rect clickRect(event.getX(), event.getY(), 1, 1);
            if (runButtons[i]->getAbsoluteRect().intersect(clickRect))
            {
                if (pump_is_enabled[i])
                {
                    Prime_Pump_Start(i);
                    currentlyPrimingPump = i;

                    // --- THIS IS THE FIX ---
                    // To make the button visually press down, we must cast the pointer
                    // back to an AbstractButton to call its handleClickEvent
                    static_cast<touchgfx::AbstractButton*>(runButtons[i])->handleClickEvent(event);
                }
                return;
            }
        }
    }
    else if (event.getType() == touchgfx::ClickEvent::RELEASED)
    {
        if (currentlyPrimingPump != -1)
        {
            Prime_Pump_Stop(currentlyPrimingPump);

            // --- THIS IS THE FIX ---
            // Cast the pointer to make the button visually release
            static_cast<touchgfx::AbstractButton*>(runButtons[currentlyPrimingPump])->handleClickEvent(event);

            currentlyPrimingPump = -1;
        }
    }
}
