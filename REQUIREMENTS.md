# Assignment requirement mapping

Source: supplied EE6161-2026-Assignment2FantasyGame.pdf, five pages. This checklist maps implementation evidence; it does not claim an unperformed Windows build.

| PDF section / requirement | Implementation evidence | Status |
|---|---|---|
| 2.1 Rectangular selectable board | `Window::start`, `Board::reset`, width/height controls | Implemented; practical range 1-100 each |
| 2.1 Square contents and initial population | `Square`, `Board::reset` | Implemented |
| 2.1 Day/night after every five commands | `Game::command`, `Game::night` | Implemented and tested |
| 2.1 Keyboard movement, pickup/drop, attack, look, inventory, exit | `Game::command`, `Window::submit` | Implemented; e/x conflict documented |
| 2.2 Character attributes and probabilities | `Character::stats`, `Stats` | Implemented and race table tested |
| 2.3 All five races | `Race`, `makeCharacter`, `Character::stats` | Implemented |
| 2.4 Special defence abilities | `Character::defendedDamage` overrides | Implemented and tested |
| 2.5 All eight items and modifiers | `catalogue`, `Character::pick/drop/stats` | Implemented and tested |
| 2.5 Weight, one item per non-ring category, repeatable rings | `Character::pick/drop`, `vector<Item>` | Implemented and tested |
| 2.6 Attack/defence probabilities, counterattack, defeat and gold | `Game::strike`, `Game::command` | Implemented and tested |
| Page 4 Windows Qt Creator build | CMake and qmake projects, standard C++/Qt APIs | Source prepared; Windows execution/build NOT verified |
| Page 4 Classes, inheritance, polymorphism, abstract base | `Character` and subclasses, virtual destructor and pure virtual method | Implemented |
| Page 4 Every square dynamically allocated with smart pointers | `Board::reset`: `make_unique<Square>`, `vector<unique_ptr<Square>>` | Implemented and allocation ownership tested |
| Page 4 Dynamic inventory without a fixed count cap | `Character::items_`: `vector<Item>` | Implemented; growing inventory tested |
| Page 4 C++ Qt Widgets GUI | `Window : QWidget`, Qt layouts/buttons/dialogs/input/log | Implemented and offscreen interaction tested |
| Page 4 Doxygen file/class/function comments and pseudocode | Headers and definitions, `Doxyfile` | Included; public API documentation checked |
| Page 4 K&R formatting and suitable files | Separated modules, `.clang-format` | Included and format checked |
| Page 4 Standard C++ / Qt libraries only | `CMakeLists.txt`, `FantasyGame.pro` | Implemented; no third-party runtime libraries |
| Page 5 Qt Creator project, source and root report in one ZIP | Build files, `src/`, `CLASS_DESIGN_REPORT.txt` | Included |

The PDF's example numbers conflict with its formal race table; the table is used. Other edge decisions and the allocation guard are stated in README.md and the design report. Marks and full compliance cannot be guaranteed without the required Windows build and the instructor's interpretation of ambiguities.
