/** @file races.cpp @brief Race-specific successful-defence rules. */
#include "races.h"
#include <stdexcept>

/** @brief Construct a Human. */
Human::Human() : Character(Race::Human)
{
}

/** @brief Apply the Human successful-defence rule.
 * @param adjusted Nonnegative adjusted damage. @param night Night flag.
 * @param random Random source. @return Signed damage; negative values heal.
 * Pseudocode: APPLY this race's defence rule; RETURN the resulting damage.
 */
int Human::defendedDamage(int adjusted, bool night, std::mt19937& random) const
{
    (void)adjusted;
    (void)night;
    (void)random;
    return 0;
}

/** @brief Construct a Elf. */
Elf::Elf() : Character(Race::Elf)
{
}

/** @brief Apply the Elf successful-defence rule.
 * @param adjusted Nonnegative adjusted damage. @param night Night flag.
 * @param random Random source. @return Signed damage; negative values heal.
 * Pseudocode: APPLY this race's defence rule; RETURN the resulting damage.
 */
int Elf::defendedDamage(int adjusted, bool night, std::mt19937& random) const
{
    (void)adjusted;
    (void)night;
    (void)random;
    return -1;
}

/** @brief Construct a Dwarf. */
Dwarf::Dwarf() : Character(Race::Dwarf)
{
}

/** @brief Apply the Dwarf successful-defence rule.
 * @param adjusted Nonnegative adjusted damage. @param night Night flag.
 * @param random Random source. @return Signed damage; negative values heal.
 * Pseudocode: APPLY this race's defence rule; RETURN the resulting damage.
 */
int Dwarf::defendedDamage(int adjusted, bool night, std::mt19937& random) const
{
    (void)adjusted;
    (void)night;
    (void)random;
    return 0;
}

/** @brief Construct a Hobbit. */
Hobbit::Hobbit() : Character(Race::Hobbit)
{
}

/** @brief Apply the Hobbit successful-defence rule.
 * @param adjusted Nonnegative adjusted damage. @param night Night flag.
 * @param random Random source. @return Signed damage; negative values heal.
 * Pseudocode: APPLY this race's defence rule; RETURN the resulting damage.
 */
int Hobbit::defendedDamage(int adjusted, bool night, std::mt19937& random) const
{
    (void)adjusted;
    (void)night;
    std::uniform_int_distribution<int> damageRoll(0, 5);
    return damageRoll(random);
}

/** @brief Construct a Orc. */
Orc::Orc() : Character(Race::Orc)
{
}

/** @brief Apply the Orc successful-defence rule.
 * @param adjusted Nonnegative adjusted damage. @param night Night flag.
 * @param random Random source. @return Signed damage; negative values heal.
 * Pseudocode: APPLY this race's defence rule; RETURN the resulting damage.
 */
int Orc::defendedDamage(int adjusted, bool night, std::mt19937& random) const
{
    (void)random;
    if (night) {
        return -1;
    }
    return adjusted / 4;
}

/** @brief Create the requested character.
 * @param race Selected race. @return Owned character through the common base type.
 * @throws std::invalid_argument Invalid race.
 * Pseudocode: SELECT the matching race; CREATE that concrete type; RETURN its pointer.
 */
std::unique_ptr<Character> makeCharacter(Race race)
{
    switch (race) {
    case Race::Human:
        return std::make_unique<Human>();
    case Race::Elf:
        return std::make_unique<Elf>();
    case Race::Dwarf:
        return std::make_unique<Dwarf>();
    case Race::Hobbit:
        return std::make_unique<Hobbit>();
    case Race::Orc:
        return std::make_unique<Orc>();
    }
    throw std::invalid_argument("Invalid race.");
}
