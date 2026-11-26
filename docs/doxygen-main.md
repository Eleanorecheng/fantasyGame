# assignment2_final API documentation

A small C++17 fantasy game with a Qt Widgets interface. This documentation describes the classes and functions in the final, beginner-oriented implementation.

## Where to start

1. Read the responsibilities of @ref Window and @ref Game.
2. Follow @ref Game::command to the individual action functions.
3. Inspect @ref Board and @ref Square for map ownership.
4. Read @ref Character and its five subclasses for attributes and virtual defence.
5. Use the **Classes** and **Files** navigation entries to inspect individual APIs.

The class API is generated from the headers to avoid merging duplicate parameter descriptions from declarations and definitions. The entry point is documented from `main.cpp`. Full implementation comments and pseudocode remain in the `.cpp` files; open them alongside the API when tracing a command.

## Module responsibilities

| Class or file | Responsibility |
| --- | --- |
| main.cpp | Create QApplication, create/show Window, run the Qt event loop |
| Window | Setup controls, named Qt slots, command input and displayed results |
| Game | Command dispatch, action functions, combat, turns and gold |
| Board | Rectangular grid and checked coordinate lookup |
| Square | One location owning optional contents |
| Character | Abstract character, inventory and calculated attributes |
| Human, Elf, Dwarf, Hobbit, Orc | Race-specific successful-defence rules |
| Item | Equipment data and constructor |
| Stats | Computed attributes returned by value |

## Object ownership

A diamond is drawn at the owning end. A square contains an enemy, an item or neither; its two optional pointers are mutually exclusive under the game rules.

\dot
 digraph ownership {
   rankdir=TB;
   node [shape=box, fontname="Helvetica"];
   edge [fontname="Helvetica", fontsize=10, dir=both, arrowtail=diamond, arrowhead=none];
   Window -> Game [label="one value member"];
   Game -> Board [label="one value member"];
   Game -> Player [label="unique_ptr"];
   Player [label="Character (player)"];
   Board -> Square [label="vector of unique_ptr: many squares"];
   Square -> Enemy [label="optional unique_ptr"];
   Enemy [label="Character (enemy)"];
   Square -> GroundItem [label="optional unique_ptr"];
   GroundItem [label="Item (ground)"];
   Player -> PlayerInventory [label="vector of values"];
   Enemy -> EnemyInventory [label="vector of values"];
   PlayerInventory [label="Item (player inventory)"];
   EnemyInventory [label="Item (enemy inventory)"];
 }
\enddot

## Character inheritance

@ref Character is abstract. Its pure virtual @ref Character::defendedDamage method is implemented by all five concrete races. The inherited virtual destructor supports safe cleanup through `unique_ptr<Character>`. Both players and enemies use the same hierarchy.

\dot
 digraph inheritance {
   rankdir=TB;
   node [shape=box, fontname="Helvetica"];
   Character [label="Character (abstract)"];
   edge [dir=back, arrowtail=empty, arrowhead=none];
   Character -> Human;
   Character -> Elf;
   Character -> Dwarf;
   Character -> Hobbit;
   Character -> Orc;
 }
\enddot

## Follow an attack command

| Step | Call | Result |
| --- | --- | --- |
| 1 | Window::onAttackClicked | Submit the letter a |
| 2 | Window::submit | Call Game::command and later display its result |
| 3 | Game::command | Dispatch to Game::attackEnemy |
| 4 | Game::attackEnemy | Call Game::strike for the player |
| 5 | Game::strike | Read stats, roll attack/defence and apply the virtual defence rule if appropriate |
| 6 | Game::attackEnemy | Award gold/remove a defeated enemy, or resolve its counterattack |
| 7 | Game::command | Check player defeat and call Game::advanceTime once |
| 8 | Window::submit | Display result text and call Window::refresh |

## Navigation notes

Private helper methods and named slots are included so the command path can be followed. Member tables distinguish public and private API. Individual class pages include inheritance/collaboration diagrams where applicable.

Gameplay rules and platform validation are described in README.md, CLASS_DESIGN_REPORT.txt and VALIDATION.txt in the project root. Windows Qt Creator compilation remains unverified.
