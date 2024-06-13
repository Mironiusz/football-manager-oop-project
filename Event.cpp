#include "Event.h"
#include <vector>

const std::type_info& Event::getType() const {
    return typeid(*this);
}

std::string Event::getString() const {
    return "";
}

std::vector<std::string> Event::getStrings() const {
    return {};
}

int Event::getInteger() const {
    return int();
}

bool Event::getBool() const {
    return bool();
}

std::vector<std::pair<int, std::string>> Event::getPairs() const {
    return {};
}

std::pair<int, std::string> Event::getPair() const {
    return {};
}

League& Event::getLeague() {
    League league;
    return league;
}

SendStringEvent::SendStringEvent(std::string s) : str(s) {}

std::string SendStringEvent::getString() const {
    return str;
}

SendStringVectorEvent::SendStringVectorEvent(std::vector<std::string> s) : strVec(s) {}

std::vector<std::string> SendStringVectorEvent::getStrings() const {
    return strVec;
}

SendIntegerEvent::SendIntegerEvent(int i) : integer(i) {}

int SendIntegerEvent::getInteger() const {
    return integer;
}

SendBoolEvent::SendBoolEvent(bool b) : boolean(b) {}

bool SendBoolEvent::getBool() const {
    return boolean;
}

SendPairEvent::SendPairEvent(std::pair<int, std::string> p) : pair(p) {}

std::pair<int, std::string> SendPairEvent::getPair() const {
    return pair;
}


SendPairVectorEvent::SendPairVectorEvent(std::vector<std::pair<int, std::string>> p) : pairs(p) {}

std::vector<std::pair<int, std::string>> SendPairVectorEvent::getPairs() const {
    return pairs;
}

SendLeagueEvent::SendLeagueEvent(League& l) : league(l) {}

League& SendLeagueEvent::getLeague() {
    return league;
}

CreateNewSaveRequest::CreateNewSaveRequest(std::string s) : SendStringEvent(s) {}

LoadSaveRequest::LoadSaveRequest(std::string s) : SendStringEvent(s) {}

SaveAlreadyExistsResponse::SaveAlreadyExistsResponse(std::string s) : SendStringEvent(s) {};

DoSavesExistsResponse::DoSavesExistsResponse(std::vector<std::string> s) : SendStringVectorEvent(s) {};

ChooseCountryResponse::ChooseCountryResponse(std::string s) : SendStringEvent(s) {};

ChooseSeasonResponse::ChooseSeasonResponse(int i) : SendIntegerEvent(i) {};

ChooseLeagueRequest::ChooseLeagueRequest(std::vector<std::pair<int, std::string>> p) : SendPairVectorEvent(p) {};

ChooseLeagueResponse::ChooseLeagueResponse(std::pair<int, std::string> p) : SendPairEvent(p) {};

ChooseTeamRequest::ChooseTeamRequest(std::vector<std::pair<int, std::string>> p) : SendPairVectorEvent(p) {};

ChooseTeamResponse::ChooseTeamResponse(std::pair<int, std::string> p) : SendPairEvent(p) {};

DeleteSaveRequest::DeleteSaveRequest(std::string s) : SendStringEvent(s) {};

StartGameEvent::StartGameEvent(League& l) : SendLeagueEvent(l) {}