/** @file square.h @brief Square declarations. */
#ifndef FANTASY_SQUARE_H
#define FANTASY_SQUARE_H
#include "character.h"
#include "item.h"
#include <memory>
/** @brief One dynamically allocated board location, owning its optional contents. */
struct Square {
    std::unique_ptr<Item> item;       ///< Owned item, or null.
    std::unique_ptr<Character> enemy; ///< Owned enemy, or null.
};
#endif
