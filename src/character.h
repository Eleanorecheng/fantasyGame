/** @file character.h @brief Character declarations. */
#ifndef FANTASY_CHARACTER_H
#define FANTASY_CHARACTER_H
#include "item.h"
#include "race.h"
#include "stats.h"
#include <cstddef>
#include <random>
#include <vector>
/** @brief Abstract character; subclasses define successful-defence behaviour. */
class Character {
  public:
    /** @brief Construct a race character. @param race Character race. */
    explicit Character(Race race);
    /** @brief Destroy polymorphically. */
    virtual ~Character() = default;
    /** @brief Compute race and equipment attributes. @param night Night flag. @return Effective stats. */
    Stats stats(bool night) const;
    /** @brief Sum carried weight. @return Total weight. */
    int weight() const;
    /** @brief Obtain race. @return Race identifier. */
    Race race() const;
    /** @brief Check defeat. @return True when effective health is at most zero. */
    bool dead() const;
    /** @brief Apply signed health damage; negative values heal. @param amount Damage. */
    void damage(int amount);
    /** @brief Add equipment when category and final capacity permit.
     * @param item Equipment to add. @param reason Failure explanation. @return Success.
     * Pseudocode: reject duplicate non-ring category; calculate capacity including item;
     * reject overweight result; append equipment and return success.
     */
    bool pick(const Item& item, std::string& reason);
    /** @brief Remove equipment if remaining inventory is supportable.
     * @param index Inventory index. @param removed Output item. @param reason Failure explanation.
     * @return Success.
     * Pseudocode: validate index; calculate remaining weight and strength;
     * reject overweight result; copy item and erase it.
     */
    bool drop(std::size_t index, Item& removed, std::string& reason);
    /** @brief Inspect carried equipment. @return Read-only inventory. */
    const std::vector<Item>& inventory() const;
    /** @brief Resolve damage after a successful defence.
     * @param adjusted Nonnegative attack minus defence. @param night Night flag.
     * @param random Random generator. @return Signed damage, with negative meaning healing.
     */
    virtual int defendedDamage(int adjusted, bool night, std::mt19937& random) const = 0;

  private:
    Race race_;
    int damage_ = 0;
    std::vector<Item> items_;
};
#endif
