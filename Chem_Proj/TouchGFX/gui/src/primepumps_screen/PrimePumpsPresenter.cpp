#include <gui/primepumps_screen/PrimePumpsView.hpp>
#include <gui/primepumps_screen/PrimePumpsPresenter.hpp>
extern "C" {
#include "PumpManager.h"
}

PrimePumpsPresenter::PrimePumpsPresenter(PrimePumpsView& v)
    : view(v)
{

}

void PrimePumpsPresenter::activate()
{

}

void PrimePumpsPresenter::deactivate()
{

}

void PrimePumpsPresenter::PumpActivate(uint8_t index)
{
	Prime_Pump_Start(index);
}

bool PrimePumpsPresenter::isPumpEnabled(int pumpIndex) const
{
    return (model->getPumpEnableState(pumpIndex) == 1);
}
