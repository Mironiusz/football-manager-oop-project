#include "Team.h"

std::vector<Player>& Team::getPlayers() {
    return players;
}

Lineup& Team::getLineup() {
    return lineup;
}

Lineup Team::getLineupCopy() {
    return lineup;
}

void Team::setPlayers(std::vector<Player> pl) {
    players = pl;
}

void Team::setLineup(Lineup lp) {
    lineup = lp;
}

/*
Method addPlayer(Player player):

Adds a player to the team.
*/
std::vector<Player> Team::getPlayersOutOfLineup() {
    std::vector<Player> playersOutSideLineup;
    for (const auto& player : players) {
        if (!lineup.isIn(player)) {
            playersOutSideLineup.push_back(player);
        }
    }
    return playersOutSideLineup;
}

/*
Method addPlayer(Player player):

Adds a player to the team.
*/
void Team::clear() {
    lineup.clear();
    players.clear();
}

/*
Method toJson():

Converts the team to a JSON object.
*/
web::json::value Team::toJson() const {
    web::json::value jsonData = web::json::value::array();
    for (const auto& player : players) {
        jsonData[jsonData.size()] = player.toJson();
    }
    return jsonData;
}

/*
Method fromJson():

Converts the JSON object to a team.
*/
void Team::fromJson(const web::json::value& jsonData) {
    for (const auto& item : jsonData.as_array()) {
        Player player;
        player.fromJson(item);
        players.push_back(player);
    }
}

