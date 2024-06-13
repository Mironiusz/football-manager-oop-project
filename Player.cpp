#include "Player.h"
#include "Config.h"
#include <random>

/*
Random constructor for the class Statistics.
*/
Statistics::Statistics() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dist(config::MIN_STATS, config::MAX_BASIC_STATS);

    speed = dist(gen);
    strength = dist(gen);
    stamina = dist(gen);
    technique = dist(gen);
    aggression = dist(gen);
}

/*
Random constructor for the class Statistics with priority.
*/
Statistics::Statistics(int priority) : Statistics() {
    int baseIncrease = priority / 2;
    int remainingIncrease = priority % 2;

    speed = std::min(speed + baseIncrease, config::MAX_STATS);
    strength = std::min(strength + baseIncrease, config::MAX_STATS);
    stamina = std::min(stamina + baseIncrease, config::MAX_STATS);
    technique = std::min(technique + baseIncrease, config::MAX_STATS);
    aggression = std::min(aggression + baseIncrease, config::MAX_STATS);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 4);

    for (int i = 0; i < remainingIncrease; ++i) {
        switch (dist(gen)) {
            case 0:
                speed = std::min(speed + 1, config::MAX_STATS);
                break;
            case 1:
                strength = std::min(strength + 1, config::MAX_STATS);
                break;
            case 2:
                stamina = std::min(stamina + 1, config::MAX_STATS);
                break;
            case 3:
                technique = std::min(technique + 1, config::MAX_STATS);
                break;
            case 4:
                aggression = std::min(aggression + 1, config::MAX_STATS);
                break;
        }
    }
}

/*
Method toString():

Returns the statistics as a string.
*/
std::string Statistics::toString() const {
    return "Speed: " + std::to_string(speed) + " Strength: " + std::to_string(strength) +
        " Stamina: " + std::to_string(stamina) + " Technique: " + std::to_string(technique) +
        " Aggression: " + std::to_string(aggression);
}

Statistics::operator int() const {
    return speed + strength + stamina + technique + aggression;
}

Player::Player(int i, std::string nm, std::string pos, int a, std::string nat, Statistics st) :
    id(i), name(nm), position(pos), age(a), nationality(nat), stats(st) {}

int Player::getId() const {
    return id;
}

std::string Player::getName() const {
    return name;
}

std::string Player::getPosition() const {
    return position;
}

int Player::getAge() const {
    return age;
}

std::string Player::getNationality() const {
    return nationality;
}

Statistics& Player::getStats() {
    return stats;
}

bool Player::operator!=(const Player& other) const {
    return id != other.id;
}

void Player::setId(int i) {
    id = i;
}

void Player::setName(std::string nm) {
    name = nm;
}

void Player::setPosition(std::string pos) {
    position = pos;
}

void Player::setAge(int a) {
    age = a;
}

void Player::setNationality(std::string nat) {
    nationality = nat;
}

void Player::setStats(Statistics st) {
    stats = st;
}

/*
Method increaseRandomStat():

Increases a random stat of the player by a random amount.
*/
std::pair<std::string, int> Player::increaseRandomStat() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 4);
    std::uniform_int_distribution<int> increaseDist(config::BASE_INCREASE, config::MAX_INCREASE);

    int statIncrease = increaseDist(rd);

    switch (dist(gen)) {
        case 0:
            stats.speed = std::min(stats.speed + statIncrease, config::MAX_STATS);
            return std::make_pair("SPEED", statIncrease);
        case 1:
            stats.strength = std::min(stats.strength + statIncrease, config::MAX_STATS);
            return std::make_pair("STRENGHT", statIncrease);
        case 2:
            stats.stamina = std::min(stats.stamina + statIncrease, config::MAX_STATS);
            return std::make_pair("STAMINA", statIncrease);
        case 3:
            stats.technique = std::min(stats.technique + statIncrease, config::MAX_STATS);
            return std::make_pair("TECHNIQUE", statIncrease);
        case 4:
            stats.aggression = std::min(stats.aggression + statIncrease, config::MAX_STATS);
            return std::make_pair("AGGRESSION", statIncrease);
    }

    return std::make_pair("NULL", 0);
}


/*
Method toString():

Returns the player as a string.
*/
std::string Player::toString() const {
    return "ID: " + std::to_string(id) + " Name: " + name + " Position: " + position +
        " Age: " + std::to_string(age) + " Nationality: " + nationality + " Stats: " + stats.toString() + "\n";
}


/*
Method toJson():

Converts the player to a JSON object.
*/
web::json::value Player::toJson() const {
    web::json::value jsonData;
    jsonData[U("id")] = web::json::value::number(id);
    jsonData[U("name")] = web::json::value::string(name);
    jsonData[U("position")] = web::json::value::string(position);
    jsonData[U("age")] = web::json::value::number(age);
    jsonData[U("nationality")] = web::json::value::string(nationality);
    jsonData[U("speed")] = web::json::value::number(stats.speed);
    jsonData[U("strength")] = web::json::value::number(stats.strength);
    jsonData[U("stamina")] = web::json::value::number(stats.stamina);
    jsonData[U("technique")] = web::json::value::number(stats.technique);
    jsonData[U("aggression")] = web::json::value::number(stats.aggression);
    return jsonData;
}


/*
Method fromJson():

Converts the JSON object to a player.
*/
void Player::fromJson(const web::json::value& jsonData) {
    id = jsonData.at(U("id")).as_integer();
    name = jsonData.at(U("name")).as_string();
    position = jsonData.at(U("position")).as_string();
    age = jsonData.at(U("age")).as_integer();
    nationality = jsonData.at(U("nationality")).as_string();
    stats.speed = jsonData.at(U("speed")).as_integer();
    stats.strength = jsonData.at(U("strength")).as_integer();
    stats.stamina = jsonData.at(U("stamina")).as_integer();
    stats.technique = jsonData.at(U("technique")).as_integer();
    stats.aggression = jsonData.at(U("aggression")).as_integer();
}