#include "Menu.h"

#include "Event.h"
#include "TerminalInterfaces/TerminalInterface.h"
#include "Config.h"

#include <iostream>
#include <cstdlib>
#include <typeinfo>

Menu::Menu(EventManager* em) : EventObject(em) {}


/*
Method showMainMenu():

Shows the main menu of the game.
*/
void Menu::showMainMenu() {
    OptionsMenuInterface mainMenu({"NEW GAME", "LOAD GAME", "QUIT GAME"});

    Option choice = mainMenu.showMenu();

    if (choice.name == "NEW GAME") {
        newGame();
        return;
    }


    if (choice.name == "LOAD GAME") {
       loadGame();
       return;
    }

    return;
}

/*
Method showMidMenu():

Shows the mid menu of the game.
*/
void Menu::showMidMenu() {
    OptionsMenuInterface midMenu({"CONTINUE", "SAVE GAME", "GO TO MAIN MENU"});

    Option choice = midMenu.showMenu();

    if (choice.name == "CONTINUE") {
        continueGame();
        return;
    }

    if (choice.name == "SAVE GAME") {
        saveGame();
        return;
    }

    if (choice.name == "GO TO MAIN MENU" || choice.goBack) {
        ClearLeagueRequest event;
        sendEvent(&event);
        showMainMenu();
        return;
    }

    return;
}

/*
Method newGame():

Starts a new game.
*/
void Menu::newGame() {
    GetStringMenuInterface saveNameMenu("ENTER SAVE NAME: ");
    Option saveName = saveNameMenu.showMenu();

    if (saveName.goBack) {
        showMainMenu();
        return;
    }

    if (saveName.invalidData) {
        saveNameMenu.showWarning("SAVE NAME CONTAINS FORBIDDEN CHARACTERS!");
        newGame();
        return;
    }

    CreateNewSaveRequest request(saveName);
    sendEvent(&request);

    return;
}


/*
Method loadGame():

Loads a game.
*/
void Menu::loadGame() {
    DoSavesExistsRequest request;
    sendEvent(&request);
    return;
}

/*
Method saveGame():

Saves a game.
*/
void Menu::saveGame() {
    SaveGameRequest saveGame;
    sendEvent(&saveGame);
    return;
}

/*
Method continueGame():

Continues the game.
*/
void Menu::continueGame() {
    ContinueGameEvent continueGame;
    sendEvent(&continueGame);
    return;
}

/*
Method chooseCountry():

Asks user to choose country to play in.
*/
void Menu::chooseCountry() {
    GetStringMenuInterface countryMenu("CHOOSE COUNTRY YOU WANT TO PLAY IN: \nFOR GLOBAL LEAGUES TYPE 'WORLD'!");
    Option country = countryMenu.showMenu();
    if (country.goBack) {
        deleteCurrentSaveAndGoBack();
        return;
    }
    ChooseCountryResponse response(country);
    sendEvent(&response);
}


/*
Method chooseSeason():

Asks user to choose season to play in.
*/
void Menu::chooseSeason() {
    GetIntegerMenuInterface seasonMenu("CHOOSE SEASON YOU WANT TO PLAY IN: \n(2008 - 2026)");
    IntegerOption season = seasonMenu.showMenu();
    if (season.goBack) {
        deleteCurrentSaveAndGoBack();
        return;
    }

    if (season.invalidData) {
        chooseSeason();
        return;
    }
    ChooseSeasonResponse response(season.value);
    sendEvent(&response);
}

/*
Method deleteCurrentSaveAndGoBack():

Deletes current save and goes back to main menu.
*/
void Menu::deleteCurrentSaveAndGoBack() {
    DeleteCurrentSaveRequest deleteSave;
    sendEvent(&deleteSave);
    showMainMenu();
    return;
}

