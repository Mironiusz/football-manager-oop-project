#pragma once

#include "EventObject.h"
#include "EventManager.h"
#include "Config.h"
#include "League.h"
#include "Player.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <fstream>
#include <iostream>
#include <string>

/*
Class DataManager:

Responsible for managing data. It contains the league and methods to get data from the API, save and load data, and create a save.

Attributes:
- saveName: name of current save
- league: league which contains save data
- apiKey: API key
- apiHost: API host
- apiUrl: API URL
- dataPath: path to data files
- savesPath: path to saves files
*/
class DataManager : public EventObject {

    private:
    std::string saveName;

    League league;

    std::string apiKey = config::API_KEY;
    std::string apiHost = config::API_HOST;
    std::string apiUrl = config::API_URL;
    std::string dataPath = config::DATA_FILE_PATH;
    std::string savesPath = config::SAVES_FILE_PATH;

    int getRemainingRequests(const web::http::http_response& response);

    public:
        DataManager(EventManager* em);

        void getFromApi(std::string endpoint, std::string fileName);

        std::vector<std::string> getCountriesNames();

        std::vector<int> getSeasons();

        std::vector<std::pair<int, std::string>> getLeagues(const std::string& fileName);

        std::vector<std::pair<int, std::string>> getTeams(const std::string& fileName);

        std::vector<std::vector<std::string>> getPlayers(const std::string& fileName);

        League& getLeague();

        void setSaveName(std::string svNm);

        std::string savePath() const;

        bool savesExists() const;

        bool saveExists() const;

        bool countryExists(std::string& country);

        bool seasonExists(int season);

        std::vector<std::string> getSavesNames() const;

        std::string formatCountryName(std::string& country);

        std::vector<std::pair<int, std::string>> chooseTeamsRandomly(std::vector<std::pair<int, std::string>>& teams, int ammount);

        std::vector<std::pair<int, std::string>> getOpponentsTeams(std::pair<int, std::string>& chosenTeam);

        bool validData();

        void createSave();

        void save();

        void loadSave();

        void loadLeagues();

        void loadTeams();

        void loadPlayers();

        void loadOpponents(std::vector<std::pair<int, std::string>> chosenTeams);

        void handleEvent(Event* event) override;
};