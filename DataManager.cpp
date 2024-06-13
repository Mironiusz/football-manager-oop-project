#include "DataManager.h"
#include "Event.h"

#include <filesystem>
#include <algorithm>
#include <thread>
#include <chrono>
#include <fstream>

DataManager::DataManager(EventManager* em) : EventObject(em) {

    if (!std::filesystem::exists(savesPath)) {
        std::filesystem::create_directory(savesPath);
    }
}

/*
Method getRemainingRequests():
Params:
- http_response - response

Returns:
- int - ammount of remaining API requests from free plan
*/
int DataManager::getRemainingRequests(const web::http::http_response& response) {
    auto headers = response.headers();
    if (headers.has("x-ratelimit-requests-remaining")) {
        std::string remainingRequestsStr = headers.find("x-ratelimit-requests-remaining")->second;
        int remainingRequests = std::stoi(remainingRequestsStr);
        return remainingRequests;
    } else {
        return -1;
    }
}

 /*
Method GetFromAPI():
Params:
- string - endpoint - end for API, from which data will be downloaded
- string - fileName - name of the file in current save directory to which data will be downloaded

List of endpoints:
seasons - seasons.json -------------------------------------> list of seasons
leagues/country/{country}/{season} - leagues.json ----------> list of leagues in gives season and country
teams/league/{league_id} - teams{league_id}.json -----------> list of clubs in given league
players/squad/{team_id}/{season} - team{team_id}.json ------> list of players in given club
players/team/{team_id}/{season} - stats{team_id}.json ------> list of players stats in give club
*/
void DataManager::getFromApi(std::string endpoint, std::string fileName)
        {
            web::http::client::http_client client(apiUrl + endpoint);
            web::http::http_request request(web::http::methods::GET);
            request.headers().add("x-rapidapi-key", apiKey);
            request.headers().add("x-rapidapi-host", apiHost);
            client.request(request).then([this, fileName](web::http::http_response response)
            {
                if (response.status_code() == web::http::status_codes::OK)
                {
                    int remainingRequests = this->getRemainingRequests(response);
                    if (remainingRequests >= 0)
                    {
                        std::cout << config::REQUESTS_MESSAGE << remainingRequests << " / " << 100 << std::endl;
                        concurrency::streams::istream bodyStream = response.body();
                        auto buffer = std::make_shared<concurrency::streams::container_buffer<std::string>>();
                        bodyStream.read_to_end(*buffer).then([this, fileName, buffer](size_t bytesRead)
                        {
                            std::string responseBody = buffer->collection();
                            std::string filePath =  savePath() + "/" + fileName;
                            std::ofstream file(filePath);

                            std::cout << config::SAVING_FILE_MESSAGE << filePath << std::endl;
                            if (!file) {
                                std::cerr << config::FILE_OPENING_ERROR_MESSAGE << filePath << std::endl;
                                return;
                            }

                            file << responseBody;
                            file.close();
                            std::cout << config::SAVING_FILE_SUCCESS_MESSAGE << fileName << std::endl;
                        }).wait();
                    }
                    else{
                        std::cerr << config::NO_REQUEST_LEFT_MESSAGE << std::endl;
                    }
                }
                else
                {
                    std::cerr << config::STATUS_ERROR_MESSAGE << response.status_code() << std::endl;
                }
            }).wait();
        }

