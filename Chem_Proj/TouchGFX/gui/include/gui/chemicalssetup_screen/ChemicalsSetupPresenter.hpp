#ifndef CHEMICALSSETUPPRESENTER_HPP
#define CHEMICALSSETUPPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ChemicalsSetupView;

class ChemicalsSetupPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ChemicalsSetupPresenter(ChemicalsSetupView& v);

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

    virtual ~ChemicalsSetupPresenter() {}

    void loadScreenData(int page_index);

    void savePumpSetupData(int page_index, int setup_index, const PumpSetup_t& data);

    void editField(int fieldID);

    // We will still need a function to save the data later
    void newValueEntered(const char* text);

private:
    int currentlyEditingFieldID;

    // We still need these to remember the context for S/M/L fields
    int activePageIndex;
    int activeSetupIndex;
    int activeFieldIndex;

    ChemicalsSetupPresenter();

    ChemicalsSetupView& view;
};

#endif // CHEMICALSSETUPPRESENTER_HPP
