#include <gui/homescreen_screen/HomeScreenView.hpp>
#include <gui/common/CustomKeyboard.hpp>

HomeScreenView::HomeScreenView()
{

}

void HomeScreenView::setupScreen()
{
    HomeScreenViewBase::setupScreen();
}

void HomeScreenView::tearDownScreen()
{
    HomeScreenViewBase::tearDownScreen();
}

void HomeScreenView::handleTickEvent()
{
    // To avoid checking too frequently, we can use a simple counter.
    // This will check the scale roughly 10 times per second (60 / 6 = 10).
    static int tickCounter = 0;
    if (++tickCounter % 60 == 0)
    {
        // On every 6th tick, tell the presenter to run its check.
        presenter->checkAndCorrectTare();
    }
}
