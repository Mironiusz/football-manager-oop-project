#pragma once
#include "Player.h"

#include <vector>

/*
Class Lineup

Represents the lineup of the club which contains players who will be play matches.

Attributes:
- attackers: list of attackers
- midfilders: list of midfilders
- defenders: list of defenders
- goalkeeper: goalkeeper
*/
class Lineup {
    private:
        std::vector<Player> attackers;
        std::vector<Player> midfilders;
        std::vector<Player> defenders;
        std::vector<Player> goalkeeper;

    public:
        Lineup() = default;

        bool isIn(const Player& player);

        bool empty();

        bool fullAttackers();

        bool fullMidfilders();

        bool fullDefenders();

        bool fullGoalkeeper();

        bool full();

        void add(const Player& player);

        void remove(const Player& player);

        void clear();

        void generateRandom(std::vector<Player>& players);

        std::vector<Player>& getAttackers();

        std::vector<Player>& getMidfilders();

        std::vector<Player>& getDefenders();

        std::vector<Player>& getGoalkeeper();

        std::vector<Player> getAll();

        void setAttackers(std::vector<Player> att);

        void setMidfilders(std::vector<Player> mid);

        void setDefenders(std::vector<Player> def);

        void setGoalkeeper(std::vector<Player> goal);

};