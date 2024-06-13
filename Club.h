#pragma once

#include "Team.h"
#include "Player.h"

/*
Class Club:

Represents the club which contains id, name and team.

Attributes:
- id: unique identifier of the club
- name: name of the club
- team: team of the club
*/
class Club {
    protected:
        int id;
        std::string name;
        Team team;
    public:
        Club() = default;
        Club(int i, std::string nm);

        int getId() const;
        std::string getName() const;
        Team& getTeam();

        void setId(int newId);
        void setName(std::string newName);
        void setTeam(Team newTeam);

        void set(std::pair<int, std::string> idAndName);

        void clear();

        web::json::value toJson() const;
        void fromJson(const web::json::value& jsonData);
};

/*
Class OpponentClub:

Represents the opponent club which contains priority and defeated status.

Attributes:
- priority: priority of the opponent club
- defeated: defeated status of the opponent club
*/
class OpponentClub : public Club {
    private:
        int priority = 0;
        bool defeated = false;
    public:
        OpponentClub() = default;
        OpponentClub(int i, std::string nm, int pr);

        int getPriority() const;
        bool isDefeated() const;

        void setPriority(int newPriority);
        void setDefeated(bool newDefeated);

        web::json::value toJson() const;
        void fromJson(const web::json::value& jsonData);
};