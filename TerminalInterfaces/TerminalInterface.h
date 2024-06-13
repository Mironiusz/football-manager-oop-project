#pragma once

#include <vector>
#include <initializer_list>
#include <string>
#include <filesystem>
#include <utility> // for std::pair
#include <sstream> // for stringstream
#include <ncurses.h> // for terminal handling

#include "../Player.h"

struct Option {
    std::string name = "";
    bool invalidData = false;
    bool goBack = false;
    bool toDelete = false;
    bool isEmpty = true;

    Option() = default;
    Option(std::string opt);

    operator std::string() const;

    void setName(std::string newName);
    void switchToDelete();
    void switchToGoBack();
    void switchToInvalidData();
};

struct IntegerOption : public Option {
    int value = 0;

    IntegerOption() = default;
    IntegerOption(std::string opt, int val);

    operator int() const;
    operator std::pair<int, std::string>() const;

    void setValue(int newValue);
};

struct PlayerOption : public Option {
    Player player;

    PlayerOption() = default;
    PlayerOption(Player& pl);
};

class TerminalInterface {
    public:
        TerminalInterface();

        void showWarning(const std::string& warning) const;

    protected:
        void initColors() const;
        void drawBorder() const;
        void drawLoadingBar(int percentage) const;

        inline std::vector<std::string> splitTitle(const std::string& title) const {
            std::vector<std::string> lines;
            std::stringstream ss(title);
            std::string line;

            while (std::getline(ss, line)) {
                lines.push_back(line);
            }

            return lines;
        }
};

class OptionsMenuInterface : public TerminalInterface {
    protected:
        std::vector<Option> menuOptions;
        std::string menuTitle = "FOOTBALL MANAGER";
        std::string prefix = "-> ";
        int currentOption = 0;
        bool deletions = false;

    public:
        OptionsMenuInterface() = default;
        OptionsMenuInterface(std::initializer_list<std::string> opts);
        OptionsMenuInterface(std::string mnTil, std::initializer_list<std::string> opts);
        OptionsMenuInterface(std::vector<std::string>& opts);
        OptionsMenuInterface(std::string mnTil, std::vector<std::string>& opts);
        OptionsMenuInterface(std::string mnTil, std::vector<std::string>& opts, bool del);

        void createOptions(std::initializer_list<std::string>& opts);
        void createOptions(std::vector<std::string>& opts);

        Option showMenu();
        void printMenu(int firstVisible, int visibleOptions); // Updated signature to handle scrolling
        bool withDeletions() const;
};

class OptionsMenuIDInterface : public OptionsMenuInterface {
    protected:
        std::vector<IntegerOption> menuOptions;
        std::string menuTitle = "FOOTBALL MANAGER";
        bool deletions = false;
    public:
        OptionsMenuIDInterface() = default;
        OptionsMenuIDInterface(std::vector<std::pair<int, std::string>>& opts);
        OptionsMenuIDInterface(std::string mnTil, std::vector<std::pair<int, std::string>>& opts);
        OptionsMenuIDInterface(std::string mnTil, std::vector<std::pair<int, std::string>>& opts, bool del);

        void createOptions(std::vector<std::pair<int, std::string>>& opts);

        IntegerOption showMenu();
        void printMenu(int firstVisible, int visibleOptions); // Updated signature to handle scrolling
        bool withDeletions() const;
};

class OptionsMenuPlayerInterface : public OptionsMenuInterface {
    protected:
        std::vector<PlayerOption> menuOptions;
        std::string menuTitle = "FOOTBALL MANAGER";
        bool deletions = false;
    public:
        OptionsMenuPlayerInterface() = default;
        OptionsMenuPlayerInterface(std::vector<Player>& opts);
        OptionsMenuPlayerInterface(std::string mnTil, std::vector<Player>& opts);
        OptionsMenuPlayerInterface(std::string mnTil, std::vector<Player>& opts, bool del);

        void createOptions(std::vector<Player>& opts);

        PlayerOption showMenu();
        void printMenu(int firstVisible, int visibleOptions); // Updated signature to handle scrolling
        bool withDeletions() const;
};

class GetStringMenuInterface : public TerminalInterface {
    private:
        std::string menuTitle;

    public:
        GetStringMenuInterface(std::string mnTil);
        ~GetStringMenuInterface() = default;

        Option showMenu();
};

class GetIntegerMenuInterface : public TerminalInterface {
    private:
        std::string menuTitle;
    public:
        GetIntegerMenuInterface(std::string mnTil);
        ~GetIntegerMenuInterface() = default;

        IntegerOption showMenu();
};
