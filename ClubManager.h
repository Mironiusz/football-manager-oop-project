#pragma once
#include "EventObject.h"
#include "Event.h"

#include "League.h"
#include "Config.h"


/*
Class ClubManager:

Responsible for managing the club. It contains the league and handles club events.

Attributes:
- league: league which contains club data (from DataManager)
- goBack: flag to go back to the previous menu
*/
class ClubManager : public EventObject {
    private:
        League& league;
        bool goBack = false;
    public:

        ClubManager(EventManager* em, League& l);

        void openLineupSetup();

        void openLineupChanger();

        void openClubManager();

        void openPlayersBrowser(std::vector<Player> players);

        void viewLeagueTable();

        void startMatch();

        void openTrainingMenu();

        void sendForTraining(Player& player);

        std::string getLineupTitle(Lineup& lineup);

        std::string getClubManagerTitle();

        std::string getLeagueTable();

        std::string getTrainingMessage(const Player& player, const std::string& stat, int increase);

        void handleEvent(Event* event) override;
};