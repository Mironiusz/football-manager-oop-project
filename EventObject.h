#pragma once

#include "EventManager.h"
#include "Event.h"

class EventManager;
class Event;

/*
Class EventObject:

Abstract class for other classes to inherit from.
Allows to send events to the event manager and handle them.

Attributes:
- eventManager: event manager to send events to
*/
class EventObject {
    private:
        EventManager* eventManager = nullptr;

    public:
        EventObject() = default;

        EventObject(EventManager* em);

        virtual ~EventObject();

        EventManager* getEventManager();

        void connectToEventManager();

        void disconnectFromEventManager();

        void sendEvent(Event* event);

        virtual void handleEvent(Event* event) = 0;
};