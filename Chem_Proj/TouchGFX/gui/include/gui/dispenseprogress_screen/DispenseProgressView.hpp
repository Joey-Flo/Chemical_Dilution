#ifndef DISPENSEPROGRESSVIEW_HPP
#define DISPENSEPROGRESSVIEW_HPP

#include <gui_generated/dispenseprogress_screen/DispenseProgressViewBase.hpp>
#include <gui/dispenseprogress_screen/DispenseProgressPresenter.hpp>
#include "DispenseManager_shared_types.h"


class DispenseProgressView : public DispenseProgressViewBase
{
public:
    DispenseProgressView();
    virtual ~DispenseProgressView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    // Called by the Presenter to update the entire screen
    void updateProgress(DispenseState_e state, bool wasCancelled, int8_t recipe_index, int8_t size,
            float currentWeight, float totalTargetWeight);

    // Interaction handlers
    void cancelButtonPressed();
    void returnHomeButtonPressed();
    const char* getRecipeName(int recipe_index);
protected:
};

#endif // DISPENSEPROGRESSVIEW_HPP
