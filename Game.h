#pragma once

#include "EventObject.h"
#include "Event.h"

#include "Menu.h"
#include "DataManager.h"
#include "ClubManager.h"

/*
Class Game:

Responsible for handling the game. It contains the data manager, menu, and club manager. It runs the game and handles events.
Implemented as EventObject to be able to handle events.

Attributes:
- dataManager: responsible for managing data
- menu: responsible for displaying the menu and handling user input
- clubManager: responsible for managing clubs and displaying club information
*/
class Game : public EventObject {
    private:
        DataManager dataManager;
        Menu menu;
        ClubManager clubManager;

    public:
        Game(EventManager* em);

        void run();

        void handleEvent(Event* event) override;
};