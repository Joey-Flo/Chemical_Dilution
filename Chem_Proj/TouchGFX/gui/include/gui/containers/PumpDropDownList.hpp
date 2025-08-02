#ifndef PUMPDROPDOWNLIST_HPP
#define PUMPDROPDOWNLIST_HPP

#include <gui_generated/containers/PumpDropdownListBase.hpp>
#include <gui/containers/PumpOption.hpp>
#include <touchgfx/Callback.hpp>
#include <vector>

// FIX: Add the inheritance from the Base class
class PumpDropdownList : public PumpDropDownListBase
{
public:
    PumpDropdownList();
    virtual ~PumpDropdownList() {}

    void buildList(const std::vector<int>& enabled_pump_indices);
    void setOptionSelectedCallback(touchgfx::GenericCallback<int>& callback);

protected:
    void pumpOptionClickedCallbackHandler(int pumpIndex);

private:
    PumpOption* pumpOptions[8];
    touchgfx::GenericCallback<int>* optionSelectedCallback;
    touchgfx::Callback<PumpDropdownList, int> childClickCallback;
};

#endif // PUMPDROPDOWNLIST_HPP
