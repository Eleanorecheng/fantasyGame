/** @file races.h @brief Races declarations. */
#ifndef FANTASY_RACES_H
#define FANTASY_RACES_H
#include "character.h"
#include <memory>
/** @brief Human and dwarf defenders block all successful defences. */
class BlockingCharacter final : public Character {
  public:
    /** @brief Construct blocker. @param race Human or dwarf race. */
    explicit BlockingCharacter(Race race);
    /** @copydoc Character::defendedDamage */
    int defendedDamage(int adjusted, bool night, std::mt19937& random) const override;
};
/** @brief Elf defenders heal on successful defence. */
class ElfCharacter final : public Character {
  public:
    /** @brief Construct elf. */
    ElfCharacter();
    /** @copydoc Character::defendedDamage */
    int defendedDamage(int adjusted, bool night, std::mt19937& random) const override;
};
/** @brief Hobbit defenders take random damage from zero through five. */
class HobbitCharacter final : public Character {
  public:
    /** @brief Construct hobbit. */
    HobbitCharacter();
    /** @copydoc Character::defendedDamage */
    int defendedDamage(int adjusted, bool night, std::mt19937& random) const override;
};
/** @brief Orc defenders change abilities with time of day. */
class OrcCharacter final : public Character {
  public:
    /** @brief Construct orc. */
    OrcCharacter();
    /** @copydoc Character::defendedDamage */
    int defendedDamage(int adjusted, bool night, std::mt19937& random) const override;
};
/** @brief Construct a polymorphic character. @param race Desired race. @return Owned character. */
std::unique_ptr<Character> makeCharacter(Race race);
#endif
