#pragma once

#include <string>
#include <typeinfo>
#include <vector>

#include "EventObject.h"
#include "League.h"


/*
Class Event:

Abstract class for events to inherit from. Contains methods to get different types of data.
Typeid gives informations for EventObjects which event is sent and how to handle it.
*/
class Event {
    public:
        Event() = default;

        virtual ~Event() = default;

        virtual const std::type_info& getType() const;

        virtual std::string getString() const;

        virtual std::vector<std::string> getStrings() const;

        virtual int getInteger() const;

        virtual bool getBool() const;

        virtual std::pair<int, std::string> getPair() const;

        virtual std::vector<std::pair<int, std::string>> getPairs() const;

        virtual League& getLeague();
};

class SendStringEvent : public Event {
    protected:
        std::string str;

    public:
        SendStringEvent(std::string str);

        std::string getString() const override;
};

class SendStringVectorEvent : public Event {
    protected:
        std::vector<std::string> strVec;

    public:
        SendStringVectorEvent(std::vector<std::string> strVec);

        std::vector<std::string> getStrings() const override;
};

class SendIntegerEvent : public Event {
    protected:
        int integer;

    public:
        SendIntegerEvent(int i);

        virtual int getInteger() const override;
};

class SendBoolEvent : public Event {
    protected:
        bool boolean;

    public:
        SendBoolEvent(bool b);

        virtual bool getBool() const override;
};

class SendPairEvent : public Event {
    protected:
        std::pair<int, std::string> pair;

    public:
        SendPairEvent(std::pair<int, std::string> p);

        virtual std::pair<int, std::string> getPair() const override;
};


class SendPairVectorEvent : public Event {
    protected:
        std::vector<std::pair<int, std::string>> pairs;

    public:
        SendPairVectorEvent(std::vector<std::pair<int, std::string>> p);

        virtual std::vector<std::pair<int, std::string>> getPairs() const override;
};

class SendLeagueEvent : public Event {
    protected:
        League& league;

    public:
        SendLeagueEvent(League& l);

        League& getLeague();
};

class CreateNewSaveRequest : public SendStringEvent {
    public:
        CreateNewSaveRequest(std::string s);
};

class LoadSaveRequest : public SendStringEvent {
    public:
        LoadSaveRequest(std::string s);
};

class SaveAlreadyExistsResponse : public SendStringEvent {
    public:
        SaveAlreadyExistsResponse(std::string s);
};

class DoSavesExistsRequest : public Event {};

class DoSavesExistsResponse : public SendStringVectorEvent {
    public:
        DoSavesExistsResponse(std::vector<std::string> s);
};

class SaveGameRequest : public Event {};

class SaveGameResponse : public Event {};

class ContinueGameEvent : public Event {};

class QuitGameEvent : public Event {};

class DeleteCurrentSaveRequest : public  Event {};

class AreThereAnySavesRequest : public Event {};

class ChooseCountryRequest : public Event {};

class ChooseCountryResponse : public SendStringEvent {
    public:
        ChooseCountryResponse(std::string s);
};

class ThereIsNoSuchCountryResponse : public Event {};

class ChooseSeasonRequest : public Event {};

class ChooseSeasonResponse : public SendIntegerEvent {
    public:
        ChooseSeasonResponse(int i);
};

class ThereIsNoSuchSeasonResponse : public Event {};

class ChooseLeagueRequest : public SendPairVectorEvent {
    public:
        ChooseLeagueRequest(std::vector<std::pair<int, std::string>> p);
};

class ChooseLeagueResponse : public SendPairEvent {
    public:
        ChooseLeagueResponse(std::pair<int, std::string> p);
};

class ChooseTeamRequest : public SendPairVectorEvent {
    public:
        ChooseTeamRequest(std::vector<std::pair<int, std::string>> p);
};

class ChooseTeamResponse : public SendPairEvent {
    public:
        ChooseTeamResponse(std::pair<int, std::string> p);
};

class DeleteSaveRequest : public SendStringEvent {
    public:
        DeleteSaveRequest(std::string s);
};

class StartGameEvent : public SendLeagueEvent {
    public:
        StartGameEvent(League& l);
};

class EndGameEvent : public Event {};

class GoBackToMainMenuEvent : public Event {};

class OpenMidMenuEvent : public Event {};

class ClearLeagueRequest : public Event {};

class InvalidDataEvent : public Event {};