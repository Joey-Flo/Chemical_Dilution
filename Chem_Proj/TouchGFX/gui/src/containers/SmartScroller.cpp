#include <gui/containers/SmartScroller.hpp>

SmartScroller::SmartScroller()
{
    // The constructor can be empty.
}

// --- THIS IS THE FIX ---
// The framework calls this function, but since our parent class
// doesn't have one and we have no special setup to do,
// the function body is simply empty.
void SmartScroller::initialize()
{
    // Do nothing.
}
