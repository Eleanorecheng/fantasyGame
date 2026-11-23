/** @file game.cpp @brief Straightforward game actions and command dispatch. */
#include "game.h"
#include "races.h"
#include <cctype>
#include <sstream>
#include <stdexcept>

/** @brief Seed the generator for a normal game.
 * Pseudocode: CREATE a seed source; SEED the game's random-number generator.
 */
Game::Game()
{
    std::random_device seedSource;
    random_.seed(seedSource());
}

/** @brief Initialise a reproducible random generator.
 * @param seed Starting seed; a fixed value makes tests repeatable.
 */
Game::Game(unsigned seed)
{
    random_.seed(seed);
}

/** @brief Start a new game.
 * @param width Board columns. @param height Board rows. @param race Player race.
 * Pseudocode: VALIDATE dimensions; BUILD board and player; RESET position, gold and time.
 */
void Game::start(int width, int height, Race race)
{
    if (width < 1 || height < 1 || width > 100 || height > 100) {
        throw std::invalid_argument("Board dimensions must be between 1 and 100.");
    }
    board_.reset(width, height, random_);
    player_ = makeCharacter(race);
    x_ = 0;
    y_ = 0;
    gold_ = 0;
    turns_ = 0;
    ended_ = false;
}

/** @brief Convert user input to a single command.
 * @param input Raw text. @return Lowercase command letter, or '?' for empty input.
 * Pseudocode: SKIP whitespace and lowercase letters; RECOGNISE full exit;
 * RETURN the first remaining letter, or '?' when no letter remains.
 */
char Game::readCommand(const std::string& input) const
{
    std::string word;
    // Build a cleaned string with a familiar indexed loop.
    for (std::size_t i = 0; i < input.size(); i++) {
        unsigned char letter = static_cast<unsigned char>(input[i]);
        if (!std::isspace(letter)) {
            word += static_cast<char>(std::tolower(letter));
        }
    }
    if (word == "exit") {
        return 'x';
    }
    if (word.empty()) {
        return '?';
    }
    return word[0];
}

/** @brief Execute one command and then advance time exactly once.
 * @param input Raw command text. @param dropIndex Item selected for dropping, or -1.
 * @return Result message for the interface.
 * Pseudocode: REJECT ended game; READ command; CALL its action;
 * CHECK defeat; ADVANCE time; ADD final score if ended; RETURN message.
 */
std::string Game::command(const std::string& input, int dropIndex)
{
    if (ended_) {
        return "Game has ended. Start a new game.";
    }
    char action = readCommand(input);
    std::string message;
    switch (action) {
    case 'n':
    case 's':
    case 'e':
    case 'w':
        message = movePlayer(action);
        break;
    case 'p':
        message = pickUpItem();
        break;
    case 'd':
        message = dropItem(dropIndex);
        break;
    case 'a':
        message = attackEnemy();
        break;
    case 'l':
        message = look();
        break;
    case 'i':
        message = inventoryText();
        break;
    case 'x':
        ended_ = true;
        message = "Game ended.";
        break;
    default:
        message = "Unknown command. Use n/s/e/w, p, d, a, l, i or x.";
        break;
    }
    if (player_->dead()) {
        ended_ = true;
        message += "\nYou have been defeated.";
    }
    message += advanceTime();
    if (ended_) {
        message += "\nFinal gold: " + std::to_string(gold_);
    }
    return message;
}

/** @brief Move by one square if inside the board.
 * @param direction One of n, s, e, w. @return Location or boundary message.
 * Pseudocode: COPY position; CHANGE one coordinate; CHECK bounds;
 * STORE valid position and RETURN the location description.
 */
std::string Game::movePlayer(char direction)
{
    int nextX = x_;
    int nextY = y_;
    if (direction == 'n') {
        nextY--;
    } else if (direction == 's') {
        nextY++;
    } else if (direction == 'e') {
        nextX++;
    } else if (direction == 'w') {
        nextX--;
    }
    if (nextX < 0 || nextX >= board_.width() || nextY < 0 || nextY >= board_.height()) {
        return "The board ends here.";
    }
    x_ = nextX;
    y_ = nextY;
    return look();
}

/** @brief Try to collect the current item.
 * @return Pickup result.
 * Pseudocode: FIND square; CHECK for item; ASK player to carry it;
 * REMOVE ground item only after successful pickup.
 */
