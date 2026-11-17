/** @file game.cpp @brief Game implementation. */
#include "game.h"
#include "races.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
/** @brief Seed a game. @param seed Reproducible random seed. */
Game::Game(unsigned seed) : random_(seed)
{
}

/** @brief Start a rectangular game.
     * @param width Board columns, 1 through 100. @param height Board rows, 1 through 100.
     * @param race Player race.
     * Pseudocode: validate size; reset counters; allocate every square independently;
     * populate non-start squares randomly; create player and reveal starting location.
     */
void Game::start(int width, int height, Race race)
{
    if (width < 1 || height < 1 || width > 100 || height > 100) {
        throw std::invalid_argument("Board dimensions must be between 1 and 100.");
    }
    board_.reset(width, height, random_);
    x_ = y_ = gold_ = turns_ = 0;
    ended_ = false;
    visited_.assign(static_cast<std::size_t>(width * height), false);
    player_ = makeCharacter(race);
    visited_[0] = true;
}

/** @brief Resolve one attack with race-specific defence.
     * @param attacker Attacker. @param defender Defender. @return Combat event.
     * Pseudocode: roll attack; if hit roll defence; calculate nonnegative adjusted damage;
     * invoke defender ability on successful defence; apply signed damage and report.
     */
std::string Game::strike(Character& attacker, Character& defender)
{
    const Stats a = attacker.stats(night()), d = defender.stats(night());
    const std::string who = raceName(attacker.race());
    if (!std::bernoulli_distribution(a.attackChance)(random_)) {
        return who + " misses.\n";
    }
    const bool defended = std::bernoulli_distribution(d.defenceChance)(random_);
    int amount = std::max(0, a.attack - d.defence);
    if (defended) {
        amount = defender.defendedDamage(amount, night(), random_);
    }
    defender.damage(amount);
    return who + " attacks: " + (defended ? "defence succeeds; " : "defence fails; ") +
           raceName(defender.race()) + (amount < 0 ? " heals " : " takes ") +
           std::to_string(amount < 0 ? -amount : amount) + ".\n";
}

/** @brief Process a case-insensitive command, advancing time even on failed commands.
     * @param input Command text. @param dropIndex Selected inventory index, or -1.
     * @return Event text.
     * Pseudocode: reject commands after game end; dispatch first command character;
     * perform action using current phase; check defeat; advance clock; report phase change.
     */
std::string Game::command(const std::string& input, int dropIndex)
{
    if (ended_) {
        return "Game has ended. Start a new game.";
    }
    const auto first = input.find_first_not_of(" \t\r\n");
    std::string normalized = first == std::string::npos ? std::string() : input.substr(first);
    for (char& letter : normalized) {
        letter = static_cast<char>(std::tolower(static_cast<unsigned char>(letter)));
    }
    normalized.erase(normalized.find_last_not_of(" \t\r\n") + 1);
    // East and exit share their first letter, so recognise full-word exit first.
    const char c = normalized == "exit" ? 'x' : normalized.empty() ? '?' : normalized.front();
    Square& square = board_.at(x_, y_);
    std::string result;
    if (c == 'n' || c == 's' || c == 'e' || c == 'w') {
        const int nx = x_ + (c == 'e') - (c == 'w');
        const int ny = y_ + (c == 's') - (c == 'n');
        if (nx < 0 || nx >= board_.width() || ny < 0 || ny >= board_.height()) {
            result = "The board ends here.";
        } else {
            x_ = nx;
            y_ = ny;
            visited_[static_cast<std::size_t>(y_ * board_.width() + x_)] = true;
            result = look();
        }
    } else if (c == 'p') {
        if (!square.item) {
            result = "No item here.";
        } else if (player_->pick(*square.item, result)) {
            result = "Picked up " + square.item->name + ".";
            square.item.reset();
        }
    } else if (c == 'd') {
        Item item{};
        if (square.item) {
            result = "There is already an item here.";
        } else if (square.enemy) {
            result = "Cannot drop here: this square is occupied by an enemy.";
        } else if (dropIndex < 0) {
            result = "Select an inventory item first.";
        } else if (player_->drop(static_cast<std::size_t>(dropIndex), item, result)) {
            square.item = std::make_unique<Item>(item);
            result = "Dropped " + item.name + ".";
        }
    } else if (c == 'a') {
        if (!square.enemy) {
            result = "No enemy here.";
        } else {
            result = "You: " + strike(*player_, *square.enemy);
            if (square.enemy->dead()) {
                const int reward = square.enemy->stats(night()).defence;
                gold_ += reward;
                result += "Enemy defeated. Earned " + std::to_string(reward) + " gold.";
                square.enemy.reset();
            } else {
                result += "Enemy: " + strike(*square.enemy, *player_);
            }
        }
    } else if (c == 'l') {
        result = look();
    } else if (c == 'i') {
        result = inventoryText();
    } else if (c == 'x') {
        ended_ = true;
        result = "Game ended.";
    } else {
        result = "Unknown command. Use n/s/e/w, p, d, a, l, i or x.";
    }
    if (player_->dead()) {
        ended_ = true;
        result += "\nYou have been defeated.";
    }
    const bool previousNight = night();
    ++turns_;
    if (previousNight != night()) {
        result += night() ? "\nNight falls." : "\nDay breaks.";
    }
    if (ended_) {
        result += "\nFinal gold: " + std::to_string(gold_);
    }
    return result;
}

