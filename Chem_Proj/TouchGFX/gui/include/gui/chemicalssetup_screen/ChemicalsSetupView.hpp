#ifndef CHEMICALSSETUPVIEW_HPP
#define CHEMICALSSETUPVIEW_HPP

#include <gui_generated/chemicalssetup_screen/ChemicalsSetupViewBase.hpp>
#include <gui/chemicalssetup_screen/ChemicalsSetupPresenter.hpp>

class ChemicalsSetupView : public ChemicalsSetupViewBase
{
public:
    ChemicalsSetupView();
    virtual ~ChemicalsSetupView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CHEMICALSSETUPVIEW_HPP
