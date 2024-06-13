#include "ClubManager.h"
#include "Event.h"

#include "TerminalInterfaces/TerminalInterface.h"
#include "Config.h"
#include "Match.h"

#include <iostream>
#include <string>
#include <typeinfo>


ClubManager::ClubManager(EventManager* em, League& l) : EventObject(em), league(l) {}



/*
Method openLineupSetup():
Displays the lineup setup menu in the terminal.
*/
void ClubManager::openLineupSetup() {

    Team& team = league.getPlayerClub().getTeam();
    Lineup& lineup = league.getPlayerClub().getTeam().getLineup();
    while (!lineup.full()) {
        std::string menuTitle = getLineupTitle(lineup);
        std::vector<Player> playersOutSideLineup = team.getPlayersOutOfLineup();
        OptionsMenuPlayerInterface lineupSetup(menuTitle, playersOutSideLineup);
        PlayerOption choice = lineupSetup.showMenu();

        if (choice.goBack) {
            goBack = true;
            GoBackToMainMenuEvent event;
            sendEvent(&event);
            return;
        }

        if ((choice.player.getPosition() == "Goalkeeper" && lineup.fullGoalkeeper()) ||
            (choice.player.getPosition() == "Defender" && lineup.fullDefenders()) ||
            (choice.player.getPosition() == "Midfielder" && lineup.fullMidfilders()) ||
            (choice.player.getPosition() == "Attacker" && lineup.fullAttackers())) {
            TerminalInterface message;
            message.showWarning(config::LINEUP_UNSUCCESFFUL_MESSAGE);
        }
        else {
        lineup.add(choice.player);
        TerminalInterface message;
        message.showWarning(config::LINEUP_SUCCESSFUL_MESSAGE);
        }
    }
    TerminalInterface message;
    message.showWarning(config::LINEUP_FULL_MESSAGE);

    return;
}


/*
Method openLineupChanger():
Displays the lineup changer menu in the terminal.
*/
void ClubManager::openLineupChanger() {
    Team& team = league.getPlayerClub().getTeam();
    Lineup& lineup = league.getPlayerClub().getTeam().getLineup();
    Lineup backupLineup =  league.getPlayerClub().getTeam().getLineupCopy();
    lineup.clear();
    while (!lineup.full()) {
        std::string menuTitle = getLineupTitle(lineup);
        std::vector<Player> playersOutSideLineup = team.getPlayersOutOfLineup();
        OptionsMenuPlayerInterface lineupSetup(menuTitle, playersOutSideLineup);
        PlayerOption choice = lineupSetup.showMenu();

        if (choice.goBack) {
            lineup = backupLineup;
            league.setActionPoints(league.getActionPoints() + 1);
            openClubManager();
            return;
        }

        if ((choice.player.getPosition() == "Goalkeeper" && lineup.fullGoalkeeper()) ||
            (choice.player.getPosition() == "Defender" && lineup.fullDefenders()) ||
            (choice.player.getPosition() == "Midfielder" && lineup.fullMidfilders()) ||
            (choice.player.getPosition() == "Attacker" && lineup.fullAttackers())) {
            TerminalInterface message;
            message.showWarning(config::LINEUP_UNSUCCESFFUL_MESSAGE);
        }
        else {
        lineup.add(choice.player);
        TerminalInterface message;
        message.showWarning(config::LINEUP_SUCCESSFUL_MESSAGE);
        }
    }
    TerminalInterface message;
    message.showWarning(config::LINEUP_FULL_MESSAGE);

    return;
}

/*
Method openClubManager():
Displays the club manager menu in the terminal.
*/
void ClubManager::openClubManager() {
    std::string menuTitle = getClubManagerTitle();
    OptionsMenuInterface clubManager(menuTitle, {"CHANGE LINEUP", "BROWSE LINEUP", "BROWSE PLAYERS", "PLAY MATCH", "VIEW LEAGUE TABLE", "SEND FOR TRAINING", "GO TO MID MENU"});
    Option choice = clubManager.showMenu();

    if (choice.name == "CHANGE LINEUP") {
        league.setActionPoints(league.getActionPoints() - 1);
        if (league.getActionPoints() > 0) {
            openLineupChanger();
            openClubManager();
            return;
        }
        else {
            TerminalInterface message;
            message.showWarning(config::NO_ACTION_POINTS_MESSAGE);
            openClubManager();
            return;
        }
        return;
    }

    if (choice.name == "BROWSE PLAYERS") {
        openPlayersBrowser(league.getPlayerClub().getTeam().getPlayers());
        return;
    }

    if (choice.name == "BROWSE LINEUP") {
        openPlayersBrowser(league.getPlayerClub().getTeam().getLineup().getAll());
        return;
    }

    if (choice.name == "PLAY MATCH") {
        startMatch();
        return;
    }

    if (choice.name == "SEND FOR TRAINING") {
        if (league.getActionPoints() > 0) {
            openTrainingMenu();
            return;
        }
        else {
            TerminalInterface message;
            message.showWarning(config::NO_ACTION_POINTS_MESSAGE);
            openClubManager();
            return;
        }
    }

    if (choice.name == "VIEW LEAGUE TABLE") {
        viewLeagueTable();
        return;
    }

    if (choice.name == "GO TO MID MENU" || choice.goBack) {
        OpenMidMenuEvent event;
        sendEvent(&event);
        return;
    }
}

