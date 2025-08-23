#include <gui/dispense_screen/DispenseView.hpp>
#include <gui/dispense_screen/DispensePresenter.hpp>

DispensePresenter::DispensePresenter(DispenseView& v)
    : view(v)
{

}

void DispensePresenter::activate()
{
    view.requestDataLoad();
}

void DispensePresenter::deactivate()
{

}

const ChemicalRecipe_t& DispensePresenter::getRecipeDataForPage(int page_index) const
{
    // The presenter gets the data from its model and passes it back to the view.
    return model->getRecipeData(page_index);
}


// You also need a function for the view to update the presenter's state
void DispensePresenter::currentPageChanged(int page_index)
{
    activePageIndex = page_index;
    // We don't need to call loadScreenData here because the view's
    // handleClickEvent will do that after calling this.
}

void DispensePresenter::loadScreenData(int page_index)
{
    activePageIndex = page_index;

    const ChemicalRecipe_t& recipe = model->getRecipeData(activePageIndex);
    // You might not need these for the Dispense screen, but we pass them for consistency
    const std::vector<int> enabled_pumps = model->getEnabledPumpIndices();
    int8_t unit = model->getVolumeUnit();

    // Pass all the data to the View
    view.displayData(recipe, enabled_pumps, unit);
}

void DispensePresenter::dispenseButtonPressed(int dispense_size)
{
    // 1. The Presenter updates the Model using the new public functions.
    model->setSelectedRecipeIndex(activePageIndex);
    model->setSelectedDispenseSize(dispense_size);

    // 2. The Presenter now has access to the application instance
    //    because it inherits from touchgfx::Presenter.
    static_cast<FrontendApplication*>(Application::getInstance())->gotoDispenseProgressScreenNoTransition();
}


void DispensePresenter::startDispense(int dispense_size)
{
//    DispenseManager_StartJob(activePageIndex, dispense_size);
}
