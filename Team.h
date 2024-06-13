#pragma once

#include "Lineup.h"
#include "Player.h"

/*
Class Team:

Represents the team of the club which contains players and lineup.

Attributes:
- players: list of players
- lineup: lineup of the team
*/
class Team {
    private:
        std::vector<Player> players;
        Lineup lineup;

    public:
        Team() = default;

        std::vector<Player>& getPlayers();
        Lineup& getLineup();
        Lineup getLineupCopy();

        void setPlayers(std::vector<Player> pl);
        void setLineup(Lineup lp);

        std::vector<Player> getPlayersOutOfLineup();

        void clear();

        web::json::value toJson() const;
        void fromJson(const web::json::value& jsonData);
};
