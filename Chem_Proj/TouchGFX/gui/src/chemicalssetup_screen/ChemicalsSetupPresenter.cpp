#include <gui/chemicalssetup_screen/ChemicalsSetupView.hpp>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>

ChemicalsSetupPresenter::ChemicalsSetupPresenter(ChemicalsSetupView& v)
    : view(v)
{

}

void ChemicalsSetupPresenter::activate()
{

}

void ChemicalsSetupPresenter::deactivate()
{

}

void ChemicalsSetupPresenter::loadScreenData(int page_index)
{
    // 1. Get the specific recipe data from the Model.
    //    These are the functions we just wrote in the Model.
    const ChemicalRecipe_t& recipe = model->getRecipeData(page_index);
    std::vector<int> enabled_pumps = model->getEnabledPumpIndices();
    int8_t unit = model->getVolumeUnit();

    // 2. Pass all three pieces of information to the View to be displayed.
    //    We will create the 'displayData' function in the View in the next step.
    view.displayData(recipe, enabled_pumps, unit);
}

void ChemicalsSetupPresenter::savePumpSetupData(int page_index, int setup_index, const PumpSetup_t& data)
{
    // The Presenter's job is simple: just pass the command to the Model.
    model->updatePumpSetup(page_index, setup_index, data);
}
