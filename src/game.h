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
    /** @brief Create a game with a newly generated random seed. */
    Game();
    /** @brief Create a repeatable game for testing. @param seed Fixed random seed. */
    explicit Game(unsigned seed);
    /** @brief Start a rectangular game.
     * @param width Board columns, 1 through 100. @param height Board rows, 1 through 100.
     * @param race Player race.
     * Pseudocode: validate dimensions; ask Board to populate squares;
     * reset counters; create the player.
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

  private:
    /** @brief Convert user input to a single command.
     * @param input Raw text. @return Lowercase command letter, or '?' for empty input.
     * Pseudocode: SKIP whitespace and lowercase letters; RECOGNISE full exit;
     * RETURN the first remaining letter, or '?' when no letter remains.
     */
    char readCommand(const std::string& input) const;
    /** @brief Move by one square if inside the board.
     * @param direction One of n, s, e, w. @return Location or boundary message.
     * Pseudocode: COPY position; CHANGE one coordinate; CHECK bounds;
     * STORE valid position and RETURN the location description.
     */
    std::string movePlayer(char direction);
    /** @brief Try to collect the current item.
     * @return Pickup result.
     * Pseudocode: FIND square; CHECK for item; ASK player to carry it;
     * REMOVE ground item only after successful pickup.
     */
    std::string pickUpItem();
    /** @brief Try to put one carried item on the current square.
     * @param index Selected inventory index, or -1. @return Drop result.
     * Pseudocode: CHECK empty destination and selection; ASK player to remove item;
     * CREATE the ground item only after successful removal.
     */
    std::string dropItem(int index);
    /** @brief Resolve the player strike and any immediate counterattack.
     * @return Combat result.
     * Pseudocode: CHECK enemy; RESOLVE player strike; IF enemy dead award gold and remove it;
     * OTHERWISE resolve enemy counterattack; RETURN collected messages.
     */
    std::string attackEnemy();
    /** @brief Perform one probabilistic attack.
     * @param attacker Character making the strike. @param defender Target character.
     * @return Text describing the strike.
     * Pseudocode: GET stats; ROLL attack; RETURN on miss; ROLL defence;
     * CALCULATE damage; APPLY race ability if defended; UPDATE health and RETURN text.
     */
    std::string strike(Character& attacker, Character& defender);
    /** @brief Count one command and report phase changes.
     * @return Day/night transition message, or an empty string.
     * Pseudocode: REMEMBER phase; INCREMENT count; COMPARE new phase; RETURN any transition.
     */
    std::string advanceTime();
    std::mt19937 random_;
    Board board_;
    std::unique_ptr<Character> player_;
    int x_ = 0;
    int y_ = 0;
    int gold_ = 0;
    int turns_ = 0;
    bool ended_ = true;
};
#endif
