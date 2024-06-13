#include "TerminalInterface.h"

#include <ncurses.h>
#include <unistd.h>
#include <filesystem>
#include <sstream>

// Definition of Option methods

Option::Option(std::string opt) : name(opt) {
    if (name != "") {
        isEmpty = false;
    }
}

Option::operator std::string() const {
    return name;
}

void Option::setName(std::string newName) {
    name = newName;
    isEmpty = false;
}

void Option::switchToDelete() {
    toDelete = true;
    isEmpty = false;
}

void Option::switchToGoBack() {
    goBack = true;
    isEmpty = false;
}

void Option::switchToInvalidData() {
    invalidData = true;
    isEmpty = false;
}

// Definition of IntegerOption methods

IntegerOption::IntegerOption(std::string opt, int val) : Option(opt), value(val) {
    if (name != "") {
        isEmpty = false;
    }
}

IntegerOption::operator int() const {
    return value;
}

IntegerOption::operator std::pair<int, std::string>() const {
    return std::make_pair(value, name);
}

void IntegerOption::setValue(int newValue) {
    value = newValue;
    isEmpty = false;
}

// Definition of PlayerOption methods

PlayerOption::PlayerOption(Player& pl) : player(pl) {
    if (player.getName() != "") {
        isEmpty = false;
    }
}

// Definition of TerminalInterface methods

TerminalInterface::TerminalInterface() {
    setlocale(LC_ALL, "");
    initscr(); // Initialize the ncurses screen
    start_color(); // Start color functionality in ncurses
    initColors(); // Initialize the color pairs
}

void TerminalInterface::showWarning(const std::string& warning) const {
    initscr(); // Initialize the ncurses screen
    cbreak(); // Disable line buffering
    noecho(); // Turn off automatic echoing of typed characters
    keypad(stdscr, TRUE); // Enable special keys to be interpreted by ncurses
    clear(); // Clear the screen

    attron(COLOR_PAIR(1)); // Turn on the color pair 1 (red text)
    printw("%s", (warning + "\n").c_str()); // Print the warning message
    attroff(COLOR_PAIR(1)); // Turn off the color pair 1
    refresh(); // Refresh the screen to show the changes

    int character;
    while ((character = getch()) != '\n') {} // Wait for the user to press Enter
    clear(); // Clear the screen
    endwin(); // End ncurses mode
}

void TerminalInterface::initColors() const {
    init_pair(1, COLOR_RED, COLOR_BLACK); // Define color pair 1: red text on black background
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Define color pair 2: green text on black background
    init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Define color pair 3: yellow text on black background
    init_pair(4, COLOR_BLUE, COLOR_BLACK); // Define color pair 4: blue text on black background
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK); // Define color pair 5: magenta text on black background
    init_pair(6, COLOR_CYAN, COLOR_BLACK); // Define color pair 6: cyan text on black background
    init_pair(7, COLOR_WHITE, COLOR_BLACK); // Define color pair 7: white text on black background
}

void TerminalInterface::drawBorder() const {
    box(stdscr, 0, 0); // Draw a box around the screen
    refresh(); // Refresh the screen to show the changes
}

void TerminalInterface::drawLoadingBar(int percentage) const {
    int width = COLS - 4; // Width of the loading bar (4 characters less than the screen width)
    int pos = (percentage * width) / 100; // Calculate the position of the loading bar based on the percentage
    move(LINES / 2, 2); // Move the cursor to the center of the screen
    for (int i = 0; i < width; ++i) { // Loop through the width of the loading bar
        if (i < pos) { // If the current position is less than the calculated position
            addch('='); // Add a '=' character to the loading bar
        } else {
            addch(' '); // Add a space character to the loading bar
        }
    }
    refresh(); // Refresh the screen to show the changes
}
