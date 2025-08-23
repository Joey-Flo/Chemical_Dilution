#include <gui/cleansystem_screen/CleanSystemView.hpp>
#include "main.h"
extern uint8_t lastSystemReadyState;

CleanSystemView::CleanSystemView()
{

}

void CleanSystemView::setupScreen()
{
    CleanSystemViewBase::setupScreen();
    tickVal = 0;
    setSystemReady(false);
}

void CleanSystemView::tearDownScreen()
{
    CleanSystemViewBase::tearDownScreen();
}

// The tick handler drives the continuous check
void CleanSystemView::handleTickEvent()
{
    // Every 10 ticks (6 times a second), ask the presenter to check the scale
    static int tickCounter = 0;
    if (++tickCounter % 10 == 0)
    {
        presenter->checkScaleStatus();
    }
    tickVal++;
}

// The function the Presenter calls to update our state
void CleanSystemView::setSystemReady(bool isReady)
{
    // --- THIS IS THE NEW LOGIC ---


    // Update the UI elements based on the state
    DisabledOverlay.setVisible(!isReady);
    ContainerPromptText.setVisible(!isReady); // The "Place container..." text
    ReadyText.setVisible(isReady); // The "Ready to clean" text

    // Invalidate all widgets to force a redraw
    DisabledOverlay.invalidate();
    ContainerPromptText.invalidate();
    ReadyText.invalidate();

    if ((isReady != lastSystemReadyState) && (tickVal > 10))
    {
        if (isReady)
        {
            PlaySound(2);
        }
        else
        {
            PlaySound(3);
        }
    }

    // Update the current state and the last known state for the next check.
    systemIsReady = isReady;
    lastSystemReadyState = isReady;
}

// The event handler for the "press and hold" button
void CleanSystemView::handleClickEvent(const touchgfx::ClickEvent& event)
{
    CleanSystemViewBase::handleClickEvent(event);

    if (systemIsReady) // Only process clicks if the system is ready
    {
        // Check if the event is on our Flush button
        touchgfx::Rect clickRect(event.getX(), event.getY(), 1, 1);
        if (FlushButton.getAbsoluteRect().intersect(clickRect))
        {
            if (event.getType() == touchgfx::ClickEvent::PRESSED)
            {
                // Finger is down, start the flush
                presenter->startFlush();
            }
            else if (event.getType() == touchgfx::ClickEvent::RELEASED)
            {
                // Finger is up, stop the flush
                presenter->stopFlush();
            }
        }
    }
}
