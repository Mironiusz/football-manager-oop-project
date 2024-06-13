# PROJEKT PROI - FOOTBALL MANAGER GAME

## Autorzy
- Rafał Mironko
- Amadeusz Lewandowski

## Uproszczony Opis Gry

Gra ma na celu uproszczoną symulację meczów piłkarskich i uczestnictwa w ligach na zasadach pseudolosowości.

Gracz w grze decyduje o:
- wyborze sezonu,
- wyborze ligi,
- wyborze swojego klubu,
- wyborze składów meczowych,
- wyborze piłkarzy, którzy będą trenować.

Wyniki meczów generowane są losowo na podstawie statystyk piłkarzy. Możemy symulować dowolną dostępną ligę w dowolnym dostępnym sezonie z losowymi przeciwnikami z danej ligi. Aby zwyciężyć (lub przegrać), musimy rozgrywać mecze. Jeśli pokonamy wszystkich przeciwników w lidze, gracz wygrywa i może rozpocząć nową grę z inną ligą i klubem.

Przegrywamy, gdy przegramy dozwoloną ilość razy. W takim przypadku gracz może również rozpocząć nową grę z inną ligą i klubem.

## Pobieranie, Magazynowanie i Obsługiwanie Danych

Wszelkie dane potrzebne do funkcjonowania gry są przechowywane w folderze `data` lub pobierane z API w formie plików JSON, które następnie są obsługiwane przez odpowiednie klasy za pomocą biblioteki cpprest. API jest płatne i jego darmowa wersja jest ograniczona do 100 zapytań dziennie, co ogranicza liczbę możliwych do rozegrania gier, ale jest to wystarczające do celów projektowych.

## Interfejs

Interfejs gry jest stworzony za pomocą zwykłego terminala z usprawnieniami dzięki bibliotece ncurses. Elementy ncurses zawierają specjalne obiekty z grupy `TerminalInterfaces` (hierarchia klas), które generują obraz i tylko go wyświetlają bądź w przypadku menu zwracają wybraną lub wpisaną przez użytkownika opcję innym obiektom.

## Testowanie

Ze względu na to, że gra opiera się na losowości i interfejsie terminalowym biblioteki zewnętrznej, nie było zbytniej możliwości napisania porządnych testów jednostkowych, więc wszelkie przypadki były sprawdzane i testowane poprzez zmianę danych parametrów w sekcji konfiguracji bądź w samym kodzie programu.

## Szczegółowy Opis Gry

Gra opiera się na wyborach gracza poprzez różnego rodzaju menu.

Daną opcję lub działanie zatwierdzamy przyciskiem "Enter".

Cofnąć się możemy za pomocą przycisku "BackSpace".

Usunąć daną opcję (o ile menu przewiduje taką możliwość) możemy za pomocą przycisku "x" bądź "delete".

### Menu Startowe

Po uruchomieniu gry wyświetla się menu startowe z opcjami:
- NEW GAME
- LOAD GAME
- QUIT GAME

#### NEW GAME
- Tworzy się nowy save, którego nazwę trzeba wpisać z klawiatury (jeśli taki już istnieje, zostaniemy zapytani, czy go nadpisać).
- Wybieramy kraj (lub `world` dla globalnych), z którego ligę chcemy wybrać (automatyczne formatowanie na format API).
- Wybieramy dany sezon z dostępnych.
- Wybieramy ligę z tych pobranych przez API na podstawie wcześniejszych wyborów.
- Wybieramy klub z tych pobranych przez API, którym będziemy grać.
- Losowane są kluby przeciwników.

W przypadku złego inputu bądź błędnych danych (np. zła nazwa kraju, zły sezon, liga nie zawiera klubów, klub nie zawiera graczy - API nie zawsze jest kompletne) program to wykryje i zostaniemy cofnięci.

#### LOAD GAME
- Jeśli jakieś zapisy istnieją, będziemy mogli wybrać z listy, który wczytać lub usunąć.
- Jeśli zapisów nie ma, zostaniemy zapytani, czy chcemy jakiś stworzyć.

### Menadżer Klubu

