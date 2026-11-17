/** @file stats.h @brief Stats declarations. */
#ifndef FANTASY_STATS_H
#define FANTASY_STATS_H

/** @brief Effective attributes and success probabilities. */
struct Stats {
    int attack;           ///< Effective attack force.
    int defence;          ///< Effective defence value.
    int health;           ///< Current health after equipment and accumulated damage.
    int strength;         ///< Effective carrying capacity.
    double attackChance;  ///< Attack success probability, from zero to one.
    double defenceChance; ///< Defence success probability, from zero to one.
};
#endif
