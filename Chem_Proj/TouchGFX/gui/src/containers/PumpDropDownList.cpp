#include <gui/containers/PumpDropDownList.hpp>
#include <cstdio>

PumpDropDownList::PumpDropDownList() :
    optionSelectedCallback(nullptr),
    childClickCallback(this, &PumpDropDownList::pumpOptionClickedCallbackHandler)
{
    // This will now work because of the inheritance in the header file
    pumpOptions[0] = &pumpOption0;
    pumpOptions[1] = &pumpOption1;
    pumpOptions[2] = &pumpOption2;
    pumpOptions[3] = &pumpOption3;
    pumpOptions[4] = &pumpOption4;
    pumpOptions[5] = &pumpOption5;
    pumpOptions[6] = &pumpOption6;
    pumpOptions[7] = &pumpOption7;
}

void PumpDropDownList::buildList(const std::vector<int>& enabled_pump_indices)
{
    for (int i = 0; i < 8; ++i)
    {
        pumpOptions[i]->setVisible(false);
    }

    int visible_option_count = 0;
    for (int pump_index : enabled_pump_indices)
    {
        if (visible_option_count < 8)
        {
            PumpOption* option_widget = pumpOptions[visible_option_count];

            char name_buffer[10];
            snprintf(name_buffer, 10, "Pump %d", pump_index + 1);

            option_widget->setup(pump_index, name_buffer);
            option_widget->setVisible(true);
            option_widget->setClickCallback(childClickCallback);

            visible_option_count++;
        }
    }

    // This will now work because of the inheritance in the header file
    scrollableContainer.invalidate();
}

void PumpDropDownList::setOptionSelectedCallback(touchgfx::GenericCallback<int>& callback)
{
    optionSelectedCallback = &callback;
}

void PumpDropDownList::pumpOptionClickedCallbackHandler(int pumpIndex)
{
    if (optionSelectedCallback && optionSelectedCallback->isValid())
    {
        optionSelectedCallback->execute(pumpIndex);
    }
}
