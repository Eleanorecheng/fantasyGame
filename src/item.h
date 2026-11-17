/** @file item.h @brief Item declarations. */
#ifndef FANTASY_ITEM_H
#define FANTASY_ITEM_H
#include <string>
#include <vector>
/** @brief Equipment categories; only rings may repeat. */
enum class Category { Weapon, Armour, Shield, Ring };
/** @brief Equipment with additive attribute changes. */
struct Item {
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