/*
Method openPlayersBrowser():
Displays the players in the terminal.
*/
void ClubManager::openPlayersBrowser(std::vector<Player> players) {
    OptionsMenuPlayerInterface playerBrowser("PLAYERS", players);
    PlayerOption choice = playerBrowser.showMenu();

    if (choice.goBack) {
        openClubManager();
        return;
    }

    TerminalInterface message;
    message.showWarning(choice.player.toString());
    openPlayersBrowser(players);
    return;
}

/*
Method viewLeagueTable():
Displays the league table in the terminal.
*/
void ClubManager::viewLeagueTable() {
    TerminalInterface message;
    message.showWarning(getLeagueTable());
    openClubManager();
    return;
}

/*
Method startMatch():
Initializes a match between the player's club and the first opponent that is not defeated.
*/
void ClubManager::startMatch() {
    TerminalInterface message;
    for (auto& oponnent : league.getOpponentsClubs()) { // Checking which opponent is not defeated
        if (!oponnent.isDefeated()) {
            oponnent.getTeam().getLineup().generateRandom(oponnent.getTeam().getPlayers());
            Match match(league.getPlayerClub(), oponnent);
            match.startMatch(); // Starting match
            if (match.isWon()) {
                oponnent.setDefeated(true);
                league.setActionPoints(config::MAX_ACTION_POINTS);
                if (league.isFinished()) {
                    message.showWarning("YOU WON THE GAME!\nCONGRATULATIONS!\n"); // Game won, end of the game
                    EndGameEvent event;
                    sendEvent(&event);
                    return;
                }
                openClubManager();
                return;
            } else {
                league.setDefeats(league.getDefeats() - 1);
                if (league.getDefeats() == 0) {
                    message.showWarning("NO MORE DEFEATS LEFT!\n YOU LOST THE GAME!\n"); // Game lost, end of the game
                    EndGameEvent event;
                    sendEvent(&event);
                    return;
                }
            openClubManager();
            return;
            }
        }
    }
}

/*
Method openTrainingMenu():
Displays the training menu in the terminal.
*/
void ClubManager::openTrainingMenu() {
    std::vector<Player> lineupPlayers = league.getPlayerClub().getTeam().getLineup().getAll();
    OptionsMenuPlayerInterface trainingMenu("SEND FOR TRAINING", lineupPlayers);
    PlayerOption choice = trainingMenu.showMenu();

    if (choice.goBack) {
        openClubManager();
        return;
    }

    sendForTraining(choice.player);

    openClubManager();
    return;
}


/*
Method sendForTraining():
Sends a player for training and increases a random stat of the player.
*/
void ClubManager::sendForTraining(Player& player) {
    league.setActionPoints(league.getActionPoints() - 1);
    std::pair<std::string, int> statIncrease = player.increaseRandomStat();
    std::string messageContent = getTrainingMessage(player, statIncrease.first, statIncrease.second);
    TerminalInterface message;
    message.showWarning(messageContent);
    openClubManager();
}

std::string ClubManager::getLineupTitle(Lineup& lineup) {
    return config::LINEUP_MENU_TITLE + " " + std::to_string(lineup.getAttackers().size()) +
        "/2 ATTACKERS " + std::to_string(lineup.getMidfilders().size()) + "/4 MIDFILDERS "
        + std::to_string(lineup.getDefenders().size()) + "/4 DEFENDERS " +
        std::to_string(lineup.getGoalkeeper().size()) + "/1 GOALKEEPER";;
}

std::string ClubManager::getClubManagerTitle() {
    return "ACTION POINTS LEFT: " + std::to_string(league.getActionPoints()) + " DEFEATS LEFT: " + std::to_string(league.getDefeats());
}

std::string ClubManager::getLeagueTable() {
    std::string result = "LEAGUE TABLE: \n";
    for (OpponentClub& club : league.getOpponentsClubs()) {
        result += std::to_string(club.getPriority() + 1) + ". " + club.getName() + " DEFEATED: " + std::to_string(club.isDefeated()) + "\n";
    }
    return result;
}

std::string ClubManager::getTrainingMessage(const Player& player, const std::string& stat, int increase) {
    return "PLAYER: " + player.getName() + " SUCCESFULLY INCREASED HIS/HERS" + stat + " BY " + std::to_string(increase) + " POINTS";
}

void ClubManager::handleEvent(Event* event) {

    if (event->getType() == typeid(StartGameEvent)) {
        /* Triggered when every data is loaded and game can be started. */
        league = event->getLeague();
        goBack = false;
        if (league.getPlayerClub().getTeam().getLineup().empty()) {
            openLineupSetup();
            if (goBack) {
                league.getPlayerClub().getTeam().getLineup().clear();
                return;
            }
            openClubManager();
            return;
        }
    }

    if (event->getType() == typeid(ContinueGameEvent)) {
        /* Triggered when continue game option in mid menu clicked. */
        openClubManager();
        return;
    }

    return;
}