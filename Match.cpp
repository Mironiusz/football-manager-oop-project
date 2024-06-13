#include "Match.h"
#include <random>
#include "Config.h"

/*
Method getRandomScorer():

Generates random scorer form Lineup.

Return random player from the lineup (not a goalkeeper).
*/
Player& Match::getRandomScorer(Lineup& lineup) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distPosition(0, 2);

    int position = distPosition(gen);

    if (position == 0) {
            std::uniform_int_distribution<> distPlayer(0, lineup.getAttackers().size() - 1);
            Player& player = lineup.getAttackers()[distPlayer(gen)];
            return player;
    } else if (position == 2) {
        std::uniform_int_distribution<> distPlayer(0, lineup.getMidfilders().size() - 1);
            Player& player = lineup.getMidfilders()[distPlayer(gen)];
            return player;
    } else {
            std::uniform_int_distribution<> distPlayer(0, lineup.getDefenders().size() - 1);
            Player& player = lineup.getDefenders()[distPlayer(gen)];
            return player;
    }
}

/*
Method distributeGoals():

Distributes goals among the players in the lineup.

Return vector with a list of scorers
*/
std::vector<Player> Match::distributeGoals(Lineup& lineup, int goals) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Player> scorers;

    for (int i = 0; i < goals; ++i) {
        Player& scorer = getRandomScorer(lineup);
        scorers.push_back(scorer);
    }

    return scorers;
}

/*
Method displayMatchLogs():

Displays match logs (informations about scoring) in the terminal interface.
*/
void Match::displayMatchLogs(TerminalInterface& matchLogs, std::vector<std::string>& events) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(events.begin(), events.end(), gen);

    std::uniform_int_distribution<> dist(0, events.size());
    int halfTimeIndex = dist(gen);

    for (size_t i = 0; i < events.size(); ++i) {
        if (i == halfTimeIndex) {
            matchLogs.showWarning("SECOND HALF STARTED!\n");
        }
        matchLogs.showWarning(events[i] + "\n");
    }
}

/*
Method handlePenaltyShootout():

Handles penalty shootout in case of a draw.
*/
void Match::handlePenaltyShootout() {
    TerminalInterface matchLogs;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> penaltyDist(0, 5);

    int playerPenalties = penaltyDist(gen);
    int opponentPenalties = penaltyDist(gen);

    while (playerPenalties == opponentPenalties) {
        playerPenalties = penaltyDist(gen);
        opponentPenalties = penaltyDist(gen);
    }

    if (playerPenalties > opponentPenalties) {
        won = true;
        matchLogs.showWarning("MATCH WON BY " + playerClub.getName() + " IN PENALTY SHOOTOUT!\n");
    } else {
        won = false;
        matchLogs.showWarning("MATCH WON BY " + opponentClub.getName() + " IN PENALTY SHOOTOUT!\n");
    }
}

bool Match::isWon() {
    return won;
}

Match::Match(Club& playerClub, OpponentClub& opponentClub) : playerClub(playerClub), opponentClub(opponentClub) {}

/*
Method startMatch():

Starts the match between the player's club and the opponent club. It generates the match result, distributes goals among the players, and displays the match logs.
*/
void Match::startMatch() {
    TerminalInterface matchLogsInterface;
    matchLogsInterface.showWarning("MATCH STARTED!\n" + playerClub.getName() + " vs " + opponentClub.getName() + "\n");
    std::pair<int, int> result = generateMatchResult(); // MATCH RESULTS
    playerGoals = result.first;
    opponentGoals = result.second;

    Lineup& playerLineup = playerClub.getTeam().getLineup();

    Lineup& opponentLineup = opponentClub.getTeam().getLineup();

    // DISTRIBUTING GOALS BETWEEN TEAMS
    std::vector<Player> playerScorers = distributeGoals(playerLineup, playerGoals);
    std::vector<Player> opponentScorers = distributeGoals(opponentLineup, opponentGoals);

    std::vector<std::string> matchLogs;

    // ADDING EVENTS (PLAYER AND OPPONENT GOALS INFO) TO VECTOR
    for (auto i = 0; i < playerScorers.size(); ++i) {
        std::string event;
        event = "PLAYER " + playerScorers[i].getName() + " SCORED A GOAL\n";
        std::pair<std::string, int> scorerIncrease =  playerScorers[i].increaseRandomStat();
        event += playerScorers[i].getName() + " INCREASED HIS/HERS " + scorerIncrease.first + " by " + std::to_string(scorerIncrease.second) + "\n";
        matchLogs.push_back(event);
    }

    for (size_t i = 0; i < opponentScorers.size(); ++i) {
        std::string event;
        event = "OPPONENT " + opponentScorers[i].getName() + " SCORED A GOAL\n";
        std::pair<std::string, int> scorerIncrease =  opponentScorers[i].increaseRandomStat();
        event += opponentScorers[i].getName() + " INCREASED HIS/HERS " + scorerIncrease.first + " by " + std::to_string(scorerIncrease.second) + "\n";
        matchLogs.push_back(event);
    }

    // DISPLAYING MATCH LOGS
    displayMatchLogs(matchLogsInterface, matchLogs);

    // HANDLING MATCH RESULT
    if (playerGoals > opponentGoals) {
        won = true;
        matchLogsInterface.showWarning("MATCH WON BY " + playerClub.getName() + "!\n");
    } else if (playerGoals < opponentGoals) {
        won = false;
        matchLogsInterface.showWarning("MATCH WON BY " + opponentClub.getName() + "!\n");
    } else {
        matchLogsInterface.showWarning("MATCH DRAWN! PENALTY SHOOTOUT!\n");
        handlePenaltyShootout();
    }

    return;
}

/*
Method generateMatchResult():

Randomly generates the match result based on the player's and opponent's stats.

Return pair with the number of goals scored by the player and the opponent.
*/
std::pair<int, int> Match::generateMatchResult() {

            Lineup& playerLineup = playerClub.getTeam().getLineup();
            Lineup& opponentLineup = opponentClub.getTeam().getLineup();

            int playerStats = 0;
            int opponentStats = 0;

            for (auto& player : playerLineup.getAll()) {
                playerStats += int(player.getStats());
            }

            for (auto& player : opponentLineup.getAll()) {
                opponentStats += int(player.getStats());
            }

            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_int_distribution<> goalDistPlayer(0, playerStats / 10);
            std::uniform_int_distribution<> goalDistOpponent(0, opponentStats / 10);

            playerGoals = goalDistPlayer(gen);
            opponentGoals = goalDistOpponent(gen);

            return std::make_pair(playerGoals, opponentGoals);
        }
