/** @file game.h @brief Game declarations. */
#ifndef FANTASY_GAME_H
#define FANTASY_GAME_H
#include "board.h"
#include <memory>
#include <random>
#include <string>
#include <vector>
/** @brief Game state and command processing, independent of the GUI. */
class Game {
  public:
    /** @brief Seed a game engine. @param seed Reproducible random seed. */
    explicit Game(unsigned seed = std::random_device{}());
    /** @brief Start a rectangular game.
     * @param width Board columns, 1 through 100. @param height Board rows, 1 through 100.
     * @param race Player race.
     * Pseudocode: validate dimensions; ask Board to populate squares;
     * reset counters; create player and mark the starting location visited.
     */
    void start(int width, int height, Race race);
    /** @brief Process a case-insensitive command, advancing time even on failed commands.
     * @param input Command text. @param dropIndex Selected inventory index, or -1.
     * @return Event text.
     * Pseudocode: reject commands after game end; dispatch first command character;
     * perform action using current phase; check defeat; advance clock; report phase change.
     */
    std::string command(const std::string& input, int dropIndex = -1);
    /** @brief Describe current location. @return Location text. */
    std::string look() const;
    /** @brief Describe inventory and gold. @return Inventory text. */
    std::string inventoryText() const;
    /** @brief Obtain current square. @return Current square. */
    const Square& current() const;
    /** @brief Obtain square at coordinates. @param x Column. @param y Row. @return Square. */
    const Square& at(int x, int y) const;
    /** @brief Obtain player. @return Read-only player. */
    const Character& player() const;
    /** @brief Obtain column count. @return Width. */
    int width() const;
    /** @brief Obtain row count. @return Height. */
    int height() const;
    /** @brief Obtain player column. @return Column. */
    int x() const;
    /** @brief Obtain player row. @return Row. */
    int y() const;
    /** @brief Obtain earned gold. @return Gold. */
    int gold() const;
    /** @brief Obtain command count. @return Turns. */
    int turns() const;
    /** @brief Determine phase. @return True at night. */
    bool night() const;
    /** @brief Determine termination. @return True after defeat or exit. */
    bool ended() const;
    /** @brief Determine exploration. @param x Column. @param y Row. @return Visited flag. */
    bool visited(int x, int y) const;

  private:
    /** @brief Resolve one attack with race-specific defence.
     * @param attacker Attacker. @param defender Defender. @return Combat event.
     * Pseudocode: roll attack; if hit roll defence; calculate nonnegative adjusted damage;
     * invoke defender ability on successful defence; apply signed damage and report.
     */
    std::string strike(Character& attacker, Character& defender);
    std::mt19937 random_;
    Board board_;
    std::vector<bool> visited_;
    std::unique_ptr<Character> player_;
    int x_ = 0, y_ = 0, gold_ = 0, turns_ = 0;
    bool ended_ = true;
};
#endif
