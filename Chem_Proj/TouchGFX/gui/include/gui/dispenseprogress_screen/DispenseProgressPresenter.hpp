#ifndef DISPENSEPROGRESSPRESENTER_HPP
#define DISPENSEPROGRESSPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include "DispenseManager_shared_types.h"

using namespace touchgfx;

class DispenseProgressView;

class DispenseProgressPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DispenseProgressPresenter(DispenseProgressView& v);

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
    void updateState();

    // Called by the View's interaction handlers
    void cancelButtonPressed();
    void returnHomeButtonPressed();
    const ChemicalRecipe_t& getRecipeDataForPage(int page_index) const;
    virtual ~DispenseProgressPresenter() {}

private:
    DispenseProgressPresenter();
    bool wasCancelled;

    DispenseProgressView& view;
};

#endif // DISPENSEPROGRESSPRESENTER_HPP
