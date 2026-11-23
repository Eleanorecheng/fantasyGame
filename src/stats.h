/** @file stats.h @brief Stats declarations. */
#ifndef FANTASY_STATS_H
#define FANTASY_STATS_H

/** @brief Effective attributes and success probabilities. */
class Stats {
  public:
    /** @brief Create a collection of effective attribute values.
     * @param attackValue Attack. @param defenceValue Defence. @param healthValue Health.
     * @param strengthValue Capacity. @param hitChance Attack probability.
     * @param blockChance Defence probability.
     * Pseudocode: COPY each supplied value into the corresponding attribute.
     */
    Stats(int attackValue, int defenceValue, int healthValue, int strengthValue, double hitChance,
          double blockChance)
    {
        attack = attackValue;
        defence = defenceValue;
        health = healthValue;
        strength = strengthValue;
        attackChance = hitChance;
        defenceChance = blockChance;
    }

    int attack;           ///< Effective attack force.
    int defence;          ///< Effective defence value.
    int health;           ///< Current health after equipment and accumulated damage.
    int strength;         ///< Effective carrying capacity.
    double attackChance;  ///< Attack success probability, from zero to one.
    double defenceChance; ///< Defence success probability, from zero to one.
};
#endif
