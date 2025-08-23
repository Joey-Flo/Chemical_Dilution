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

void DispenseProgressView::updateProgress(DispenseState_e state, bool wasCancelled, int8_t recipe_index, int8_t size,
                                        float currentWeight, float totalTargetWeight)
{
    // Determine the high-level status of the job
    bool isJobRunning = (state > DISPENSE_STATE_TARING && state < DISPENSE_STATE_DONE);
    bool isWaitingForRemoval = (state == DISPENSE_STATE_WAITING_FOR_REMOVAL || state == DISPENSE_STATE_DONE || state == DISPENSE_STATE_CANCELLED);
    bool isWaitingForContainer = (state == DISPENSE_STATE_WAITING_FOR_CONTAINER);
    bool isComplete = (state == DISPENSE_STATE_POST_JOB_TARE || state == DISPENSE_STATE_IDLE);

    // --- Manage Visibility based on the new workflow ---
    LoadingText.setVisible(false);
    ContainerPromptText.setVisible(isWaitingForContainer);
    DispensingText.setVisible(isJobRunning);
    DispenseCompleteText.setVisible(isWaitingForRemoval); // This text is no longer needed with the new prompt

    // NEW: Show the removal prompt when job is done or cancelled
    ContainerRemovePromptText.setVisible(isWaitingForRemoval);

    ProgressIndicators.setVisible(isJobRunning ||isWaitingForRemoval);
    CancelButton.setVisible(isJobRunning || isWaitingForContainer); // Can cancel while waiting for container too

    if (isWaitingForRemoval) {
        // If we are waiting for removal, check the flag to see how we got here.
        DispenseCompleteText.setVisible(!wasCancelled);
        DispenseCancelledText.setVisible(wasCancelled);
    } else {
        // If the job isn't over yet, hide both final texts.
        DispenseCompleteText.setVisible(false);
        DispenseCancelledText.setVisible(false);
    }

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
    DispenseCancelledText.invalidate();
    ContainerRemovePromptText.invalidate();
    ProgressIndicators.invalidate();
    CancelButton.invalidate();
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
