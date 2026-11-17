/** @file races.cpp @brief Races implementation. */
#include "races.h"
#include <stdexcept>
/** @brief Construct blocker. @param race Human or dwarf race.
 * Pseudocode: CHECK that the selected race is Human or Dwarf; INITIALISE the shared character state.
 */
BlockingCharacter::BlockingCharacter(Race race) : Character(race)
{
    if (race != Race::Human && race != Race::Dwarf) {
        throw std::invalid_argument("Blocking characters must be Human or Dwarf.");
    }
}

/** @brief Resolve successful defence for this race.
 * @param adjusted Nonnegative attack minus defence. @param night Night flag.
 * @param random Random source. @return Signed health damage; negative heals.
 * Pseudocode: SELECT the race-specific defence outcome; RETURN signed damage (negative means healing).
 */
int BlockingCharacter::defendedDamage(int adjusted, bool night, std::mt19937& random) const
{
    (void)adjusted;
    (void)night;
    (void)random;
    return 0;
}

/** @brief Construct elf. */
ElfCharacter::ElfCharacter() : Character(Race::Elf)
{
}

/** @brief Resolve successful defence for this race.
 * @param adjusted Nonnegative attack minus defence. @param night Night flag.
 * @param random Random source. @return Signed health damage; negative heals.
 * Pseudocode: SELECT the race-specific defence outcome; RETURN signed damage (negative means healing).
 */
int ElfCharacter::defendedDamage(int adjusted, bool night, std::mt19937& random) const
{
    (void)adjusted;
    (void)night;
    (void)random;
    return -1;
}

/** @brief Construct hobbit. */
HobbitCharacter::HobbitCharacter() : Character(Race::Hobbit)
{
}

/** @brief Resolve successful defence for this race.
 * @param adjusted Nonnegative attack minus defence. @param night Night flag.
 * @param random Random source. @return Signed health damage; negative heals.
 * Pseudocode: SELECT the race-specific defence outcome; RETURN signed damage (negative means healing).
 */
int HobbitCharacter::defendedDamage(int adjusted, bool night, std::mt19937& random) const
{
    (void)adjusted;
    (void)night;
    return std::uniform_int_distribution<int>(0, 5)(random);
}

/** @brief Construct orc. */
OrcCharacter::OrcCharacter() : Character(Race::Orc)
{
}

/** @brief Resolve successful defence for this race.
 * @param adjusted Nonnegative attack minus defence. @param night Night flag.
 * @param random Random source. @return Signed health damage; negative heals.
 * Pseudocode: SELECT the race-specific defence outcome; RETURN signed damage (negative means healing).
 */
int OrcCharacter::defendedDamage(int adjusted, bool night, std::mt19937& random) const
{
    (void)random;
    return night ? -1 : adjusted / 4;
}

/** @brief Construct a polymorphic character. @param race Desired race. @return Owned character.
 * Pseudocode: select subclass for race; allocate and return unique ownership; reject invalid race.
 */
std::unique_ptr<Character> makeCharacter(Race race)
{
    switch (race) {
    case Race::Human:
    case Race::Dwarf:
        return std::make_unique<BlockingCharacter>(race);
    case Race::Elf:
        return std::make_unique<ElfCharacter>();
    case Race::Hobbit:
        return std::make_unique<HobbitCharacter>();
    case Race::Orc:
        return std::make_unique<OrcCharacter>();
    }
    throw std::invalid_argument("Invalid race");
}
