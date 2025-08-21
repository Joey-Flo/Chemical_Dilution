#ifndef CLEANSYSTEMVIEW_HPP
#define CLEANSYSTEMVIEW_HPP

#include <gui_generated/cleansystem_screen/CleanSystemViewBase.hpp>
#include <gui/cleansystem_screen/CleanSystemPresenter.hpp>

class CleanSystemView : public CleanSystemViewBase
{
public:
    CleanSystemView();
    virtual ~CleanSystemView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CLEANSYSTEMVIEW_HPP
