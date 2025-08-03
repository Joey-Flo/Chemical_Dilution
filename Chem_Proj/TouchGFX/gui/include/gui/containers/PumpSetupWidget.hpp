#ifndef PUMPSETUPWIDGET_HPP
#define PUMPSETUPWIDGET_HPP

#include <gui_generated/containers/PumpSetupWidgetBase.hpp>

class PumpSetupWidget : public PumpSetupWidgetBase
{
public:
    PumpSetupWidget();
    virtual ~PumpSetupWidget() {}

    virtual void initialize();
protected:
};

#endif // PUMPSETUPWIDGET_HPP
