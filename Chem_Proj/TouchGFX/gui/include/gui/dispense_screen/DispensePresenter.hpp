#ifndef DISPENSEPRESENTER_HPP
#define DISPENSEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include "shared_types.h"
#include <vector>

using namespace touchgfx;

class DispenseView;

class DispensePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DispensePresenter(DispenseView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    const ChemicalRecipe_t& getRecipeDataForPage(int page_index) const;
    void currentPageChanged(int page_index);
    void startDispense(int dispense_size);
    void loadScreenData(int page_index);

    virtual ~DispensePresenter() {}

private:
    DispensePresenter();

    DispenseView& view;
    int activePageIndex;
};

#endif // DISPENSEPRESENTER_HPP
