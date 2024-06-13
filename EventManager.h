#pragma once

#include <vector>

#include "EventObject.h"
#include "Event.h"

class EventObject;
class Event;


/*
Class EventManager:

Responsible for managing events and passing them to event objects.

Attributes:
- eventObjects: list of event objects
*/
class EventManager {

    private:

        std::vector<EventObject*> eventObjects;

    public:
        EventManager() = default;
        ~EventManager() = default;

        EventManager* getPointer();

        void registerEventObject(EventObject* eventObject);

        void unregisterEventObject(EventObject* eventObject);

        void passEvent(Event* event);
};