/*
Method getCountriesNames():

Loads countries from file and returns vector(string) of their names.
*/
std::vector<std::string> DataManager::getCountriesNames() {
    std::vector<std::string> countries;
    std::string filePath = dataPath + "countries.json";

    std::fstream file(filePath);

    if (!file.is_open()) {
        std::cerr << config::FILE_OPENING_ERROR_MESSAGE << filePath << std::endl;
        std::perror("Error");
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string fileContent = buffer.str();

    web::json::value jsonData = web::json::value::parse(fileContent);

    if (jsonData.has_field(U("countries"))) {
        web::json::array jsonCountries = jsonData.at(U("countries")).as_array();
        for (const auto& country : jsonCountries) {
            countries.push_back(country.as_string());
        }
        return countries;
    }

    return {};
}

/*
Method getSeasons():

Loads seasons from file and returns vector(int) of them.
*/
std::vector<int> DataManager::getSeasons()
    {
        std::string filePath = dataPath + "seasons.json";
        std::fstream file(filePath);

        if (!file) {
            std::cerr << config::FILE_OPENING_ERROR_MESSAGE << filePath << std::endl;
            return {};
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        std::string fileContent = buffer.str();
        web::json::value jsonValue = web::json::value::parse(fileContent);

        std::vector<int> seasons;
        if (jsonValue.has_field(U("api")) && jsonValue[U("api")].has_field(U("seasons"))) {
            auto seasonsArray = jsonValue[U("api")][U("seasons")].as_array();
            for (const auto& season : seasonsArray) {
                    seasons.push_back(season.as_integer());
            }
        }
            return seasons;
        }

/*
Method getLeagues():
Params:
- string - file name to load data from to in save directory

Returns:
- vector(pair(int, string)) - list of leagues in given file (league id, league name)
*/
std::vector<std::pair<int, std::string>> DataManager::getLeagues(const std::string& fileName)
    {
        std::string filePath = savePath() + "/" + fileName;
        std::ifstream file(filePath);

        if (!file) {
            std::cerr << "Nie można otworzyć pliku do odczytu: " << filePath << std::endl;
            return {};
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        std::string fileContent = buffer.str();
        web::json::value jsonValue = web::json::value::parse(fileContent);

        std::vector<std::pair<int, std::string>> leagues;
        if (jsonValue.has_field(U("api")) && jsonValue[U("api")].has_field(U("leagues"))) {
            for (const auto& leagueValue : jsonValue[U("api")][U("leagues")].as_array()) {
                if (leagueValue.has_field(U("league_id")) && leagueValue.has_field(U("name")) && leagueValue.has_field(U("coverage"))) {
                    auto& coverageField = leagueValue.at(U("coverage"));
                    if (coverageField.has_field(U("players")))
                    leagues.emplace_back(
                        leagueValue.at(U("league_id")).as_integer(),
                        leagueValue.at(U("name")).as_string()
                    );
                }
            }
        } else {
            std::cerr << "Nie znaleziono oczekiwanych pól w JSON." << std::endl;
        }

        return leagues;
    }

/*
Method getTeams():
Params:
- string - file name to load data from to in save directory

Returns:
- vector(pair(int, string)) - list of teams in given file (team id, team name)
*/
std::vector<std::pair<int, std::string>> DataManager::getTeams(const std::string& fileName)
    {
        std::string filePath = savePath() + "/" + fileName;
        std::ifstream file(filePath);

        if (!file) {
            std::cerr << config::FILE_OPENING_ERROR_MESSAGE << filePath << std::endl;
            return {};
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        std::string fileContent = buffer.str();
        web::json::value jsonValue = web::json::value::parse(fileContent);

        std::vector<std::pair<int, std::string>> teams;
        if (jsonValue.has_field(U("api")) && jsonValue[U("api")].has_field(U("teams"))) {
            for (const auto& leagueValue : jsonValue[U("api")][U("teams")].as_array()) {
                if (leagueValue.has_field(U("team_id")) && leagueValue.has_field(U("name"))) {
                    teams.emplace_back(
                        leagueValue.at(U("team_id")).as_integer(),
                        leagueValue.at(U("name")).as_string()
                    );
                }
            }
        } else {
            std::cerr << config::WRONG_JSON_FIELDS_MESSAGE << std::endl;
        }

        return teams;
}

/*
Method getPlayers():
Params:
- string - file name to load data from to in save directory

Returns:
- vector(vector(string)) - list of players data in given file
- [0] - id
- [1] - name
- [2] - position
- [3] - age
- [4] - nationality
*/
std::vector<std::vector<std::string>> DataManager::getPlayers(const std::string& fileName) {
            std::string filePath = savePath() + "/" + fileName;
            std::ifstream file(filePath);

            if (!file) {
                std::cerr << "Nie można otworzyć pliku do odczytu: " << filePath << std::endl;
                return {};
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();

            std::string fileContent = buffer.str();
            web::json::value jsonValue = web::json::value::parse(fileContent);

            std::vector<std::vector<std::string>> players;
            if (jsonValue.has_field(U("api")) && jsonValue[U("api")].has_field(U("players"))) {
                for (const auto& playerValue : jsonValue[U("api")][U("players")].as_array()) {
                    if (playerValue.has_field(U("player_id")) && playerValue.has_field(U("player_name")) &&
                        playerValue.has_field(U("position")) && playerValue.has_field(U("age")) &&
                        playerValue.has_field(U("nationality"))) {

                        std::vector<std::string> playerInfo;
                        playerInfo.push_back(std::to_string(playerValue.at(U("player_id")).as_integer()));
                        playerInfo.push_back(playerValue.at(U("player_name")).as_string());
                        playerInfo.push_back(playerValue.at(U("position")).as_string());
                        playerInfo.push_back(std::to_string(playerValue.at(U("age")).as_integer()));
                        playerInfo.push_back(playerValue.at(U("nationality")).as_string());

                        players.push_back(playerInfo);
                    }
                }
            } else {
                std::cerr << "Nie znaleziono oczekiwanych pól w JSON." << std::endl;
            }

            return players;
        }

League& DataManager::getLeague() {
    return league;
}

void DataManager::setSaveName(std::string svNm) {
    saveName = svNm;
}

/*
Method savePath():
- returns the path to the current save directory
*/
std::string DataManager::savePath() const {
    return savesPath + saveName;
}

/*
Method saveExists():

Returns true if main saves directory is not empty.
*/
bool DataManager::savesExists() const {
    return !std::filesystem::is_empty(savesPath);
}

/*
Method saveExists():

Returns true if current save exists.
*/
bool DataManager::saveExists() const {
    return std::filesystem::exists(savePath());
}

/*
Method countryExists():
Params:
- string - country name

Returns:
- bool - true if country exists (in API)
*/
bool DataManager::countryExists(std::string& country) {
    std::string formattedCountry = formatCountryName(country);
    std::vector<std::string> countries = getCountriesNames();

    return std::find(countries.begin(), countries.end(), formattedCountry) != countries.end();
}

/*
Method seasonExists():
Params:
- int - season

Returns:
- bool - true if season exists (in API)
*/
bool DataManager::seasonExists(int season) {
    std::vector<int> seasons = getSeasons();

    return std::find(seasons.begin(), seasons.end(), season) != seasons.end();
}

/*
Method getSavesNames():

Returns all of saves directory from main saves directory.
*/
std::vector<std::string> DataManager::getSavesNames() const {
    std::vector<std::string> saves;
    if (savesExists()) {
        for (const auto& entry : std::filesystem::directory_iterator(savesPath)) {
            saves.push_back(entry.path().filename().string());
        }
        return saves;
    }
    return {};
}

/*
Method formatCountryName():
Params:
- string - country name

Returns:
- string - country name transformed to API format
*/
std::string DataManager::formatCountryName(std::string& country) {

    std::transform(country.begin(), country.end(), country.begin(), ::tolower);

    if (country == "usa") {
        return "USA";
    }

    std::replace(country.begin(), country.end(), ' ', '-');

    bool newWord = true;
    for (auto& ch : country) {
        if (newWord && std::isalpha(ch)) {
            ch = std::toupper(ch);
            newWord = false;
        } else if (ch == '-') {
            newWord = true;
        }
    }

    return country;
}

/*
Method chooseTeamsRandomly():
Params:
- vector(pair(int, string)) - data of teams (team id, team name)
- int - ammount of teams to return

Returns:
- vector(pair(int, string)) - data of randomly chosen teams (team id, team name)
*/
std::vector<std::pair<int, std::string>> DataManager::chooseTeamsRandomly(std::vector<std::pair<int, std::string>>& teams, int ammount) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(teams.begin(), teams.end(), g);

    if (teams.size() < ammount)
        ammount = int(teams.size());

    std::vector<std::pair<int, std::string>> chosenTeams(teams.begin(), teams.begin() + ammount);
    return chosenTeams;
}


/*
Method getOpponentsTeams():
Params:
- pair(int, string) - data of the team chosen by player (team id, team name)

Returns:
- vector(pair(int, string)) - data of opponents teams (team id, team name)
*/
std::vector<std::pair<int, std::string>> DataManager::getOpponentsTeams(std::pair<int, std::string>& chosenTeam) {
    std::vector<std::pair<int, std::string>> teams = getTeams("teams.json");

    teams.erase(std::remove_if(teams.begin(), teams.end(), [&chosenTeam](std::pair<int, std::string> team) {
    return team == chosenTeam;}), teams.end());

    return chooseTeamsRandomly(teams, config::MAX_OPPONENTS);
}

/*
Method validData():

Returns:
- bool - true if data is valid (player's team and opponents teams are not empty)
*/
bool DataManager::validData() {
    bool valid = true;
    if (league.getPlayerClub().getTeam().getPlayers().empty()) {
        valid = false;
    }

    for (auto& opponent : league.getOpponentsClubs()) {
        if (opponent.getTeam().getPlayers().empty()) {
            valid = false;
        }
    }

    return valid;
}

/*
Method createSave():
Creates new save directory in saves folder.
*/
void DataManager::createSave() {
    std::filesystem::create_directory(savePath());
    return;
}

/*
Method save():
Saves game data to save file from current save folder.
*/
void DataManager::save() {
    std::string filePath = savePath() + "/save.json";
    std::ofstream file(filePath);

    if (!file) {
        std::cerr << config::FILE_OPENING_ERROR_MESSAGE << filePath << std::endl;
        return;
    }

    web::json::value jsonData = league.toJson();
    file << jsonData.serialize();
    file.close();
    return;
}

/*
Method loadSave():
Loads game data from save file from current save folder.
*/
void DataManager::loadSave() {
    std::string filePath = savePath() + "/save.json";
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << config::FILE_OPENING_ERROR_MESSAGE << filePath << std::endl;
        return;
    }

    web::json::value jsonData;
    file >> jsonData;

    league.fromJson(jsonData);
    file.close();
    return;
}

/*
Method loadLeagues():
Downloads leagues file from the API to current save folder.
*/
void DataManager::loadLeagues() {
    system("clear");
    std::cout << config::LOADING_LEAGUES_MESSAGE << std::endl;
    getFromApi("leagues/country/" + league.getCountry() + "/" + std::to_string(league.getSeason()), "leagues.json");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    system("clear");
}

/*
Method loadTeams():
Downloads teams file from the API to curren save folder.
*/
void DataManager::loadTeams() {
    system("clear");
    std::cout << config::LOADING_TEAMS_MESSAGE << std::endl;
    getFromApi("teams/league/" + std::to_string(league.getId()), "teams.json");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    system("clear");
}

/*
Method loadPlayers():
Downloads user's players file from the API to current save folder and adds them to the player's team.
*/
void DataManager::loadPlayers() {
    system("clear");
    std::cout << config::LOADING_PLAYERS_MESSAGE << std::endl;
    getFromApi("players/squad/" + std::to_string(league.getPlayerClub().getId()) + "/" + std::to_string(league.getSeason()), "playersAPI" + std::to_string(league.getPlayerClub().getId()) + ".json");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    system("clear");

    std::vector<std::vector<std::string>> playersData = getPlayers("playersAPI" + std::to_string(league.getPlayerClub().getId()) + ".json");

    for (const auto& playerData : playersData) {
        Player player(std::stoi(playerData[0]), playerData[1], playerData[2], std::stoi(playerData[3]), playerData[4], Statistics());
        league.getPlayerClub().getTeam().getPlayers().push_back(player);
    }
}

/*
Method loadOpponents():
Downloads opponents teams files from API to current save folder and adds them to oponnent's teams.
*/
void DataManager::loadOpponents(std::vector<std::pair<int, std::string>> chosenTeams) {
    for (const auto& team : chosenTeams) {
        OpponentClub opponentClub(team.first, team.second, int(league.getOpponentsClubs().size()));
        system("clear");
        std::cout << config::LOADING_PLAYERS_MESSAGE << std::endl;
        getFromApi("players/squad/" + std::to_string(opponentClub.getId()) + "/" + std::to_string(league.getSeason()), "playersAPI" + std::to_string(opponentClub.getId()) + ".json");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        system("clear");

        std::vector<std::vector<std::string>> playersData = getPlayers("playersAPI" + std::to_string(opponentClub.getId()) + ".json");
        for (const auto& playerData : playersData) {
            Player player(std::stoi(playerData[0]), playerData[1], playerData[2], std::stoi(playerData[3]), playerData[4], Statistics(opponentClub.getPriority()));
            opponentClub.getTeam().getPlayers().push_back(player);
        }

        league.getOpponentsClubs().push_back(opponentClub);
    }
}

/*
Method handleEvent():
Params:
- pointer for Event

Handles events passed by EventManager
*/
void DataManager::handleEvent(Event* event) {

    /* Handling event for creating new save */
    if (event->getType() == typeid(CreateNewSaveRequest)) {
        saveName = event->getString();
        if (saveExists()) {
            SaveAlreadyExistsResponse saveExistsEvent(saveName);
            sendEvent(&saveExistsEvent);
            return;
        }
        else {
            createSave();
            ChooseCountryRequest request;
            sendEvent(&request);
            return;
        }
    }

    /* Handlig event for deleting current save (in DataManager) */
    if (event->getType() == typeid(DeleteCurrentSaveRequest)) {
        std::filesystem::remove_all(savePath());
        league.clear();
        return;
    }

    /* Handling event for deleting save chosen by user in menu */
    if (event->getType() == typeid(DeleteSaveRequest)) {
        std::filesystem::remove_all(savesPath + event->getString());
        return;
    }

    /* Handling event for checking if game contains any saves to load */
    if (event->getType() == typeid(DoSavesExistsRequest)) {
        std::vector<std::string> savesNames = getSavesNames();
        DoSavesExistsResponse response(savesNames);
        sendEvent(&response);
        return;
    }

    /* Handling event for choosing country to play with by user in menu */
    if (event->getType() == typeid(ChooseCountryResponse)) {
        std::string chosenCountry = event->getString();
        if (countryExists(chosenCountry)) {
            league.setCountry(chosenCountry);
            ChooseSeasonRequest request;
            sendEvent(&request);
            return;
        } else {
            ThereIsNoSuchCountryResponse response;
            sendEvent(&response);
            return;
        }
    }

    /* Handling event for choosing season to play with by user in menu */
    if (event->getType() == typeid(ChooseSeasonResponse)) {
        int chosenSeason = event->getInteger();
        if(seasonExists(chosenSeason)) {
            league.setSeason(chosenSeason);
            loadLeagues();
            std::vector<std::pair<int, std::string>> leagues = getLeagues("leagues.json");
            ChooseLeagueRequest request(leagues);
            sendEvent(&request);
            return;
        } else {
            ThereIsNoSuchSeasonResponse response;
            sendEvent(&response);
            return;
        }
    }

    /* Handling event for choosing league to play with by user in menu */
    if (event->getType() == typeid(ChooseLeagueResponse)) {
        std::pair<int, std::string> chosenLeague = event->getPair();
        league.set(chosenLeague);

        loadTeams();

        std::vector<std::pair<int, std::string>> teams = getTeams("teams.json");
        ChooseTeamRequest request(teams);
        sendEvent(&request);

        return;
    }

    /* Handling event for choosing team to play with by user in menu */
    if (event->getType() == typeid(ChooseTeamResponse)) {
        std::pair<int, std::string> chosenTeam = event->getPair();
        league.getPlayerClub().set(chosenTeam);

        loadPlayers();

        loadOpponents(getOpponentsTeams(chosenTeam));

        if (!validData()) {
            league.clear();
            InvalidDataEvent event;
            sendEvent(&event);
            return;
        }

        save();

        StartGameEvent startGame(getLeague());
        sendEvent(&startGame);
        return;
    }

    /* Handling event for loading save data (league) to the game from JSON file */
    if (event->getType() == typeid(LoadSaveRequest)) {
        saveName = event->getString();
        loadSave();

        StartGameEvent startGame(getLeague());
        sendEvent(&startGame);
        return;
    }

    /* Handling event for saving current game data to JSON file */
    if (event->getType() == typeid(SaveGameRequest)) {
        save();
        SaveGameResponse response;
        sendEvent(&response);
        return;
    }

    /* Handling event for clearing league data */
    if (event->getType() == typeid(ClearLeagueRequest)) {
        league.clear();
        return;
    }

    return;
}