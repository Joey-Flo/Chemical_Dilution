#ifndef CLEANSYSTEMPRESENTER_HPP
#define CLEANSYSTEMPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class CleanSystemView;

class CleanSystemPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CleanSystemPresenter(CleanSystemView& v);

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

    virtual ~CleanSystemPresenter() {}

private:
    CleanSystemPresenter();

    CleanSystemView& view;
};

#endif // CLEANSYSTEMPRESENTER_HPP
