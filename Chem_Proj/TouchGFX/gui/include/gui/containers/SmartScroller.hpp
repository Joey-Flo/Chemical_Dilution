#ifndef SMARTSCROLLER_HPP
#define SMARTSCROLLER_HPP

#include <gui/common/Catch_and_Release_ScrollableContainer.hpp>

class SmartScroller : public Catch_and_Release_ScrollableContainer
{
public:
    SmartScroller();
    virtual ~SmartScroller() {}

    // We must declare this function because the auto-generated code
    // that uses this container will call it.
    virtual void initialize();
};

#endif // SMARTSCROLLER_HPP
