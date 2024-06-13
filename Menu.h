#include "EventObject.h"
#include "Event.h"

class Menu : public EventObject {

    public:
        Menu(EventManager* em);

        void showMainMenu();

        void showMidMenu();

        void newGame();

        void loadGame();

        void saveGame();

        void continueGame();

        void chooseCountry();

        void chooseSeason();

        void deleteCurrentSaveAndGoBack();

        void handleEvent(Event* event) override;
};