std::string Game::pickUpItem()
{
    Square& square = board_.at(x_, y_);
    if (!square.item) {
        return "No item here.";
    }
    std::string reason;
    if (!player_->pick(*square.item, reason)) {
        return reason;
    }
    std::string message = "Picked up " + square.item->name + ".";
    square.item.reset();
    return message;
}

/** @brief Try to put one carried item on the current square.
 * @param index Selected inventory index, or -1. @return Drop result.
 * Pseudocode: CHECK empty destination and selection; ASK player to remove item;
 * CREATE the ground item only after successful removal.
 */
std::string Game::dropItem(int index)
{
    Square& square = board_.at(x_, y_);
    if (square.item) {
        return "There is already an item here.";
    }
    if (square.enemy) {
        return "Cannot drop here: this square is occupied by an enemy.";
    }
    if (index < 0) {
        return "Select an inventory item first.";
    }
    Item removed;
    std::string reason;
    if (!player_->drop(static_cast<std::size_t>(index), removed, reason)) {
        return reason;
    }
    square.item = std::make_unique<Item>(removed);
    return "Dropped " + removed.name + ".";
}

/** @brief Resolve the player strike and any immediate counterattack.
 * @return Combat result.
 * Pseudocode: CHECK enemy; RESOLVE player strike; IF enemy dead award gold and remove it;
 * OTHERWISE resolve enemy counterattack; RETURN collected messages.
 */
std::string Game::attackEnemy()
{
    Square& square = board_.at(x_, y_);
    if (!square.enemy) {
        return "No enemy here.";
    }
    std::string message = "You: " + strike(*player_, *square.enemy);
    if (square.enemy->dead()) {
        Stats enemyStats = square.enemy->stats(night());
        int reward = enemyStats.defence;
        gold_ += reward;
        square.enemy.reset();
        message += "Enemy defeated. Earned " + std::to_string(reward) + " gold.";
    } else {
        message += "Enemy: " + strike(*square.enemy, *player_);
    }
    return message;
}

/** @brief Perform one probabilistic attack.
 * @param attacker Character making the strike. @param defender Target character.
 * @return Text describing the strike.
 * Pseudocode: GET stats; ROLL attack; RETURN on miss; ROLL defence;
 * CALCULATE damage; APPLY race ability if defended; UPDATE health and RETURN text.
 */
std::string Game::strike(Character& attacker, Character& defender)
{
    Stats attackStats = attacker.stats(night());
    Stats defenceStats = defender.stats(night());
    std::bernoulli_distribution attackRoll(attackStats.attackChance);
    bool hit = attackRoll(random_);
    if (!hit) {
        return raceName(attacker.race()) + " misses.\n";
    }
    std::bernoulli_distribution defenceRoll(defenceStats.defenceChance);
    bool defended = defenceRoll(random_);
    int damage = attackStats.attack - defenceStats.defence;
    if (damage < 0) {
        damage = 0;
    }
    if (defended) {
        // Virtual dispatch selects Human/Elf/Dwarf/Hobbit/Orc behaviour.
        damage = defender.defendedDamage(damage, night(), random_);
    }
    defender.damage(damage);
    std::string message = raceName(attacker.race()) + " attacks: ";
    if (defended) {
        message += "defence succeeds; ";
    } else {
        message += "defence fails; ";
    }
    message += raceName(defender.race());
    if (damage < 0) {
        message += " heals " + std::to_string(-damage);
    } else {
        message += " takes " + std::to_string(damage);
    }
    return message + ".\n";
}

/** @brief Count one command and report phase changes.
 * @return Day/night transition message, or an empty string.
 * Pseudocode: REMEMBER phase; INCREMENT count; COMPARE new phase; RETURN any transition.
 */
std::string Game::advanceTime()
{
    bool wasNight = night();
    turns_++;
    if (wasNight != night()) {
        if (night()) {
            return "\nNight falls.";
        }
        return "\nDay breaks.";
    }
    return "";
}

/** @brief Describe current location. @return Location text.
 * Pseudocode: print coordinates; describe enemy effective stats and item; describe empty location if neither exists.
 */
std::string Game::look() const
{
    std::ostringstream out;
    out << "Location (" << x_ + 1 << ", " << y_ + 1 << "). ";
    const Square& square = current();
    if (square.enemy) {
        const Stats s = square.enemy->stats(night());
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
    for (const Item& item : player_->inventory()) {
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
    int phase = turns_ / 5;
    if (phase % 2 == 1) {
        return true;
    }
    return false;
}

/** @brief Determine termination. @return True after defeat or exit. */
bool Game::ended() const
{
    return ended_;
}
