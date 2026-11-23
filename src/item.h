/** @file item.h @brief Item declarations. */
#ifndef FANTASY_ITEM_H
#define FANTASY_ITEM_H
#include <string>
#include <vector>
/** @brief Equipment categories; only rings may repeat. */
enum class Category { Weapon, Armour, Shield, Ring };
/** @brief Equipment with additive attribute changes. */
class Item {
  public:
    /** @brief Create an empty item for a drop result. */
    Item();
    /** @brief Create one equipment definition.
     * @param itemName Display name. @param itemCategory Equipment category.
     * @param itemWeight Weight. @param attackBonus Attack change.
     * @param defenceBonus Defence change. @param healthBonus Health change.
     * @param strengthBonus Strength change.
     */
    Item(const std::string& itemName, Category itemCategory, int itemWeight, int attackBonus,
         int defenceBonus, int healthBonus, int strengthBonus);

    std::string name;  ///< Display name.
    Category category; ///< Equipment slot category.
    int weight;        ///< Carrying weight.
    int attack;        ///< Additive attack modifier.
    int defence;       ///< Additive defence modifier.
    int health;        ///< Additive current-health modifier.
    int strength;      ///< Additive carrying-capacity modifier.
};
/** @brief Return the eight immutable equipment definitions. @return Item catalogue. */
const std::vector<Item>& catalogue();
#endif
