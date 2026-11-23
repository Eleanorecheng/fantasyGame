/** @file board.cpp @brief Board implementation. */
#include "board.h"
#include "races.h"
#include <stdexcept>
#include <utility>
/** @brief Rebuild a rectangular board using directly owned square storage.
 * @param width Requested columns. @param height Requested rows.
 * @param random Random source. @throws std::invalid_argument Invalid dimensions.
 * Pseudocode: VALIDATE size; CLEAR old storage; ALLOCATE each square; POPULATE contents;
 * MOVE each square pointer into the board container.
 */
void Board::reset(int width, int height, std::mt19937& random)
{
    if (width < 1 || height < 1 || width > 100 || height > 100) {
        throw std::invalid_argument("Board dimensions must be between 1 and 100.");
    }
    squares_.clear();
    width_ = width;
    height_ = height;
    std::uniform_int_distribution<int> contentRoll(0, 99);
    std::uniform_int_distribution<int> raceRoll(0, 4);
    std::uniform_int_distribution<int> itemRoll(0, 7);
    std::bernoulli_distribution equipmentRoll(0.35);
    for (int i = 0; i < width * height; ++i) {
        std::unique_ptr<Square> square = std::make_unique<Square>();
        const int kind = contentRoll(random);
        if (i != 0 && kind < 35) {
            square->enemy = makeCharacter(static_cast<Race>(raceRoll(random)));
            // Some enemies carry one item; gold uses their effective defence.
            if (equipmentRoll(random)) {
                std::string reason;
                square->enemy->pick(catalogue()[itemRoll(random)], reason);
            }
        } else if (i != 0 && kind < 70) {
            square->item = std::make_unique<Item>(catalogue()[itemRoll(random)]);
        }
        squares_.push_back(std::move(square));
    }
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