Po utworzeniu nowej gry bądź załadowaniu save'a dalej jest już to samo. Wybieramy nasz "Lineup" (skład, którym będziemy grać) z piłkarzy z naszego klubu. Potem otwiera się menedżer klubu, w którym mamy kilka opcji, takich jak:
- CHANGE LINEUP - pozwala zmienić nasz lineup.
- BROWSE LINEUP - pozwala przeglądać nasz lineup (po wybraniu danego gracza możemy zobaczyć szczegółowe informacje na jego temat).
- BROWSE PLAYERS - pozwala przeglądać graczy naszego klubu (po wybraniu danego gracza możemy zobaczyć szczegółowe informacje na jego temat).
- PLAY MATCH
- VIEW LEAGUE TABLE - pozwala zobaczyć tabelę ligi, czyli listę klubów, z którymi przyjdzie nam się zmierzyć i czy je pokonaliśmy, czy nie.
- SEND FOR TRAINING - pozwala wysłać piłkarza na trening, czyli zwiększyć jego losową statystykę o losową wartość.
- GO TO MID MENU - wyświetla pośrednie menu, które pozwala zapisać grę i do niej wrócić bądź wrócić do głównego menu.

### Mechaniki

Dodatkowo mamy takie mechaniki jak punkty akcji i dozwolone przegrane.
- W zamian za punkty akcji możemy zmienić lineup bądź wysłać piłkarza na trening (odnawiają się co mecz).
- Dozwolone przegrane to ilość razy, ile możemy przegrać mecz tak, aby nie odpaść z rozgrywki.

### Rozgrywka Meczu

Mecz rozgrywany jest automatycznie (wyniki są losowane na podstawie statystyk piłkarzy).
Podczas jego trwania widzimy jedynie komunikaty, kto strzelił bramkę. Po strzeleniu bramki zwiększa się losowa statystyka piłkarza.
- W przypadku wygranej, następny mecz będziemy rozgrywać z następnym klubem w lidze.
- W przypadku przegranej, nasza liczba dozwolonych przegranych zmniejsza się i będziemy grać z tym klubem do skutku lub wyczerpania się limitu przegranych.
- W przypadku remisu (bo istnieje na niego jakaś szansa) rozgrywane są strzały karne, które determinują, kto zwycięży.

## Główne Klasy i Sposób Ich Komunikacji

W grze mamy cztery, a dokładnie trzy główne klasy, które za nią odpowiadają. Są to:
- Klasa gry - służy jako opakowanie dla pozostałych trzech klas.
- Klasa menu - służy do pobierania inputu od użytkownika i wyświetlania interfejsu.
- Klasa data manager - trzyma dane takie jak liga czy gracze, pobiera je z API, formatuje i obsługuje - najważniejsza klasa w programie.
- Klasa club manager - obsługuje klub, to w nim rozgrywane są mecze, wybierane lineupy itd., w nim toczy się cała gra po wczytaniu odpowiednich danych.

Obiekty komunikują się ze sobą na podstawie pseudo wzorca projektowego Mediatora (pseudo, bo zaimplementowany według naszej wariacji). Mianowicie klasy te dziedziczą po klasie abstrakcyjnej "Event Object", która to jest podłączona do "Event Managera". Komunikacja opiera się na tym, że obiekty emitują z siebie "eventy", które zawierają w sobie dane (same w sobie też są daną) do menedżera, a ten przekazuje je do innych obiektów do niego podłączonych. Informacja, jaki event został przekazany i tym samym jak na niego zareagować, odbywa się po `typeid`. W związku z tym, mamy jedną klasę abstrakcyjną event, po której wszystkie inne rodzaje dziedziczą (dlatego jest ich dużo). Nazwa klasy jest jej identyfikatorem. Na tym w głównej mierze polega wzorzec Mediatora, więc wymagania w większym stopniu zostały spełnione.

## Klasy Reprezentujące "Świat Rzeczywisty - Piłkarski"

Takie klasy to:
- League
- Club
- Team
- Lineup
- Player
- Match

Większość tych klas służy jako kontener na odpowiednie dane (jeśli posiadają jakieś metody, to te klasy, o których wspomniano wyżej, je wywołują). Mogą posiadać pomocnicze struktury (np. gracz dla statystyk).

Wyjątkiem jest tutaj klasa meczu, bo sama w sobie zawiera logikę jego rozegrania. Wystarczy podać klub gracza i oponenta i wywołać jedną metodę.

## Użyte Biblioteki

Tak jak wcześniej wspomniano, użyte zostały biblioteki takie jak ncurses do interfejsu terminalowego oraz cpprest do obsługi plików JSON i API. Oprócz tego, wykorzystane zostały standardowe mechanizmy biblioteki standardowej. Z mniej powszechnych, korzystaliśmy z modułów "random", "filesystem" czy "typeinfo".

## Założenia a Ich Realizacja

Model rozgrywki jest dość uproszczony. Brak w nim między innymi mechaniki fauli, asyst, czy czerwonych i żółtych kartek czy historii gier, które miały się w nim znaleźć, aczkolwiek zabrakło na to czasu. Jednakże obecny stan rzeczy, biorąc pod uwagę nasze umiejętności i możliwości czasowe, jest naszym zdaniem zadowalający.
