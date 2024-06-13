#pragma once

#include <string>

/*
Namespace config:

Contains all the constants used in the game.
*/
namespace config {

    /* GAME OPTIONS */
    const int MAX_OPPONENTS = 4;
    const int MAX_ACTION_POINTS = 5;
    const int MAX_DEFEATS = 10;

    const int MIN_STATS = 1;
    const int MAX_BASIC_STATS = 5;
    const int MAX_STATS = 20;
    const int BASE_INCREASE = 3;
    const int MAX_INCREASE = 5;

    /* LINEUP OPTIONS */
    const int MAX_ATTACKERS = 2;
    const int MAX_MIDFIELDERS = 4;
    const int MAX_DEFENDERS = 4;
    const int MAX_GOALKEEPERS = 1;

    /* FILE PATHS */
    const std::string DATA_FILE_PATH = "./data/";
    const std::string SAVES_FILE_PATH = "./saves/";

    /* API */
    const std::string API_KEY = "06e3d28899mshd220c9cc3d3912cp16137ajsn3fa62ae003e2";
    const std::string API_HOST = "api-football-v1.p.rapidapi.com";
    const std::string API_URL = "https://api-football-v1.p.rapidapi.com/v2/";

    /* IN-GAME MESSAGES */
    const std::string REQUESTS_MESSAGE = "API REQUESTS LEFT: ";
    const std::string NO_REQUEST_LEFT_MESSAGE = "NO REQUESTS LEFT, CANNOT GET DATA FROM API!\n WAIT UNTIL TOMMOROW!";
    const std::string STATUS_ERROR_MESSAGE = "ERROR! STATUS CODE: ";
    const std::string SAVING_FILE_MESSAGE = "SAVING FILE TO: ";
    const std::string FILE_OPENING_ERROR_MESSAGE = "CANNOT OPEN FILE: ";
    const std::string SAVING_FILE_SUCCESS_MESSAGE = "FILE SAVED SUCCESSFULLY: ";
    const std::string WRONG_JSON_FIELDS_MESSAGE = "REQUESTED JSON FIELDS NOT FOUND";

    const std::string LOADING_LEAGUES_MESSAGE = "DOWNLOADING LEAGUES FROM API...";
    const std::string LOADING_TEAMS_MESSAGE = "DOWNLOADING TEAMS FROM API...";
    const std::string LOADING_PLAYERS_MESSAGE = "DOWNLOADING PLAYERS FROM API...";

    const std::string SAVE_EXISTS_MESSAGE = "THAT SAVE ALREADY EXISTS, DO YOU WANT TO OVERRIDE IT?";

    const std::string LINEUP_SUCCESSFUL_MESSAGE = "PLAYER SUCCESFULLY ADDED TO LINEUP!";
    const std::string LINEUP_UNSUCCESFFUL_MESSAGE = "REACHED MAXIMUM NUMBER OF PLAYERS OF THAT POSITION IN LINEUP!";
    const std::string LINEUP_FULL_MESSAGE = "LINEUP SUCCESFULLY CREATED!";

    const std::string NO_ACTION_POINTS_MESSAGE = "NO ACTION POINTS LEFT!";

    const std::string SAVING_MESSAGE = "GAME SAVED SUCCESSFULLY!";

    const std::string INVALID_API_DATA_MESSAGE = "INVALID PLAYERS DATA, API HAS GIVEN EMPTY RESPOSNSE!\nSTART A NEW GAME!";

    /* MENU TITLES */
    const std::string BASIC_MENU_TITLE = "FOOTBALL SIMULATOR";

    const std::string LINEUP_MENU_TITLE = "CREATE YOUR LINEUP";

    /* POSITIONS NAMES */
    const std::string ATTACKER = "Attacker";
    const std::string MIDFIELDER = "Midfielder";
    const std::string DEFENDER = "Defender";
    const std::string GOALKEEPER = "Goalkeeper";

};