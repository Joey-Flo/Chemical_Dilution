#include <gui/dispenseprogress_screen/DispenseProgressView.hpp>
#include <gui/dispenseprogress_screen/DispenseProgressPresenter.hpp>

extern "C" {
#include "DispenseManager.h"
}

DispenseProgressPresenter::DispenseProgressPresenter(DispenseProgressView& v)
    : view(v)
{

}

void DispenseProgressPresenter::activate()
{
    int8_t recipe_index = model->getSelectedRecipeIndex();
    int8_t dispense_size = model->getSelectedDispenseSize();
	DispenseManager_Init();
    // Start the C-level dispense job with this information.
    DispenseManager_StartJob(recipe_index, dispense_size);
}

void DispenseProgressPresenter::deactivate()
{
	DispenseManager_CancelJob();
}

void DispenseProgressPresenter::updateState()
{
    // 1. Ask the C manager for the current status
    const DispenseJob_t* job = DispenseManager_GetJobStatus();
    float currentWeight = DispenseManager_GetCurrentWeight();

    // 2. Pass this information to the View to be displayed
    view.updateProgress(job->state, job->recipe_index, job->dispense_size,
                        currentWeight, job->total_target_weight_grams);
}

void DispenseProgressPresenter::cancelButtonPressed()
{
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
