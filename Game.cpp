#include <iostream>

#include "Game.h"


Game::Game(EventManager* em) : EventObject(em), dataManager(em), menu(em), clubManager(em, dataManager.getLeague()){}

/*
Method run():

Runs the game by showing the main menu.
*/
void Game::run() {
    menu.showMainMenu();
    return;
}

void Game::handleEvent(Event* event) {
    return;
}