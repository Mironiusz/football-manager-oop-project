# PROJEKT PROI - FOOTBALL MANAGER GAME

## UPROSZCZONY OPIS GRY

Gra ma na celu uproszczoną symulację meczy piłkarskich i uczestnictwa w ligach na zasadach pseudolosowości.

Gracz w grze decyduje o:
- wyborze sezonu
- wyborze ligii
- wyborze swojego klubu
- wyborze składów meczowych
- wyborze piłkarzy, którzy będą trenować

Wyniki meczy generowane są losowo na podstawie statystyk piłkarzy.  Możemy symulować dowolną dostępną ligę w dowolnym dostępnym sezonie z losowymi przeciwnikami z danej ligii. Aby zwyciężyć (lub przegrać) musimy rozgrywać mecze, jeśli pokonamy wszystkich przeciwników w lidze gracz wygrywa i może rozpocząć nową grę z inną ligą i klubem.

Przegrywamy w momencie kiedy przegramy dozwoloną ilość razy. W takim przypadku również gracz może rozpocząć nową grę z inną ligą i klubem.

## POBIERANIE, MAGAZYNOWANIE I OBSŁUGIWANIE DANYCH

Wszelkie dane potrzebne do funkcjonowania gry trzymane są folderze data lub pobierane z API w formie plików JSON, które następnie obłusgiwane są przez dane klasy poprzez bibliotekę cpprest.
API jest płatne i jego darmowa wersja jest ograniczona do 100 zapytań dziennie stąd liczba możliwych do rozegrania gier jest mała, ale w celach projektowych jest to wystarczająco.

## INTERFEJS

Interfejs gry jest stworzony za pomocą zwykłego termina z usprawnieniami dzięki bibliotece ncurses. Elementy ncurses zawierają specjalne obiekty z grupy TerminalInterfaces (hierarchia klas), które generują obraz i tylko go wyświetlają bądź w przypadku menu zwracają wybraną bądź wpisaną przez użytkownika opcję innym obiektom.

## TESTOWANIE

Ze względu na to, że gra opiera się na losowości i interfejsie terminalowym biblioteki zewnętrznej nie było zbytniej możliwości napisania porządnych testów jednostkowych więc wszelkie przypadki były sprawdzane i testowane poprzez zmianę danych parametrów w sekcji konfiguracji bądź w samym kodzie programu.

## SZCZEGÓŁOWY OPIS GRY

Gra opiera się na wyborach gracza poprzez różnego rodzaju menu.

Daną opcję lub działanie zatwierdzamy przyciskiem "Enter".

Cofnąć możemy się za pomocą przycisku "BackSpace".

Usunąć daną opcję (o ile menu przewiduje taką możliwość) możemy za pomocą przycisku "x" bądź "delete".

Po uruchomieniu gry wyświetla nam się menu startowe z opcjami:

- NEW GAME

- LOAD GAME

- QUIT GAME

Po kliknięciu NEW GAME:
- tworzy się nowy save, którego nazwę trzeba wpisać z klawiatury
(jeśli taki już istnieje zostaniemy zapytani czy go nadpisać)
- wybieramy kraj (lub world dla globalnych) z którego ligę chcemy wybrać (automatyczne formatowanie na format API)
- wybieramy dany sezon z dostępnych
- wybieramy ligę z tych pobranych przez API na podstawie wcześniejszych wyborów
- wybieramy klub z tych pobranych przez API, którym będziemy grać
- losowane są kluby przeciwników

W przypadku złego inputu bądź błędych danych (np. zna nazwa kraju, zły sezon, liga nie zawiera klubów, klub nie zawiera graczy - API nie zawsze jest kompletne) program to wykryje i zostaniemy cofnięci.

Po kliknięciu LOAD GAME:
- jeśli jakieś zapisy istnieją będziemy mogli wybrać sobie z listy który wczytać lub usunąć
- jeśli zapisów nie ma to zostaniemy zapytani czy chcemy jakiś stworzyć

Po utworzeniu nowej gry bądź załadowaniu save'a dalej jest już to samo.

Wybieramy nasz "Lineup" (skład którym będziemy grać) z piłkarzy z naszego klubu. Potem otwiera się nam menager clubu w którym mamy kilka opcji, takich jak:

- CHANGE LINEUP - pozwala zmienić nasz lineup

- BROWNSE LINEUP - pozwala przeglądać nasz lineup (po wybraniu danego gracza możemy zobaczyć szczeogółowe informacje na jego temat)

- BROWSE PLAYERS - pozwala przeglądać graczy naszego klubu (po wybraniu danego gracza możemy zobaczyć szczeogółowe informacje na jego temat)

