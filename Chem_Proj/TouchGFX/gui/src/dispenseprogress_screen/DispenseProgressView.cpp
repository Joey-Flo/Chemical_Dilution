#include <gui/dispenseprogress_screen/DispenseProgressView.hpp>
#include <cstdio>
#include "shared_types.h"

DispenseProgressView::DispenseProgressView()
{

}

void DispenseProgressView::setupScreen()
{
    DispenseProgressViewBase::setupScreen();
}

void DispenseProgressView::tearDownScreen()
{
    DispenseProgressViewBase::tearDownScreen();
}

void DispenseProgressView::handleTickEvent()
{
    // Every 6 ticks (10 times a second), ask the presenter for an update
    static int tickCounter = 0;
    if (++tickCounter % 6 == 0) {
        presenter->updateState();
    }
}

void DispenseProgressView::updateProgress(DispenseState_e state, int8_t recipe_index, int8_t size,
                                        float currentWeight, float totalTargetWeight)
{
    bool isDone = (state == DISPENSE_STATE_DONE);
    bool isCancelled = (state == DISPENSE_STATE_CANCELLED);
    bool isIdle = (state == DISPENSE_STATE_IDLE);

    // --- Manage Visibility ---
    ContainerPromptText.setVisible(state == DISPENSE_STATE_WAITING_FOR_CONTAINER);
    DispensingText.setVisible(state > DISPENSE_STATE_TARING && !isDone && !isCancelled);
    DispenseCompleteText.setVisible(isDone || isCancelled);
    ProgressIndicators.setVisible(state > DISPENSE_STATE_TARING && !isDone && !isCancelled);
    CancelButton.setVisible(!isDone && !isCancelled && !isIdle);
    ReturnHomeButton.setVisible(isDone || isCancelled);

    // --- Update Text Wildcards ---
    if (DispensingText.isVisible()) {
        const char* sizeStr = (size == 0) ? "Light" : (size == 1) ? "Medium" : "Heavy";
        Unicode::strncpy(DispensingTextBuffer1, sizeStr, DISPENSINGTEXTBUFFER1_SIZE);
        Unicode::strncpy(DispensingTextBuffer2, getRecipeName(recipe_index), DISPENSINGTEXTBUFFER2_SIZE);
    }


    // --- Update Progress Bar ---
    if (ProgressIndicators.isVisible()) {
        int percentage = 0;
        if (totalTargetWeight > 0) {
            percentage = static_cast<int>((currentWeight / totalTargetWeight) * 100.0f);
        }
        if (percentage > 100) percentage = 100;
        if (percentage < 0) percentage = 0;

        circleProgress1.setValue(percentage);
        Unicode::snprintf(WildcardTextProgressBuffer, WILDCARDTEXTPROGRESS_SIZE, "%d", percentage);
    }

    // Invalidate everything that might have changed
    ContainerPromptText.invalidate();
    DispensingText.invalidate();
    DispenseCompleteText.invalidate();
    ProgressIndicators.invalidate();
    CancelButton.invalidate();
    ReturnHomeButton.invalidate();
}

// Helper to get recipe name
const char* DispenseProgressView::getRecipeName(int recipe_index)
{
    // This is not efficient, but it's the simplest way.
    // It asks the presenter for the data just to get the name.
    return presenter->getRecipeDataForPage(recipe_index).name;
}

// Interaction Handlers
void DispenseProgressView::cancelButtonPressed() { presenter->cancelButtonPressed(); }
void DispenseProgressView::returnHomeButtonPressed() {
    // The view handles its own screen transitions
    application().gotoHomeScreenScreenNoTransition();
}
