#include "Lineup.h"
#include "Config.h"

/*
Method isIn():
Params:
- const Player& player

Returns true if the given player is in the lineup, false otherwise.
*/
bool Lineup::isIn(const Player& player) {
     for (const auto& attacker : attackers) {
        if (attacker.getId() == player.getId()) {
            return true;
        }
    }
    for (const auto& midfielder : midfilders) {
        if (midfielder.getId() == player.getId()) {
            return true;
        }
    }
    for (const auto& defender : defenders) {
        if (defender.getId() == player.getId()) {
            return true;
        }
    }
    for (const auto& gk : goalkeeper) {
        if (gk.getId() == player.getId()) {
            return true;
        }
    }

    return false;
}

/*
Method empty()

Returns true if the lineup is empty, false otherwise.
*/
bool Lineup::empty() {
    return (getAll().empty());
}

bool Lineup::fullAttackers() {
    return (attackers.size() == config::MAX_ATTACKERS);
}

bool Lineup::fullMidfilders() {
    return (midfilders.size() == config::MAX_MIDFIELDERS);
}

bool Lineup::fullDefenders() {
    return (defenders.size() == config::MAX_DEFENDERS);
}

bool Lineup::fullGoalkeeper() {
    return (goalkeeper.size() == config::MAX_GOALKEEPERS);
}

/*
Method full()

Returns true if the lineup is full, false otherwise.
*/
bool Lineup::full() {
    return (fullAttackers() && fullMidfilders() && fullDefenders() && fullGoalkeeper());
}

/*
Methor add():
Params:
- const Player& player

Adds given player to the lineup.
*/
void Lineup::add(const Player& player) {
    if (player.getPosition() == "Attacker")
            attackers.push_back(player);
    else if (player.getPosition() == "Midfielder")
            midfilders.push_back(player);
    else if (player.getPosition() == "Defender")
            defenders.push_back(player);
    else if (player.getPosition() == "Goalkeeper")
            goalkeeper.push_back(player);
    return;
}

/*
Method remove():
Params:
-    const Player& player

Removes given player from the lineup.
*/
void Lineup::remove(const Player& player) {
    if (player.getPosition() == "Attacker") {
        for (auto it = attackers.begin(); it != attackers.end(); ++it) {
            if (it->getId() == player.getId()) {
                attackers.erase(it);
                return;
            }
        }
    }
    else if (player.getPosition() == "Midfilder") {
        for (auto it = midfilders.begin(); it != midfilders.end(); ++it) {
            if (it->getId() == player.getId()) {
                midfilders.erase(it);
                return;
            }
        }
    }
    else if (player.getPosition() == "Defender") {
        for (auto it = defenders.begin(); it != defenders.end(); ++it) {
            if (it->getId() == player.getId()) {
                defenders.erase(it);
                return;
            }
        }
    }
    else if (player.getPosition() == "GoalKeeper") {
        for (auto it = goalkeeper.begin(); it != goalkeeper.end(); ++it) {
            if (it->getId() == player.getId()) {
                goalkeeper.erase(it);
                return;
            }
        }
    }
    return;
}


/*
Method clear():

Clears the lineup by removing all players from it.
*/
void Lineup::clear() {
    attackers.clear();
    midfilders.clear();
    defenders.clear();
    goalkeeper.clear();
}

void Lineup::generateRandom(std::vector<Player>& players) {
    for (const auto& player : players) {
        if (player.getPosition() == "Attacker" && !fullAttackers()) {
            attackers.push_back(player);
        }
        else if (player.getPosition() == "Midfielder" && !fullMidfilders()) {
            midfilders.push_back(player);
        }
        else if (player.getPosition() == "Defender" && !fullDefenders()) {
            defenders.push_back(player);
        }
        else if (player.getPosition() == "Goalkeeper" && !fullGoalkeeper()) {
            goalkeeper.push_back(player);
        }
    }
}

std::vector<Player>& Lineup::getAttackers() {
    return attackers;
}

std::vector<Player>& Lineup::getMidfilders() {
    return midfilders;
}

std::vector<Player>& Lineup::getDefenders() {
    return defenders;
}

std::vector<Player>& Lineup::getGoalkeeper() {
    return goalkeeper;
}

void Lineup::setAttackers(std::vector<Player> att) {
    attackers = att;
}

void Lineup::setMidfilders(std::vector<Player> mid) {
    midfilders = mid;
}

void Lineup::setDefenders(std::vector<Player> def) {
    defenders = def;
}

void Lineup::setGoalkeeper(std::vector<Player> goal) {
    goalkeeper = goal;
}

/*
Method getAll():

Returns a vector containing all players in the lineup.
*/
std::vector<Player> Lineup::getAll() {
    std::vector<Player> all;
    all.insert(all.end(), attackers.begin(), attackers.end());
    all.insert(all.end(), midfilders.begin(), midfilders.end());
    all.insert(all.end(), defenders.begin(), defenders.end());
    all.insert(all.end(), goalkeeper.begin(), goalkeeper.end());
    return all;
}