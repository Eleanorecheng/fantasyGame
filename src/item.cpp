/** @file item.cpp @brief Item implementation. */
#include "item.h"

/** @brief Return the eight immutable equipment definitions. @return Item catalogue. */
const std::vector<Item>& catalogue()
{
    static const std::vector<Item> items = {{"Sword", Category::Weapon, 10, 10, 0, 0, 0},
                                            {"Dagger", Category::Weapon, 5, 5, 0, 0, 0},
                                            {"Plate Armour", Category::Armour, 40, -5, 10, 0, 0},
                                            {"Leather Armour", Category::Armour, 20, 0, 5, 0, 0},
                                            {"Large Shield", Category::Shield, 30, -5, 10, 0, 0},
                                            {"Small Shield", Category::Shield, 10, 0, 5, 0, 0},
                                            {"Ring of Life", Category::Ring, 1, 0, 0, 10, 0},
                                            {"Ring of Strength", Category::Ring, 1, 0, 0, -10, 50}};
    return items;
}
