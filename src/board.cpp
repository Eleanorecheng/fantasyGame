/** @file board.cpp @brief Board implementation. */
#include "board.h"
#include "races.h"
#include <stdexcept>
#include <utility>
/** @brief Rebuild a rectangular board while preserving the old board on allocation failure.
 * @param width Requested columns. @param height Requested rows.
 * @param random Random source. @throws std::invalid_argument Invalid dimensions.
 * Pseudocode: VALIDATE size; ALLOCATE temporary squares; POPULATE contents;
 * MOVE completed storage into this board and update dimensions.
 */
void Board::reset(int width, int height, std::mt19937& random)
{
    if (width < 1 || height < 1 || width > 100 || height > 100) {
        throw std::invalid_argument("Board dimensions must be between 1 and 100.");
    }
    std::vector<std::unique_ptr<Square>> next;
    next.reserve(static_cast<std::size_t>(width * height));
    for (int i = 0; i < width * height; ++i) {
        auto square = std::make_unique<Square>();
        const int kind = std::uniform_int_distribution<int>(0, 99)(random);
        if (i != 0 && kind < 35) {
            square->enemy =
                makeCharacter(static_cast<Race>(std::uniform_int_distribution<int>(0, 4)(random)));
            // Some enemies carry one item; gold uses their effective defence.
            if (std::bernoulli_distribution(0.35)(random)) {
                std::string reason;
                square->enemy->pick(catalogue()[std::uniform_int_distribution<int>(0, 7)(random)],
                                    reason);
            }
        } else if (i != 0 && kind < 70) {
            square->item = std::make_unique<Item>(
                catalogue()[std::uniform_int_distribution<int>(0, 7)(random)]);
        }
        next.push_back(std::move(square));
    }
    squares_ = std::move(next);
    width_ = width;
    height_ = height;
}

/** @brief Access the square at checked coordinates.
 * @param x Column index. @param y Row index. @return Square at the requested location.
 * @throws std::out_of_range Invalid coordinates.
 * Pseudocode: CHECK both coordinates; COMPUTE flat index; RETURN square reference.
 */
Square& Board::at(int x, int y)
{
    // Validate both dimensions; a flattened index alone could wrap into another row.
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        throw std::out_of_range("Board coordinates out of range.");
    }
    return *squares_.at(static_cast<std::size_t>(y * width_ + x));
}

/** @brief Access the square at checked coordinates.
 * @param x Column index. @param y Row index. @return Square at the requested location.
 * @throws std::out_of_range Invalid coordinates.
 * Pseudocode: CHECK both coordinates; COMPUTE flat index; RETURN square reference.
 */
const Square& Board::at(int x, int y) const
{
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        throw std::out_of_range("Board coordinates out of range.");
    }
    return *squares_.at(static_cast<std::size_t>(y * width_ + x));
}

/** @brief Read the board dimension. @return Column count. */
int Board::width() const
{
    return width_;
}

/** @brief Read the board dimension. @return Row count. */
int Board::height() const
{
    return height_;
}
