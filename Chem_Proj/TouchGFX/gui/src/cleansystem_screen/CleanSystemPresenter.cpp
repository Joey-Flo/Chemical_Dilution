#include <gui/cleansystem_screen/CleanSystemView.hpp>
#include <gui/cleansystem_screen/CleanSystemPresenter.hpp>

extern "C" {
#include "hx711.h"
#include "ValveDriver.h"
}

extern hx711_t my_scale;

CleanSystemPresenter::CleanSystemPresenter(CleanSystemView& v)
    : view(v)
{

}

void CleanSystemPresenter::activate()
{
	Valve_Close();
}

void CleanSystemPresenter::deactivate()
{

}

void CleanSystemPresenter::checkScaleStatus()
{

    // Get the current weight in grams
    long tared_value = hx711_value_ave(&my_scale, 5) - my_scale.offset;
    float factor = model->getScaleCalibrationFactor();
    if (factor == 0.0f) return; // Not calibrated
    float current_weight = (float)tared_value / factor;

    // Now, command the view based on the weight
    if (current_weight >= 25.0f)
    {
        view.setSystemReady(true); // Container is present
    }
    else
    {
    	Valve_Close();
        view.setSystemReady(false); // Container is missing
    }
}

void CleanSystemPresenter::startFlush()
{
    Valve_Open();
}

void CleanSystemPresenter::stopFlush()
{
    Valve_Close();
}
