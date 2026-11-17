/** @file character.cpp @brief Character implementation. */
#include "character.h"
#include <stdexcept>

/** @brief Construct a race character. @param race Character race.
 * Pseudocode: CHECK that the race value is supported; STORE the race and initialise damage.
 */
Character::Character(Race race) : race_(race)
{
    if (static_cast<int>(race) < 0 || static_cast<int>(race) > 4) {
        throw std::invalid_argument("Invalid character race.");
    }
}

/** @brief Compute race and equipment attributes. @param night Night flag. @return Effective stats.
 * Pseudocode: choose race baseline and phase; add all equipment modifiers;
 * subtract accumulated damage from modified health; return computed attributes.
 */
Stats Character::stats(bool night) const
{
    static const Stats base[] = {{30, 20, 60, 100, 2.0 / 3, 0.5},
                                 {40, 10, 40, 70, 1.0, 0.25},
                                 {30, 20, 50, 130, 2.0 / 3, 2.0 / 3},
                                 {25, 20, 70, 85, 1.0 / 3, 2.0 / 3},
                                 {25, 10, 50, 130, 0.25, 0.25}};
    Stats s = base[static_cast<int>(race_)];
    if (race_ == Race::Orc && night) {
        s.attack = 45;
        s.defence = 25;
        s.attackChance = 1.0;
        s.defenceChance = 0.5;
    }
    for (const auto& item : items_) {
        s.attack += item.attack;
        s.defence += item.defence;
        s.health += item.health;
        s.strength += item.strength;
    }
    s.health -= damage_;
    return s;
}

/** @brief Sum carried weight. @return Total weight.
 * Pseudocode: initialise total to zero; add each carried item weight; return total.
 */
int Character::weight() const
{
    int result = 0;
    for (const auto& item : items_) {
        result += item.weight;
    }
    return result;
}

/** @brief Obtain race. @return Race identifier. */
Race Character::race() const
{
    return race_;
}

/** @brief Check defeat. @return True when effective health is at most zero. */
bool Character::dead() const
{
    return stats(false).health <= 0;
}

/** @brief Apply signed health damage; negative values heal. @param amount Damage. */
void Character::damage(int amount)
{
    damage_ += amount;
}

/** @brief Add equipment when category and final capacity permit.
     * @param item Equipment to add. @param reason Failure explanation. @return Success.
     * Pseudocode: reject duplicate non-ring category; calculate capacity including item;
     * reject overweight result; append equipment and return success.
     */
bool Character::pick(const Item& item, std::string& reason)
{
    for (const auto& carried : items_) {
        if (item.category != Category::Ring && carried.category == item.category) {
            reason = "Already carrying an item in this category.";
            return false;
        }
    }
    if (weight() + item.weight > stats(false).strength + item.strength) {
        reason = "Too heavy to carry.";
        return false;
    }
    items_.push_back(item);
    return true;
}

/** @brief Remove equipment if remaining inventory is supportable.
     * @param index Inventory index. @param removed Output item. @param reason Failure explanation.
     * @return Success.
     * Pseudocode: validate index; calculate remaining weight and strength;
     * reject overweight result; copy item and erase it.
     */
bool Character::drop(std::size_t index, Item& removed, std::string& reason)
{
    if (index >= items_.size()) {
        reason = "Select an inventory item first.";
        return false;
    }
    const Item item = items_[index];
    if (weight() - item.weight > stats(false).strength - item.strength) {
        reason = "Cannot drop this item: remaining inventory would be overweight.";
        return false;
    }
    removed = item;
    items_.erase(items_.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}

/** @brief Inspect carried equipment. @return Read-only inventory. */
const std::vector<Item>& Character::inventory() const
{
    return items_;
}