/** @brief Describe current location. @return Location text.
 * Pseudocode: print coordinates; describe enemy effective stats and item; describe empty location if neither exists.
 */
std::string Game::look() const
{
    std::ostringstream out;
    out << "Location (" << x_ + 1 << ", " << y_ + 1 << "). ";
    const auto& square = current();
    if (square.enemy) {
        const auto s = square.enemy->stats(night());
        out << "Enemy " << raceName(square.enemy->race()) << " A:" << s.attack << " D:" << s.defence
            << " H:" << s.health << ". ";
    }
    if (square.item) {
        out << "Item: " << square.item->name << " (weight " << square.item->weight << ").";
    }
    if (!square.enemy && !square.item) {
        out << "Empty square.";
    }
    return out.str();
}

/** @brief Describe inventory and gold. @return Inventory text.
 * Pseudocode: print gold; append each carried item name and weight; return text.
 */
std::string Game::inventoryText() const
{
    std::string text = "Gold: " + std::to_string(gold_) + ". Items:";
    for (const auto& item : player_->inventory()) {
        text += "\n" + item.name + " (weight " + std::to_string(item.weight) + ")";
    }
    return text;
}

/** @brief Obtain current square. @return Current square. */
const Square& Game::current() const
{
    return at(x_, y_);
}

/** @brief Obtain square at coordinates. @param x Column. @param y Row. @return Square.
 * Pseudocode: CHECK each coordinate against its dimension; FLATTEN the valid coordinates; RETURN the referenced square.
 */
const Square& Game::at(int x, int y) const
{
    return board_.at(x, y);
}

/** @brief Obtain player. @return Read-only player. */
const Character& Game::player() const
{
    return *player_;
}

/** @brief Obtain column count. @return Width. */
int Game::width() const
{
    return board_.width();
}

/** @brief Obtain row count. @return Height. */
int Game::height() const
{
    return board_.height();
}

/** @brief Obtain player column. @return Column. */
int Game::x() const
{
    return x_;
}

/** @brief Obtain player row. @return Row. */
int Game::y() const
{
    return y_;
}

/** @brief Obtain earned gold. @return Gold. */
int Game::gold() const
{
    return gold_;
}

/** @brief Obtain command count. @return Turns. */
int Game::turns() const
{
    return turns_;
}

/** @brief Determine phase. @return True at night. */
bool Game::night() const
{
    return (turns_ / 5) % 2 != 0;
}

/** @brief Determine termination. @return True after defeat or exit. */
bool Game::ended() const
{
    return ended_;
}

/** @brief Determine exploration. @param x Column. @param y Row. @return Visited flag. */
bool Game::visited(int x, int y) const
{
    return visited_.at(static_cast<std::size_t>(y * board_.width() + x));
}
