/** @file core_tests.cpp @brief Deterministic checks for game rules and transitions. */
#include "game.h"
#include "races.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <utility>
/** @brief Require a condition. @param ok Condition. @param message Failure text. */
void require(bool ok, const char* message)
{
    if (!ok) {
        throw std::runtime_error(message);
    }
}
/** @brief Exercise rules and seeded complete games. @return Zero on success.
 * Pseudocode: check race abilities and equipment invariants; check command clock;
 * traverse seeded boards, collecting items and fighting; verify rewards and defeat.
 */
int main()
{
    std::mt19937 rng(42);
    std::string reason;
    auto human = makeCharacter(Race::Human);
    require(human->stats(false).attack == 30, "human baseline");
    require(human->defendedDamage(100, false, rng) == 0, "human block");
    auto dwarf = makeCharacter(Race::Dwarf);
    require(dwarf->defendedDamage(100, false, rng) == 0, "dwarf block");
    auto elf = makeCharacter(Race::Elf);
    require(elf->defendedDamage(100, false, rng) == -1, "elf heals");
    auto orc = makeCharacter(Race::Orc);
    require(orc->stats(true).attack == 45 && orc->stats(true).defence == 25, "orc night stats");
    require(orc->defendedDamage(11, false, rng) == 2, "orc quarter rounding");
    require(orc->defendedDamage(11, true, rng) == -1, "orc night healing");
    auto hobbit = makeCharacter(Race::Hobbit);
    for (int i = 0; i < 100; ++i) {
        const int damage = hobbit->defendedDamage(100, false, rng);
        require(damage >= 0 && damage <= 5, "hobbit defence range");
    }
    require(human->pick(catalogue()[0], reason), "sword pickup");
    require(!human->pick(catalogue()[1], reason), "duplicate weapon rejected");
    require(human->pick(catalogue()[2], reason), "armour pickup");
    require(human->pick(catalogue()[4], reason), "shield pickup");
    require(human->stats(false).attack == 30 && human->stats(false).defence == 40,
            "equipment modifiers");
    require(human->pick(catalogue()[6], reason), "life ring");
    require(human->pick(catalogue()[6], reason), "repeat ring");
    require(human->stats(false).health == 80, "ring health stacking");
    for (int i = 0; i < 150; ++i) {
        require(human->pick(catalogue()[7], reason), "dynamic ring inventory");
    }
    require(human->inventory().size() == 155, "no fixed inventory count");
    auto load = makeCharacter(Race::Elf);
    require(load->pick(catalogue()[7], reason), "strength ring");
    require(load->pick(catalogue()[2], reason), "plate");
    require(load->pick(catalogue()[4], reason), "large shield");
    require(load->pick(catalogue()[0], reason), "sword");
    Item removed{};
    require(!load->drop(0, removed, reason), "overweight strength ring drop refused");
    require(load->drop(1, removed, reason), "drop armour");
    require(load->drop(0, removed, reason), "drop ring after unloading");
    auto fragile = makeCharacter(Race::Elf);
    fragile->damage(35);
    require(fragile->pick(catalogue()[7], reason) && fragile->dead(), "equipment can cause defeat");
    Game g(12);
    g.start(1, 1, Race::Human);
    require(!g.current().enemy && !g.current().item, "empty start");
    g.command("N");
    require(g.x() == 0 && g.y() == 0 && g.turns() == 1, "boundary counts time");
    for (int i = 0; i < 4; ++i) {
        g.command("look");
    }
    require(g.night(), "night after five commands");
    for (int i = 0; i < 5; ++i) {
        g.command("invalid");
    }
    require(!g.night(), "day after ten commands");
    g.command("  eXiT  ");
    require(g.ended(), "case insensitive full exit");
    const int turns = g.turns();
    g.command("l");
    require(g.turns() == turns, "ended game does not advance");
    bool invalidSize = false;
    try {
        g.start(0, 2, Race::Human);
    } catch (const std::invalid_argument&) {
        invalidSize = true;
    }
    require(invalidSize, "invalid dimensions rejected");
    // Every command path consumes one turn, including rejected drop and empty input.
    Game commands(7);
    commands.start(2, 1, Race::Human);
    commands.command("");
    commands.command("p");
    commands.command("d");
    commands.command("a");
    require(commands.turns() == 4 && !commands.night(), "failed actions advance time");
    commands.command("i");
    require(commands.turns() == 5 && commands.night(), "fifth command flips phase");
    int occupiedChecks = 0;
    for (unsigned seed = 0; seed < 40; ++seed) {
        Game occupied(seed);
        occupied.start(2, 1, Race::Human);
        occupied.command("e");
        if (occupied.current().enemy) {
            const auto* enemy = occupied.current().enemy.get();
            const int before = occupied.turns();
            const std::string result = occupied.command("d", 0);
            require(result.find("occupied") != std::string::npos,
                    "drop prompts no mutation on enemy");
            require(occupied.current().enemy.get() == enemy && !occupied.current().item,
                    "exclusive square contents preserved");
            require(occupied.turns() == before + 1, "rejected drop advances clock");
            ++occupiedChecks;
        }
    }
    require(occupiedChecks > 0, "occupied-square coverage");
    Board board;
    std::mt19937 boardRandom(123);
    board.reset(3, 2, boardRandom);
    require(board.width() == 3 && board.height() == 2, "rectangular board");
    require(&board.at(0, 0) != &board.at(1, 0), "independent squares");
    for (const auto& coordinate : std::vector<std::pair<int, int>>{{-1, 0}, {3, 0}, {0, 2}}) {
        bool rejected = false;
        try {
            board.at(coordinate.first, coordinate.second);
        } catch (const std::out_of_range&) {
            rejected = true;
        }
        require(rejected, "coordinates cannot wrap to a different row");
    }
    try {
        board.reset(0, 2, boardRandom);
    } catch (const std::invalid_argument&) {
        require(board.width() == 3 && board.height() == 2, "invalid reset preserves board");
    }
    const Stats expected[] = {{30, 20, 60, 100, 2.0 / 3, 0.5},
                              {40, 10, 40, 70, 1.0, 0.25},
                              {30, 20, 50, 130, 2.0 / 3, 2.0 / 3},
                              {25, 20, 70, 85, 1.0 / 3, 2.0 / 3},
                              {25, 10, 50, 130, 0.25, 0.25}};
    for (int raceIndex = 0; raceIndex < 5; ++raceIndex) {
        auto character = makeCharacter(static_cast<Race>(raceIndex));
        const Stats actual = character->stats(false), wanted = expected[raceIndex];
        require(actual.attack == wanted.attack && actual.defence == wanted.defence &&
                    actual.health == wanted.health && actual.strength == wanted.strength &&
                    std::abs(actual.attackChance - wanted.attackChance) < 1e-9 &&
                    std::abs(actual.defenceChance - wanted.defenceChance) < 1e-9,
                "all baseline attributes match PDF race table");
    }
    require(catalogue().size() == 8, "all eight item types available");
    for (const auto& equipment : catalogue()) {
        auto wearer = makeCharacter(Race::Dwarf);
        require(wearer->pick(equipment, reason), "individual item fits dwarf");
        const Stats equipped = wearer->stats(false);
        require(equipped.attack == 30 + equipment.attack &&
                    equipped.defence == 20 + equipment.defence &&
                    equipped.health == 50 + equipment.health &&
                    equipped.strength == 130 + equipment.strength &&
                    wearer->weight() == equipment.weight,
                "item effects apply on pickup");
        require(wearer->drop(0, removed, reason), "drop individual item");
        require(wearer->stats(false).health == 50 && wearer->weight() == 0,
                "drop reverses item effects");
    }
    int fights = 0, kills = 0;
    // Exercise real command/combat paths across seeds and all player races.
    for (unsigned seed = 0; seed < 30; ++seed) {
        Game run(seed);
        run.start(8, 8, static_cast<Race>(seed % 5));
        for (int row = 0; row < 8 && !run.ended(); ++row) {
            for (int col = 0; col < 8 && !run.ended(); ++col) {
                if (run.current().item) {
                    run.command("p");
                }
                int rounds = 0;
                while (run.current().enemy && !run.ended() && rounds++ < 500) {
                    const int oldGold = run.gold();
                    const int reward = run.current().enemy->stats(run.night()).defence;
                    run.command("a");
                    ++fights;
                    if (!run.current().enemy) {
                        ++kills;
                        require(run.gold() - oldGold == reward, "effective defence gold reward");
                    }
                    if (run.player().dead()) {
                        require(run.ended(), "death terminates game");
                    }
                }
                // Zero damage and defence healing can cause a legitimate stalemate.
                // The player may move away, so the traversal bounds attack attempts.
                if (col < 7 && !run.ended()) {
                    run.command(row % 2 == 0 ? "e" : "w");
                }
            }
            if (row < 7 && !run.ended()) {
                run.command("s");
            }
        }
    }
    require(fights > 50 && kills > 10, "combat coverage");
    std::cout << "All core checks passed; " << fights << " combat rounds, " << kills << " kills.\n";
}
