#ifndef SCALECALIBRATIONVIEW_HPP
#define SCALECALIBRATIONVIEW_HPP

#include <gui_generated/scalecalibration_screen/ScaleCalibrationViewBase.hpp>
#include <gui/scalecalibration_screen/ScaleCalibrationPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>

class ScaleCalibrationView : public ScaleCalibrationViewBase
{
public:
    ScaleCalibrationView();
    virtual ~ScaleCalibrationView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    void EnterPressed();
    void ExitPressed();
    void TareScale();
    void ScaleCalibrate();
protected:

    CustomKeyboard keyboard;

};

#endif // SCALECALIBRATIONVIEW_HPP
