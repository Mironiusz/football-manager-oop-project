cmake_minimum_required(VERSION 3.11.0)
project(stack VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(PROJECT_SOURCES Config.h Event.h Event.cpp
    EventManager.h EventManager.cpp
    EventObject.h EventObject.cpp
    Game.h Game.cpp
    Menu.h Menu.cpp
    TerminalInterfaces/TerminalInterface.h TerminalInterfaces/TerminalInterface.cpp
    TerminalInterfaces/OptionsMenuInterface.cpp TerminalInterfaces/GetStringMenuInterface.cpp TerminalInterfaces/OptionsMenuPlayerInterface.cpp
    TerminalInterfaces/GetIntegerMenuInterface.cpp TerminalInterfaces/OptionsMenuIDInterface.cpp DataManager.h DataManager.cpp ClubManager.h ClubManager.cpp
    League.h League.cpp Club.h Club.cpp Team.h Team.cpp Lineup.h Lineup.cpp Player.h Player.cpp Match.h Match.cpp)

add_executable(Game main.cpp ${PROJECT_SOURCES})

# Znajdź i dodaj biblioteki cpprestsdk i Boost
find_package(Boost 1.58.0 REQUIRED COMPONENTS system thread)
find_package(cpprestsdk REQUIRED)

# Dodaj ścieżki do katalogów nagłówkowych i bibliotek
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# Linkuj biblioteki dla targetu Game
target_link_libraries(Game PUBLIC cpprest ${Boost_LIBRARIES} ${CURSES_LIBRARIES} crypto ssl)

file(COPY data/countries.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY data/seasons.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

# # Dodanie testów
# add_executable(Tests UnitTests.cpp ${PROJECT_SOURCES})
# target_link_libraries(Tests PUBLIC gtest_main cpprest ${Boost_LIBRARIES} ${CURSES_LIBRARIES} crypto ssl)
# enable_testing()
# include(GoogleTest)
# gtest_discover_tests(Tests)
