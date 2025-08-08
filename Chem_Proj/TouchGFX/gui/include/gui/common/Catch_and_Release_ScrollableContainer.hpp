#ifndef CATCH_AND_RELEASE_SCROLLABLE_CONTAINER_HPP
#define CATCH_AND_RELEASE_SCROLLABLE_CONTAINER_HPP

#include <touchgfx/containers/ScrollableContainer.hpp>

class Catch_and_Release_ScrollableContainer : public touchgfx::ScrollableContainer
{
public:
    Catch_and_Release_ScrollableContainer();

    // We will override the default touch event handler
    virtual void handleDragEvent(const touchgfx::DragEvent& evt);
};

#endif // CATCH_AND_RELEASE_SCROLLABLE_CONTAINER_HPP
