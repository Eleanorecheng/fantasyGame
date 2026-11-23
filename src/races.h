/** @file races.h @brief Five concrete character types and their creation helper. */
#ifndef ASSIGNMENT2_RACES_H
#define ASSIGNMENT2_RACES_H
#include "character.h"
#include <memory>

/** @brief A Human character with its own successful-defence rule. */
class Human : public Character {
  public:
    /** @brief Create a Human with no equipment or accumulated damage. */
    Human();
    /** @brief Calculate damage after successful defence.
     * @param adjusted Attack minus defence, clamped at zero.
     * @param night True during night. @param random Random-number generator.
     * @return Signed damage; negative one means healing one health.
     */
    int defendedDamage(int adjusted, bool night, std::mt19937& random) const override;
};

/** @brief A Elf character with its own successful-defence rule. */
class Elf : public Character {
  public:
    /** @brief Create a Elf with no equipment or accumulated damage. */
    Elf();
    /** @brief Calculate damage after successful defence.
     * @param adjusted Attack minus defence, clamped at zero.
     * @param night True during night. @param random Random-number generator.
     * @return Signed damage; negative one means healing one health.
     */
    int defendedDamage(int adjusted, bool night, std::mt19937& random) const override;
};

/** @brief A Dwarf character with its own successful-defence rule. */
class Dwarf : public Character {
  public:
    /** @brief Create a Dwarf with no equipment or accumulated damage. */
    Dwarf();
    /** @brief Calculate damage after successful defence.
     * @param adjusted Attack minus defence, clamped at zero.
     * @param night True during night. @param random Random-number generator.
     * @return Signed damage; negative one means healing one health.
     */
    int defendedDamage(int adjusted, bool night, std::mt19937& random) const override;
};

/** @brief A Hobbit character with its own successful-defence rule. */
class Hobbit : public Character {
  public:
    /** @brief Create a Hobbit with no equipment or accumulated damage. */
    Hobbit();
    /** @brief Calculate damage after successful defence.
     * @param adjusted Attack minus defence, clamped at zero.
     * @param night True during night. @param random Random-number generator.
     * @return Signed damage; negative one means healing one health.
     */
    int defendedDamage(int adjusted, bool night, std::mt19937& random) const override;
};

/** @brief A Orc character with its own successful-defence rule. */
class Orc : public Character {
  public:
    /** @brief Create a Orc with no equipment or accumulated damage. */
    Orc();
    /** @brief Calculate damage after successful defence.
     * @param adjusted Attack minus defence, clamped at zero.
     * @param night True during night. @param random Random-number generator.
     * @return Signed damage; negative one means healing one health.
     */
    int defendedDamage(int adjusted, bool night, std::mt19937& random) const override;
};

/** @brief Build one of the five concrete characters.
 * @param race Selected race. @return Exclusive ownership of the new character.
 * @throws std::invalid_argument Invalid race.
 */
std::unique_ptr<Character> makeCharacter(Race race);
#endif
