#pragma once

#include "Club.h"
#include "Config.h"

/*
Class League

The most important class which contains save data which allows user to play the game.

Attributes:
- id: unique identifier of the league
- name: name of the league
- country: country of the league
- season: current season of the league

- defeats: number of allowed defeats of the player's club
- actionPoints: number of action points of the player's club per round
- currentPriority: current priority of the opponent club

- playerClub: club of the player
- opponentsClubs: list of opponent clubs
*/
class League {
    private:
        int id;
        std::string name;
        std::string country;
        int season;

        int defeats = config::MAX_DEFEATS;
        int actionPoints = config::MAX_ACTION_POINTS;
        int currentPriority = 0;

        Club playerClub;
        std::vector<OpponentClub> opponentsClubs;

    public:
        League() = default;

        int getId() const;

        std::string getName() const;

        std::string getCountry() const;

        int getSeason() const;

        int getDefeats() const;

        int getActionPoints() const;

        int getCurrentPriority() const;

        Club& getPlayerClub();

        std::vector<OpponentClub>& getOpponentsClubs();

        void setId(int newId);

        void setName(std::string newName);

        void setCountry(std::string newCountry);

        void setSeason(int newSeason);

        void setDefeats(int newDefeats);

        void setActionPoints(int newActionPoints);

        void setCurrentPriority(int newCurrentPriority);

        void setPlayerClub(Club newClub);

        void setOpponentsClubs(std::vector<OpponentClub> newOpponentsClubs);

        void set(std::pair<int, std::string> idAndName);

        void clear();

        bool isFinished() const;

        web::json::value toJson() const;
        void fromJson(const web::json::value& jsonData);
};