#ifndef PRIMEPUMPSPRESENTER_HPP
#define PRIMEPUMPSPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class PrimePumpsView;

class PrimePumpsPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    PrimePumpsPresenter(PrimePumpsView& v);

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

    virtual ~PrimePumpsPresenter() {}
    void PumpActivate(uint8_t index);
    bool isPumpEnabled(int pumpIndex) const;

private:
    PrimePumpsPresenter();

    PrimePumpsView& view;
};

#endif // PRIMEPUMPSPRESENTER_HPP
