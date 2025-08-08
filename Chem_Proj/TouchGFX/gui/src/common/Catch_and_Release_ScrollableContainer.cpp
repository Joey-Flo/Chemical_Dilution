#include <gui/common/Catch_and_Release_ScrollableContainer.hpp>

Catch_and_Release_ScrollableContainer::Catch_and_Release_ScrollableContainer()
{
}

void Catch_and_Release_ScrollableContainer::handleDragEvent(const touchgfx::DragEvent& evt)
{
    // --- THIS IS THE SMART LOGIC ---

    // First, check if we CAN scroll vertically.
    // getContainedArea().height is the total height of all our child widgets.
    // getHeight() is the visible height of this container.
    if (getContainedArea().height > getHeight())
    {
        // If our content is TALLER than our visible area, it means we have
        // something to scroll. In this case, we act like a normal scroll container.
        touchgfx::ScrollableContainer::handleDragEvent(evt);
    }
    // ELSE: Our content fits entirely within our visible area. We have nothing to scroll.
    // By doing nothing here, we effectively "release" the touch event, allowing
    // our parent container to handle the drag instead.
}
