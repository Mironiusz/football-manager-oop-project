#include "Club.h"

Club::Club(int i, std::string nm) : id(i), name(nm) {}

int Club::getId() const {
    return id;
}

std::string Club::getName() const {
    return name;
}

Team& Club::getTeam() {
    return team;
}

void Club::setId(int newId) {
    id = newId;
}

void Club::setName(std::string newName) {
    name = newName;
}

void Club::setTeam(Team newTeam) {
    team = newTeam;
}

/*
Method clear():

Clears the club by setting the id to 0, name to an empty string and clearing the team.
*/
void Club::clear() {
    id = int();
    name = "";
    team.clear();
}

void Club::set(std::pair<int, std::string> idAndName) {
    setId(idAndName.first);
    setName(idAndName.second);
}

/*
Method toJson():

Converts the club to a JSON object.
*/
web::json::value Club::toJson() const {
    web::json::value jsonData;
    jsonData[U("id")] = web::json::value::number(id);
    jsonData[U("name")] = web::json::value::string(name);
    jsonData[U("team")] = team.toJson();
    return jsonData;
}


/*
Method fromJson():

Converts the JSON object to a club.
*/
void Club::fromJson(const web::json::value& jsonData) {
    id = jsonData.at(U("id")).as_integer();
    name = jsonData.at(U("name")).as_string();
    team.fromJson(jsonData.at(U("team")));
}

OpponentClub::OpponentClub(int i, std::string nm, int pr) : Club(i, nm), priority(pr) {}

int OpponentClub::getPriority() const {
    return priority;
}

bool OpponentClub::isDefeated() const {
    return defeated;
}

void OpponentClub::setPriority(int newPriority) {
    priority = newPriority;
}

void OpponentClub::setDefeated(bool newDefeated) {
    defeated = newDefeated;
}

/*
Method toJson():

Converts the opponent club to a JSON object.
*/
web::json::value OpponentClub::toJson() const {
    web::json::value jsonData = Club::toJson();
    jsonData[U("priority")] = web::json::value::number(priority);
    jsonData[U("defeated")] = web::json::value::boolean(defeated);
    return jsonData;
}

/*
Method fromJson():

Converts the JSON object to an opponent club.
*/
void OpponentClub::fromJson(const web::json::value& jsonData) {
    Club::fromJson(jsonData);
    priority = jsonData.at(U("priority")).as_integer();
    defeated = jsonData.at(U("defeated")).as_bool();
}