/*
Method handleEvent():

Handles events.
*/
void Menu::handleEvent(Event* event) {

    if (event->getType() == typeid(SaveAlreadyExistsResponse)) {
        /* Trigerred when save with the same name already exists. */
        OptionsMenuInterface saveExistsMenu(config::SAVE_EXISTS_MESSAGE, {"YES", "NO"});
        Option choice = saveExistsMenu.showMenu();
        if (choice.name == "YES") {
            DeleteCurrentSaveRequest deleteSave;
            sendEvent(&deleteSave);
            CreateNewSaveRequest request(event->getString());
            sendEvent(&request);
            return;
        }
        if (choice.name == "NO" || choice.goBack) {
            newGame();
            return;
        }
    }

    if (event->getType() == typeid(DoSavesExistsResponse)) {
        /* Trigerred when there are saves to load. */
        if (!event->getStrings().empty()) {
            std::vector<std::string> savesNames = event->getStrings();
            OptionsMenuInterface savesMenu("CHOOSE SAVE TO LOAD", savesNames , true);
            Option choice = savesMenu.showMenu();

            if (choice.goBack) {
                showMainMenu();
                return;
            }

            if (choice.toDelete) {
                OptionsMenuInterface deleteSaveMenu("ARE YOU SURE YOU WANT TO DELETE THIS SAVE?", {"YES", "NO"});
                Option deleteChoice = deleteSaveMenu.showMenu();
                if (deleteChoice.name == "YES") {
                    DeleteSaveRequest deleteSave(choice);
                    sendEvent(&deleteSave);
                }
                loadGame();
                return;
            }

            LoadSaveRequest loadSave(choice);
            sendEvent(&loadSave);
            return;

        } else {
            OptionsMenuInterface noSavesMenu("THERE ARE NO SAVES TO LOAD, DO YOU WANT TO CREATE A NEW ONE?", {"YES", "NO"});
            Option choice = noSavesMenu.showMenu();
            if (choice.name == "YES") {
                newGame();
                return;
            }

            if (choice.name == "NO" || choice.goBack) {
                showMainMenu();
                return;
            }
        }

    }

    if (event->getType() == typeid(ChooseCountryRequest)) {
        /*Trigerred when user has to choose country to play in.*/
        chooseCountry();
        return;
    }

    if (event->getType() == typeid(ThereIsNoSuchCountryResponse)) {
        /*Trigerred when country chosen by user is invalid.*/
        TerminalInterface noCountryMenu;
        noCountryMenu.showWarning("THERE IS NO SUCH COUNTRY, PLEASE TRY AGAIN!");
        chooseCountry();
        return;
    }

    if (event->getType() == typeid(ChooseSeasonRequest)) {
        /*Trigerred when user has to choose season to play in.*/
        chooseSeason();
        return;
    }

    if (event->getType() == typeid(ThereIsNoSuchSeasonResponse)) {
        /*Trigerred when season chosen by user is invalid.*/
        TerminalInterface noSeasonMenu;
        noSeasonMenu.showWarning("THERE IS NO SUCH SEASON, PLEASE TRY AGAIN!");
        chooseCountry();
        return;
    }

    if (event->getType() == typeid(ChooseLeagueRequest)) {
        /*Trigerred when user has to choose league to play in.*/
        std::vector<std::pair<int, std::string>> leagues = event->getPairs();
        OptionsMenuIDInterface leaguesMenu("CHOOSE LEAGUE YOU WANT TO PLAY IN:", leagues);

        if (leagues.empty()) {
            leaguesMenu.showWarning("NO LEAGUES IN THIS SEASON AVAIBLE");
            chooseSeason();
            return;
        }

        IntegerOption choice = leaguesMenu.showMenu();
        if (choice.goBack) {
            chooseSeason();
            return;
        }

        ChooseLeagueResponse response(choice);
        sendEvent(&response);
        return;
    }

    if (event->getType() == typeid(ChooseTeamRequest)) {
        /*Trigerred when user has to choose team to play with.*/
        std::vector<std::pair<int, std::string>> teams = event->getPairs();
        OptionsMenuIDInterface teamsMenu("CHOOSE TEAM YOU WANT TO PLAY WITH:", teams);
        IntegerOption choice = teamsMenu.showMenu();
        if (choice.goBack) {
            deleteCurrentSaveAndGoBack();
            return;
        }
        ChooseTeamResponse response(choice);
        sendEvent(&response);
        return;
    }

    if (event->getType() == typeid(SaveGameResponse)) {
        /*Trigerred when game is saved.*/
        TerminalInterface saveGameMenu;
        saveGameMenu.showWarning(config::SAVING_MESSAGE);
        showMidMenu();
        return;
    }

    if (event->getType() == typeid(GoBackToMainMenuEvent)) {
        /*Trigerred when user wants to go back to main menu.*/
        showMainMenu();
        return;
    }

    if (event->getType() == typeid(OpenMidMenuEvent)) {
        /*Trigerred when user wants to go to mid menu.*/
        showMidMenu();
        return;
    }

    if (event->getType() == typeid(EndGameEvent)) {
        /* Triggered when game is ended. */
        deleteCurrentSaveAndGoBack();
        return;
    }

    if (event->getType() == typeid(InvalidDataEvent)) {
        /* Triggered when data is invalid. */
        TerminalInterface invalidDataMenu;
        invalidDataMenu.showWarning(config::INVALID_API_DATA_MESSAGE);
        showMainMenu();
        return;
    }
}