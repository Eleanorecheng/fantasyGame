/** @file race.h @brief Race declarations. */
#ifndef FANTASY_RACE_H
#define FANTASY_RACE_H
#include <string>
/** @brief The five supported races, in the assignment table order. */
enum class Race { Human, Elf, Dwarf, Hobbit, Orc };
/** @brief Name a race. @param race Race to name. @return Display name. */
std::string raceName(Race race);
#endif
