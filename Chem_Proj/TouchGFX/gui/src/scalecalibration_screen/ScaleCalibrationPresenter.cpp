#include <gui/scalecalibration_screen/ScaleCalibrationView.hpp>
#include <gui/scalecalibration_screen/ScaleCalibrationPresenter.hpp>
#include "main.h"
extern "C" {
#include "hx711.h"
}

extern hx711_t my_scale;

ScaleCalibrationPresenter::ScaleCalibrationPresenter(ScaleCalibrationView& v)
    : view(v)
{

}

void ScaleCalibrationPresenter::activate()
{

}

void ScaleCalibrationPresenter::deactivate()
{

}

void ScaleCalibrationPresenter::calculateNewCalibrationFactor(const char* knownWeightText)
{
    // 1. Convert the user's text input to a number
    char* end_ptr;
    float known_weight = strtof(knownWeightText, &end_ptr);

    // 2. Validate the input. It must be a valid number greater than zero.
    if (*end_ptr != '\0' || end_ptr == knownWeightText || known_weight <= 0.0f) {
        // Input was invalid, do nothing.
        return;
    }

    // 3. Get a stable, RAW (not tared, not converted) reading from the HX711.
    //    We need the value before the tare offset is subtracted.
    //    We can get this by adding the offset back to an averaged reading.
    long raw_adc_value = hx711_value_ave(&my_scale, 10) - my_scale.offset;

    // 4. Calculate the new calibration factor.
    float new_factor = (float)raw_adc_value / known_weight;

    // 5. Command the Model to save the new factor to flash.
    model->updateScaleCalibrationFactor(new_factor);
}

float ScaleCalibrationPresenter::getLiveWeight()
{
    // This is for the live display on the screen.
        // 1. Get the current raw, tared value
        long tared_value = hx711_value_ave(&my_scale, 3) - my_scale.offset;; // Average 5 samples for stability

        // 2. Get the current calibration factor from the model
        float factor = model->getScaleCalibrationFactor();
        if (factor == 0.0f) return 0.0f; // Prevent division by zero

        // 3. Return the calculated weight in grams
        return (float)tared_value / factor;

}

void ScaleCalibrationPresenter::tareScale()
{
    PlaySound(1);
	hx711_tare(&my_scale, 10);
}
