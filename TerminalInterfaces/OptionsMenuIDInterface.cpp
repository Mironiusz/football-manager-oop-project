#include "TerminalInterface.h"

#include <ncurses.h>
#include <stdexcept>

// Constructors for OptionsMenuIDInterface

OptionsMenuIDInterface::OptionsMenuIDInterface(std::vector<std::pair<int, std::string>>& opts) {
    createOptions(opts); // Create options from the vector of pairs
}

OptionsMenuIDInterface::OptionsMenuIDInterface(std::string mnTil, std::vector<std::pair<int, std::string>>& opts) : menuTitle(mnTil) {
    createOptions(opts); // Create options from the vector of pairs
}

OptionsMenuIDInterface::OptionsMenuIDInterface(std::string mnTil, std::vector<std::pair<int, std::string>>& opts, bool del) : menuTitle(mnTil), deletions(del) {
    createOptions(opts); // Create options from the vector of pairs
}

// Method to create options from a vector of pairs (ID and string)
void OptionsMenuIDInterface::createOptions(std::vector<std::pair<int, std::string>>& opts) {
    for (auto& option : opts) { // Loop through each pair in the vector
        IntegerOption newOption(option.second, option.first); // Create a new IntegerOption object
        menuOptions.push_back(newOption); // Add the new IntegerOption to the menuOptions vector
    }
}

// Method to display the menu and handle user input
IntegerOption OptionsMenuIDInterface::showMenu() {
    initscr(); // Initialize the ncurses screen
    cbreak(); // Disable line buffering
    noecho(); // Turn off automatic echoing of typed characters
    keypad(stdscr, TRUE); // Enable special keys to be interpreted by ncurses
    start_color(); // Enable color functionality

    // Initialize colors if not already initialized
    if (!has_colors()) {
        endwin();
        throw std::runtime_error("Terminal does not support colors");
    }

    initColors(); // Initialize color pairs

    curs_set(0); // Hide the cursor

    int character; // Variable to store the pressed key
    IntegerOption chosenOption; // Variable to store the chosen option

    // Calculate the number of visible options based on the terminal height
    int visibleOptions = LINES - 4; // Adjust as necessary for padding
    int firstVisible = 0; // Index of the first visible option

    while (chosenOption.isEmpty) { // Loop until an option is chosen
        clear(); // Clear the screen
        drawBorder(); // Draw a border around the screen
        printMenu(firstVisible, visibleOptions); // Print the menu options
        character = getch(); // Get the pressed key
        switch (character) {
            case KEY_UP: // If the up arrow key or 'w' is pressed
            case 'w':
                if (currentOption > 0) {
                    currentOption--; // Move the selection up
                } else {
                    currentOption = menuOptions.size() - 1; // Wrap around to the bottom
                }

                if (currentOption < firstVisible) {
                    firstVisible = currentOption;
                } else if (currentOption == menuOptions.size() - 1) {
                    firstVisible = menuOptions.size() - visibleOptions;
                    if (firstVisible < 0) firstVisible = 0;
                }
                break;
            case KEY_DOWN: // If the down arrow key or 's' is pressed
            case 's':
                if (currentOption < menuOptions.size() - 1) {
                    currentOption++; // Move the selection down
                } else {
                    currentOption = 0; // Wrap around to the top
                }

                if (currentOption >= firstVisible + visibleOptions) {
                    firstVisible = currentOption - visibleOptions + 1;
                } else if (currentOption == 0) {
                    firstVisible = 0;
                }
                break;
            case '\n': // If the Enter key is pressed
                chosenOption = menuOptions[currentOption]; // Choose the current option
                break;
            case KEY_BACKSPACE: // If the backspace key is pressed
                chosenOption.goBack = true; // Set the option state to GoBack
                break;
            case KEY_DC: // If the delete key or 'x' is pressed
            case 'x':
                if (withDeletions()) { // If deletions are allowed
                    chosenOption = menuOptions[currentOption]; // Choose the current option
                    chosenOption.toDelete = true; // Set the option state to Delete
                }
                break;
        }

        // Adjust firstVisible to ensure the current option is always visible
        if (currentOption < firstVisible) {
            firstVisible = currentOption;
        } else if (currentOption >= firstVisible + visibleOptions) {
            firstVisible = currentOption - visibleOptions + 1;
        }

        // Handle wrapping
        if (currentOption == 0) {
            firstVisible = 0;
        } else if (currentOption == menuOptions.size() - 1) {
            firstVisible = menuOptions.size() - visibleOptions;
            if (firstVisible < 0) firstVisible = 0;
        }
    }

    clear(); // Clear the screen
    endwin(); // End ncurses mode
    curs_set(1); // Restore the cursor
    return chosenOption; // Return the chosen option
}

// Method to print the menu to the terminal with scrolling support
void OptionsMenuIDInterface::printMenu(int firstVisible, int visibleOptions) {
    clear(); // Clear the screen
    attron(COLOR_PAIR(2)); // Turn on color pair 2

    // Define the padding
    int paddingY = 1; // Vertical padding (top and bottom)
    int paddingX = 2; // Horizontal padding (left and right)

    // Print the menu title with padding
    move(paddingY, paddingX); // Move the cursor to the padded position
    printw("%s\n", menuTitle.c_str()); // Print the menu title

    // Print the menu options with padding
    for (int i = firstVisible; i < firstVisible + visibleOptions && i < menuOptions.size(); ++i) { // Loop through each menu option
        move(paddingY + i - firstVisible + 1, paddingX); // Move the cursor to the padded position for each option
        if (i == currentOption) { // If the current option is selected
            attron(A_REVERSE); // Turn on reverse video mode to highlight the option
            printw("%s%s\n", prefix.c_str(), menuOptions[i].name.c_str()); // Print the option with the prefix
            attroff(A_REVERSE); // Turn off reverse video mode
        } else {
            printw("%s\n", menuOptions[i].name.c_str()); // Print the option without the prefix
        }
    }

    attroff(COLOR_PAIR(2)); // Turn off color pair 2
    drawBorder(); // Draw a border around the screen
    refresh(); // Refresh the screen to show the changes
}

// Method to check if deletions are allowed
bool OptionsMenuIDInterface::withDeletions() const {
    return deletions; // Return the deletions flag
}
