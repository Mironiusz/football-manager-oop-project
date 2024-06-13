#pragma once
#include <cpprest/json.h>

/*
Structure Statistics:

Contains statistics of the player which are used to calculate the player's performance in the match.

Attributes:
- speed: speed of the player
- strength: strength of the player
- stamina: stamina of the player
- technique: technique of the player
- aggression: aggression of the player
*/
struct Statistics {
            int speed = 0;
            int strength = 0;
            int stamina = 0;
            int technique = 0;
            int aggression = 0;

            Statistics();
            Statistics(int priority);

            std::string toString() const;
            operator int() const;
};

/*
Class Player

Represents the player of the club which contains statistics of the player and player's information.

Attributes:
- id: unique identifier of the player
- name: name of the player
- position: position of the player
- age: age
- nationality: nationality of the player
- stats: statistics of the player
*/
class Player {
    private:
        int id = int();
        std::string name = "";
        std::string position = "";
        int age = int();
        std::string nationality = "";
        Statistics stats;

    public:
        Player() = default;
        Player(int i, std::string nm, std::string pos, int a, std::string nat, Statistics st);

        int getId() const;
        std::string getName() const;
        std::string getPosition() const;
        int getAge() const;
        std::string getNationality() const;
        Statistics& getStats();
        bool operator!=(const Player& player) const;

        void setId(int i);
        void setName(std::string nm);
        void setPosition(std::string pos);
        void setAge(int a);
        void setNationality(std::string nat);
        void setStats(Statistics st);
        std::pair<std::string, int> increaseRandomStat();

        std::string toString() const;

        web::json::value toJson() const;
        void fromJson(const web::json::value& jsonData);
};