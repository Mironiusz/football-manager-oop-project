#pragma once

#include "Club.h"
#include "TerminalInterfaces/TerminalInterface.h"

/*
Class Match

Responsible for handling the match between the player's club and the opponent club. It generates the match result, distributes goals among the players, and displays the match logs.

Attributes:
- playerClub: player's club
- opponentClub: opponent club
- won: true if the player's club won the match, false otherwise
- playerGoals: number of goals scored by the player's club
- opponentGoals: number of goals scored by the opponent club
*/
class Match {
    private:
        Club& playerClub;
        OpponentClub& opponentClub;
        bool won = false;
        int playerGoals = 0;
        int opponentGoals = 0;

        Player& getRandomScorer(Lineup& lineup);

    public:
        Match(Club& playerClub, OpponentClub& opponentClub);

        void startMatch();
        std::pair<int, int> generateMatchResult();
        std::vector<Player> distributeGoals(Lineup& lineup, int goals);
        void displayMatchLogs(TerminalInterface& matchLogs, std::vector<std::string>& events);
        void handlePenaltyShootout();
        bool isWon();
};