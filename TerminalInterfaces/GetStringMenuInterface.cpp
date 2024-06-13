#include "TerminalInterface.h"

#include <ncurses.h>
#include <vector>
#include <sstream>

// Constructor for GetStringMenuInterface
GetStringMenuInterface::GetStringMenuInterface(std::string title) : menuTitle(title) {}

// Method to show the menu
Option GetStringMenuInterface::showMenu() {
    std::string forbiddenChars = "/?&;(){}[]|*~<>\"\'`\\^%$#@!"; // List of forbidden characters

    initscr(); // Initialize ncurses screen
    cbreak(); // Disable line buffering
    noecho(); // Disable automatic echoing of typed characters
    keypad(stdscr, TRUE); // Enable special keys in ncurses
    start_color(); // Enable color support

    // Check if the terminal supports colors
    if (!has_colors()) {
        endwin(); // End ncurses mode
        throw std::runtime_error("Terminal does not support colors");
    }

    initColors(); // Initialize color pairs
    curs_set(1); // Show the cursor

    int character; // Variable to store pressed key
    Option chosenOption; // Variable to store the chosen option
    std::string optionName = ""; // Variable to store the entered text

    // Split the title into lines
    std::vector<std::string> titleLines = splitTitle(menuTitle);

    while (chosenOption.isEmpty) { // Loop until an option is chosen
        clear(); // Clear the screen
        drawBorder(); // Draw the border
        attron(COLOR_PAIR(2)); // Enable color pair number 2

        // Display the menu title with padding
        int paddingY = 1; // Vertical padding (top and bottom)
        int paddingX = 2; // Horizontal padding (left and right)
        for (int i = 0; i < titleLines.size(); ++i) {
            move(paddingY + i, paddingX); // Move cursor to position with padding for each line
            printw("%s", titleLines[i].c_str()); // Display the title line
        }

        // Display the entered text
        move(paddingY + titleLines.size() + 1, paddingX); // Move cursor below the title
        printw("%s", optionName.c_str()); // Display the entered text

        attroff(COLOR_PAIR(2)); // Disable color pair number 2
        refresh(); // Refresh the screen to display changes

        character = getch(); // Get the pressed key
        switch (character) {
            case '\n': // If Enter is pressed
                if (optionName.find_first_of(forbiddenChars) == std::string::npos) {
                    chosenOption.setName(optionName); // Set option name if it does not contain forbidden characters
                } else {
                    chosenOption.switchToInvalidData(); // Switch to invalid data state
                }
                break;
            case KEY_BACKSPACE:
            case 127: // If Backspace is pressed
                if (!optionName.empty()) {
                    optionName.pop_back(); // Remove the last character from the entered text
                } else {
                    chosenOption.switchToGoBack(); // Switch to go back state
                }
                break;
            case 27: // ESC key
                chosenOption.switchToGoBack(); // Switch to go back state
                break;
            default:
                if (character >= 32 && character <= 126) { // Printable ASCII characters
                    optionName += character; // Add character to the entered text
                }
                break;
        }
    }

    clear(); // Clear the screen
    endwin(); // End ncurses mode
    return chosenOption; // Return the chosen option
}
