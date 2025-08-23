#include <gui/homescreen_screen/HomeScreenView.hpp>
#include <gui/homescreen_screen/HomeScreenPresenter.hpp>

extern "C" {
#include "hx711.h"
}

extern hx711_t my_scale;

HomeScreenPresenter::HomeScreenPresenter(HomeScreenView& v)
    : view(v)
{

}

void HomeScreenPresenter::activate()
{

}

void HomeScreenPresenter::deactivate()
{

}

void HomeScreenPresenter::checkAndCorrectTare()
{
    // 1. Get a stable, tared reading from the scale.
    // We average a few samples for stability.
    long raw_value = hx711_value_ave(&my_scale, 1);
    long tared_value = raw_value - my_scale.offset;

    // 2. Get the calibration factor from the model.
    float factor = model->getScaleCalibrationFactor();
    if (factor == 0.0f) {
        return; // Do nothing if not calibrated
    }

    // 3. Calculate the actual current weight in grams.
    float current_weight_grams = (float)tared_value / factor;

    // 4. Check if the weight is negative (with a small tolerance).
    // If the scale reads -0.5g or less, it's a clear sign of a tare error.
    if (current_weight_grams < -1.0f)
    {
        // 5. If it's negative, command a re-tare.
        hx711_tare(&my_scale, 1);
    }
}
