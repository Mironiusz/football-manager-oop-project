#include <vector>
#include <iostream>

#include "EventManager.h"
#include "Game.h"
#include "DataManager.h"
#include "Event.h"


int main() {
    EventManager em;
    Game game(&em);
    game.run();

    return 0;
}