# assignment2_final

C++17 / Qt Widgets fantasy game for EE6161 Assignment 2.

## Build and run

Open `CMakeLists.txt` in Qt Creator, select a Desktop Qt kit, configure, build,
and run `assignment2_final`. Qt 5 or Qt 6 Widgets is required.

Choose the board width, height and race, then click New game. Use the buttons
or enter commands: `n/s/e/w` move, `p` pick up, `d` drop, `a` attack,
`l` look, `i` inventory, and `x` or `exit` end the game.

## Files

- `src/`: 9 headers and 8 C++ source files.
- `CMakeLists.txt`: Qt Creator / CMake project configuration.
- `CLASS_DESIGN_REPORT.txt`: class responsibilities, ownership, inheritance and rule decisions.
- `Doxyfile` and `docs/doxygen-main.md`: Doxygen configuration and overview.
- [Doxygen HTML](docs/assignment2_final-doxygen.zip): extract and open `index.html`.
- [Submission ZIP](submission/assignment2_final_submission.zip): source, build configuration, design report and Doxygen HTML.

To regenerate documentation, install Doxygen and Graphviz and run
`doxygen Doxyfile` from the project root. Open `docs-generated/html/index.html`.

macOS compilation has been checked. The assignment's required Windows
Qt Creator compilation remains unverified.
