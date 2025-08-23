#include <gui/dispenseprogress_screen/DispenseProgressView.hpp>
#include <gui/dispenseprogress_screen/DispenseProgressPresenter.hpp>

extern "C" {
#include "DispenseManager.h"
#include "hx711.h"
}

extern hx711_t my_scale;
#define CONTAINER_MIN_WEIGHT_GRAMS    25.0f

DispenseProgressPresenter::DispenseProgressPresenter(DispenseProgressView& v)
    : view(v), wasCancelled(false)
{

}

void DispenseProgressPresenter::activate()
{
    wasCancelled = false;

    // Get the necessary data from the Model
    int8_t recipe_index = model->getSelectedRecipeIndex();
    int8_t dispense_size = model->getSelectedDispenseSize();

    // Initialize the C-level manager
	DispenseManager_Init();

    // --- CORRECTED LOGIC: Perform a proper weight calculation ---

    // 1. Get the current raw, tared ADC value from the scale.
    long raw_value = hx711_value_ave(&my_scale, 5);
    long tared_value = raw_value - my_scale.offset;

    // 2. Get the calibration factor from the model.
    float factor = model->getScaleCalibrationFactor();
    // Prevent a division-by-zero error if the scale is not calibrated.
    if (factor == 0.0f) {
        factor = 1.0f;
    }

    // 3. Calculate the actual current weight in grams.
    float initial_weight_grams = (float)tared_value / factor;

    // 4. Decide the entry point for the state machine based on the correct weight.
    if (initial_weight_grams >= CONTAINER_MIN_WEIGHT_GRAMS)
    {
        // A container is already present!
        // Start the job, but tell it to SKIP the container check.
        DispenseManager_StartJob_SkipContainerCheck(recipe_index, dispense_size);
    }
    else
    {
        // No container present. Start the job from the beginning.
        DispenseManager_StartJob(recipe_index, dispense_size);
    }
}

void DispenseProgressPresenter::deactivate()
{

    const DispenseJob_t* job = DispenseManager_GetJobStatus();

    if (job->state != DISPENSE_STATE_IDLE)
    {
        DispenseManager_CancelJob();
    }
}

void DispenseProgressPresenter::updateState()
{
    const DispenseJob_t* job = DispenseManager_GetJobStatus();
    float currentWeight = DispenseManager_GetCurrentWeight();

    // --- MODIFIED: Pass the new 'wasCancelled' flag to the View ---
    view.updateProgress(job->state, wasCancelled, job->recipe_index, job->dispense_size,
                        currentWeight, job->total_target_weight_grams);

    if (job->state == DISPENSE_STATE_IDLE)
    {
        static_cast<FrontendApplication*>(Application::getInstance())->gotoHomeScreenScreenNoTransition();
    }
}
void DispenseProgressPresenter::cancelButtonPressed()
{
	wasCancelled = true;
    DispenseManager_CancelJob();
}

void DispenseProgressPresenter::returnHomeButtonPressed()
{
    // The View handles the screen transition, but this is here for completeness
    // In a real app, you might do cleanup here before the transition.
}

const ChemicalRecipe_t& DispenseProgressPresenter::getRecipeDataForPage(int page_index) const
{
    // The presenter gets the data from its model and passes it back to the view.
    return model->getRecipeData(page_index);
}
