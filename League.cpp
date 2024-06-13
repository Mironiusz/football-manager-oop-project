#include "League.h"

int League::getId() const {
    return id;
}

std::string League::getName() const {
    return name;
}

std::string League::getCountry() const {
    return country;
}

int League::getSeason() const {
    return season;
}

int League::getDefeats() const {
    return defeats;
}

int League::getActionPoints() const {
    return actionPoints;
}

int League::getCurrentPriority() const {
    return currentPriority;
}

Club& League::getPlayerClub() {
    return playerClub;
}

std::vector<OpponentClub>& League::getOpponentsClubs() {
    return opponentsClubs;
}

void League::setId(int newId) {
    id = newId;
}

void League::setName(std::string newName) {
    name = newName;
}

void League::setCountry(std::string newCountry) {
    country = newCountry;
}

void League::setSeason(int newSeason) {
    season = newSeason;
}

void League::setDefeats(int newDefeats) {
    defeats = newDefeats;
}

void League::setActionPoints(int newActionPoints) {
    actionPoints = newActionPoints;
    if (actionPoints < 0) {
        actionPoints = 0;
    }
}

void League::setCurrentPriority(int newCurrentPriority) {
    currentPriority = newCurrentPriority;
}

void League::setPlayerClub(Club newClub) {
    playerClub = newClub;
}

void League::setOpponentsClubs(std::vector<OpponentClub> newOpponentsClubs) {
    opponentsClubs = newOpponentsClubs;
}

void League::set(std::pair<int, std::string> idAndName) {
    setId(idAndName.first);
    setName(idAndName.second);
}

/*
Method clear():

Clears the league data.
*/
void League::clear() {
    id = int();
    name = "";
    country = "";
    season = int();
    playerClub.clear();
    opponentsClubs.clear();
    defeats = config::MAX_DEFEATS;
    actionPoints = config::MAX_ACTION_POINTS;
}

/*
Method isFinished() const:

Checks if the league is finished or not.
*/
bool League::isFinished() const {
    for (const auto& opponentClub : opponentsClubs) {
        if (!opponentClub.isDefeated()) {
            return false;
        }
    }
    return true;
}

/*
Method toJson() const:

Converts the League object to JSON data.
*/
web::json::value League::toJson() const {
    web::json::value jsonData;
    jsonData[U("id")] = web::json::value::number(id);
    jsonData[U("name")] = web::json::value::string(name);
    jsonData[U("country")] = web::json::value::string(country);
    jsonData[U("season")] = web::json::value::number(season);
    jsonData[U("defeats")] = web::json::value::number(defeats);
    jsonData[U("actionPoints")] = web::json::value::number(actionPoints);
    jsonData[U("currentPriority")] = web::json::value::number(currentPriority);
    jsonData[U("playerClub")] = playerClub.toJson();
    jsonData[U("opponentsClubs")] = web::json::value::array();
    for (const auto& opponentClub : opponentsClubs) {
        jsonData[U("opponentsClubs")][jsonData[U("opponentsClubs")].size()] = opponentClub.toJson();
    }
    return jsonData;
}

/*
Method fromJson(const web::json::value& jsonData):

Converts the JSON data to the League object.
*/
void League::fromJson(const web::json::value& jsonData) {
    id = jsonData.at(U("id")).as_integer();
    name = jsonData.at(U("name")).as_string();
    country = jsonData.at(U("country")).as_string();
    season = jsonData.at(U("season")).as_integer();
    defeats = jsonData.at(U("defeats")).as_integer();
    actionPoints = jsonData.at(U("actionPoints")).as_integer();
    currentPriority = jsonData.at(U("currentPriority")).as_integer();
    playerClub.fromJson(jsonData.at(U("playerClub")));
    for (const auto& item : jsonData.at(U("opponentsClubs")).as_array()) {
        OpponentClub opponentClub;
        opponentClub.fromJson(item);
        opponentsClubs.push_back(opponentClub);
    }
}