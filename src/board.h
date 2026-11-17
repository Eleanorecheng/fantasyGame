/** @file board.h @brief Board declarations. */
#ifndef FANTASY_BOARD_H
#define FANTASY_BOARD_H
#include "square.h"
#include <random>
#include <vector>
/** @brief Rectangular board owning each independently allocated square. */
class Board {
  public:
    /** @brief Allocate and populate a board.
     * @param width Column count (1-100). @param height Row count (1-100).
     * @param random Random source. @throws std::invalid_argument Invalid dimensions.
     * Pseudocode: validate dimensions; allocate each square; randomly assign content
     * except at the empty start; transfer completed squares into the board.
     */
    void reset(int width, int height, std::mt19937& random);
    /** @brief Access a square. @param x Column. @param y Row. @return Square.
     * @throws std::out_of_range Coordinates outside board.
     */
    Square& at(int x, int y);
    /** @brief Inspect a square. @param x Column. @param y Row. @return Square.
     * @throws std::out_of_range Coordinates outside board.
     */
    const Square& at(int x, int y) const;
    /** @brief Read column count. @return Width. */
    int width() const;
    /** @brief Read row count. @return Height. */
    int height() const;

  private:
    int width_ = 0, height_ = 0;
    std::vector<std::unique_ptr<Square>> squares_;
};
#endif
