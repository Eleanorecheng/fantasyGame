/** @file item.cpp @brief Item implementation. */
#include "item.h"

/** @brief Initialise a blank item used as an output value. */
Item::Item()
{
    name = "";
    category = Category::Weapon;
    weight = 0;
    attack = 0;
    defence = 0;
    health = 0;
    strength = 0;
}

/** @brief Initialise equipment data.
 * @param itemName Name. @param itemCategory Category. @param itemWeight Weight.
 * @param attackBonus Attack modifier. @param defenceBonus Defence modifier.
 * @param healthBonus Health modifier. @param strengthBonus Strength modifier.
 * Pseudocode: COPY each constructor argument into its corresponding data member.
 */
Item::Item(const std::string& itemName, Category itemCategory, int itemWeight, int attackBonus,
           int defenceBonus, int healthBonus, int strengthBonus)
{
    name = itemName;
    category = itemCategory;
    weight = itemWeight;
    attack = attackBonus;
    defence = defenceBonus;
    health = healthBonus;
    strength = strengthBonus;
}

/** @brief Return the eight immutable equipment definitions. @return Item catalogue. */
const std::vector<Item>& catalogue()
{
    static const std::vector<Item> items = {
        Item("Sword", Category::Weapon, 10, 10, 0, 0, 0),
        Item("Dagger", Category::Weapon, 5, 5, 0, 0, 0),
        Item("Plate Armour", Category::Armour, 40, -5, 10, 0, 0),
        Item("Leather Armour", Category::Armour, 20, 0, 5, 0, 0),
        Item("Large Shield", Category::Shield, 30, -5, 10, 0, 0),
        Item("Small Shield", Category::Shield, 10, 0, 5, 0, 0),
        Item("Ring of Life", Category::Ring, 1, 0, 0, 10, 0),
        Item("Ring of Strength", Category::Ring, 1, 0, 0, -10, 50)};
    return items;
}
