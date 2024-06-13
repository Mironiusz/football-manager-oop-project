#include <iostream>
#include <typeinfo>

#include "EventManager.h"

EventManager* EventManager::getPointer() {
    return this;
}

/*
Method registerEventObject():

Registers an event object to the event manager.
*/
void EventManager::registerEventObject(EventObject* eventObject) {
    eventObjects.push_back(eventObject);
}


/*
Method unregisterEventObject():

Unregisters an event object from the event manager.
*/
void EventManager::unregisterEventObject(EventObject* eventObject) {
    for (auto it = eventObjects.begin(); it != eventObjects.end(); it++) {
        if (*it == eventObject) {
            eventObjects.erase(it);
            break;
        }
    }
}

/*
Method passEvent():

Passes an event to all event objects.
*/
void EventManager::passEvent(Event* event) {
    for (auto& eventObject : eventObjects)
        eventObject->handleEvent(event);
}