- PLAY MATCH

- VIEW LEAGUE TABLE - pozwala zobaczyć tabelę ligii, czyli listę klubów z którymi przyjdzie nam się zmierzyć i czy je pokonaliśmy czy nie

- SEND FOR TRAINING - pozwala wysłać piłkarza na trening, czyli zwiększyć jego losową statystykę o losowa wartość

- GO TO MID MENU - wyświetla pośrednie menu, które pozwala zapisać grę i do niej wrócić bądź wrócić do głównego menu

Dodatkowo mamy takie mechaniki jak punkty akcji i dozwolone przegrane.

W zamian za punkty akcji możemy zmienić lineup bądź wysłać piłkarza na trening (odnawiają się co mecz).

Dozwolone przegrany to ilość razy ile możemy przegrać mecz tak, aby nie odpaść z rogrywki.

Mecz rozgrywany jest automatycznie (wyniki są losowane na podstawie statystyk piłkarzy).

Podczas jego trwania widzimy jedynie komunikaty kto strzelił bramkę. Po strzeleniu bramki zwiększa się losowa statystyka piłkarza.

W przypadku wygranej następny mecz będziemy rozgrywać z następnym klubem w lidze.

W przypadku przegranej nasza liczba dozwolonych przegranych zmniejsza się i będziemy grać z tym klubem do skutku lub wyczerpaniu się limitu przegranych.

W przypadku remisu (bo istnieje na niego jakaś szansa) rozgrywane są strzały karne, które determinują to kto zwycieży.


## GŁÓWNE KLASY I SPOSÓB ICH KOMUNIKACJI

W grze mamy cztery, a dokładnie trzy główne klasy, które za nią odpowiadają. Są to:
- klasa gry - służy jako opakowanie dla pozostałych trzech klas
- klasa menu - służy do pobierania inputu od użytkownika i wyświetlania interfejsu
- klasa data manager - trzyma dane takie jak liga czy gracze, pobiera je z api, formatuje i obsługuje - najważniejsza klasa w programie
- klasa club manager - obsługuje klub, to w nim rozgrywane są mecze, wybierane lineup'y itd., w nim toczy się cała gra po wczytaniu odpowiednich danych

Obiektu komunikują się ze sobą na podstawie pseudo wzorca projektowego Mediatora (pseudo, bo zaimplementowany według naszej wariacji). Mianowicie klasy te dziedziczą po klasie abstrakcyjnej "Event Object", która to jest podłączona do "Event Managera". Komunikacją więc opiera się na tym, że obiekty emitują z siebie "eventy", które zawierają w sobie dane (same w sobie też są daną) do menadżera, a ten przekazuje je do innych obiektów do niego podłączonych. Informacja jaki event został przekazany i tym samym jak na niego zareagować odbywa się po typeid w związku z tym, mamy jedną klasę abstrakcyjną event po której wszystkie inne rodzaje dziedziczą (dlatego jest ich dużo). Nazwa klasy jest jej identyfikatorem. Na tym w głównej mierze polega wzorzec Mediatora więc wymagania w większym stopniu zostały spełnione.

## KLASY REPREZENTUJĄCE "ŚWIAT RZECZYWISTY - PIŁKARSKI"

Takie klasy to:
- League
- Club
- Team
- Lineup
- Player
- Match

Większość tych klas służy jako kontener na odpowiednie dane (jeśli posiadają jakieś metody to te klasy, o których wspomanie było wyżej je wywołują). Mogą posiadać pomocnicze struktury (np. gracz dla statystyk).

Wyjątkiem jest tutaj klasa meczu, bo sama w sobie zawiera logikę jego rozegrania, wystarczy podać klub gracza i oponenta i wywołać jedną metodę.

## UŻYTE BIBLIOTEKI

Tak jak wcześniej było wspomniane użyte zostały biblioteki takie jak ncurses do interfejsu terminalowego oraz cpprest do obsługi plików JSON i API. Oprócz tego wykorzystane zostały standardowe mechanizmy biblioteki standardowej. Z mniej powszechnych korzystaliśmy z modułu "random", "filesystem" czy "typeinfo".

## ZAŁOŻENIA, A ICH REALIZACJA

Model rozgrywki jest dość uproszczony. Brak w nim między innymi mechaniki fauli, asyst, czy czerwonych i żółtych kartek czy historii gier, które miały się w nim znaleźć, aczkolwiek zabrakło na to czasu. Jednakże obecny stan rzeczy biorąc pod uwagę nasze umiejętności i możliwości czasowe jest naszym zdaniem zadawalający.