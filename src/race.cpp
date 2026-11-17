/** @file race.cpp @brief Race implementation. */
#include "race.h"
#include <stdexcept>

/** @brief Name a race. @param race Race to name. @return Display name.
 * Pseudocode: CHECK the race index; LOOK UP and RETURN its display name.
 */
std::string raceName(Race race)
{
    static const char* names[] = {"Human", "Elf", "Dwarf", "Hobbit", "Orc"};
    if (static_cast<int>(race) < 0 || static_cast<int>(race) > 4) {
        throw std::invalid_argument("Invalid race.");
    }
    return names[static_cast<int>(race)];
}
