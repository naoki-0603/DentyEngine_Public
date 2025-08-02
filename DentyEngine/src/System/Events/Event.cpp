#include "stdafx.h"

#include "Event.h"

namespace DentyEngine
{
    NotifyEvent::EventCallback NotifyEvent::_callback = {};

    NotifyEvent::NotifyEvent(const EventCallback& callback)
    {
        _callback = callback;
    }

    void NotifyEvent::Notify(Event* e)
    {
        _callback(e);
    }
};