#include <functional>

#include "EventObject.h"


EventObject::EventObject(EventManager* em) : eventManager(em) {
    connectToEventManager();
}

EventObject::~EventObject() {
    if (eventManager != nullptr)
        disconnectFromEventManager();
}

EventManager* EventObject::getEventManager() {
    return eventManager;
}

/*
Method connectToEventManager():

Registers the event object to the event manager.
*/
void EventObject::connectToEventManager() {
    eventManager->registerEventObject(this);
}

/*
Method disconnectFromEventManager():

Unregisters the event object from the event manager.
*/
void EventObject::disconnectFromEventManager() {
    eventManager->unregisterEventObject(this);
}

/*
Method sendEvent():

Sends an event to the event manager.
*/
void EventObject::sendEvent(Event* event) {
    eventManager->passEvent(event);
}

