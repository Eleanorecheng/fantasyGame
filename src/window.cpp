/** @file window.cpp @brief Basic Qt Widgets controls and game state display. */
#include "window.h"
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>

/** @brief Build interface and start initial game. @param parent Owning widget.
 * Pseudocode: create setup, status, action buttons and command log;
 * connect buttons and keyboard submission to the engine; start the initial game.
 */
Window::Window(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Fantasy Game - Assignment 2");
    resize(760, 520);
    auto* root = new QVBoxLayout(this);
    auto* setup = new QHBoxLayout;
    width_ = new QSpinBox(this);
    width_->setObjectName("boardWidth");
    height_ = new QSpinBox(this);
    height_->setObjectName("boardHeight");
    for (auto* spin : {width_, height_}) {
        spin->setRange(1, 100);
        spin->setValue(8);
    }
    race_ = new QComboBox(this);
    race_->setObjectName("raceChoice");
    race_->addItems({"Human", "Elf", "Dwarf", "Hobbit", "Orc"});
    setup->addWidget(new QLabel("Width", this));
    setup->addWidget(width_);
    setup->addWidget(new QLabel("Height", this));
    setup->addWidget(height_);
    setup->addWidget(new QLabel("Race", this));
    setup->addWidget(race_);
    auto* newGame = new QPushButton("New game", this);
    newGame->setObjectName("newGame");
    setup->addWidget(newGame);
    setup->addStretch();
    root->addLayout(setup);
    status_ = new QLabel(this);
    status_->setObjectName("playerStatus");
    status_->setWordWrap(true);
    root->addWidget(status_);
    location_ = new QLabel(this);
    location_->setObjectName("locationStatus");
    location_->setWordWrap(true);
    root->addWidget(location_);
    auto* buttons = new QGridLayout;
    const QStringList labels = {"North [n]",     "West [w]", "South [s]",  "East [e]",
                                "Pick up [p]",   "Drop [d]", "Attack [a]", "Look [l]",
                                "Inventory [i]", "Exit [x]"};
    const QStringList commands = {"n", "w", "s", "e", "p", "d", "a", "l", "i", "x"};
    for (int i = 0; i < labels.size(); ++i) {
        auto* button = new QPushButton(labels[i], this);
        button->setObjectName("action_" + commands[i]);
        buttons->addWidget(button, i / 5, i % 5);
        connect(button, &QPushButton::clicked, this, [this, commands, i] { submit(commands[i]); });
    }
    root->addLayout(buttons);
    log_ = new QTextEdit(this);
    log_->setObjectName("eventLog");
    log_->setReadOnly(true);
    log_->document()->setMaximumBlockCount(1000);
    root->addWidget(log_, 2);
    command_ = new QLineEdit(this);
    command_->setObjectName("commandInput");
    command_->setPlaceholderText("Type a command and press Enter (east: e, exit: x or exit)");
    auto* inputRow = new QHBoxLayout;
    inputRow->addWidget(command_);
    auto* send = new QPushButton("Send", this);
    send->setObjectName("sendCommand");
    inputRow->addWidget(send);
    root->addLayout(inputRow);
    connect(send, &QPushButton::clicked, this, [this] {
        const QString input = command_->text();
        command_->clear();
        submit(input);
    });
    connect(command_, &QLineEdit::returnPressed, this, [this] {
        const QString input = command_->text();
        command_->clear();
        submit(input);
    });
    connect(newGame, &QPushButton::clicked, this, [this] { start(); });
    start();
}

/** @brief Reset engine from setup controls.
     * Pseudocode: read dimensions and race; reset engine and event log; refresh widgets.
     */
void Window::start()
{
    game_.start(width_->value(), height_->value(), static_cast<Race>(race_->currentIndex()));
    log_->clear();
    command_->clear();
    log_->append("Collect gold by defeating enemies. Every command advances time; phases last five "
                 "commands.");
    log_->append(QString::fromStdString(game_.look()));
    refresh();
    command_->setFocus();
}

/** @brief Submit a game command. @param command Command string.
     * Pseudocode: prompt for drop item when appropriate; execute exactly one command;
     * append event and refresh; show final gold on termination; restore keyboard focus.
     */
void Window::submit(const QString& command)
{
    int dropIndex = -1;
    const QString input = command.trimmed().toLower();
    // Ask for the item as required, rather than silently using a selection.
    if (!game_.ended() && input.startsWith('d') && !game_.current().item &&
        !game_.current().enemy && !game_.player().inventory().empty()) {
        QStringList choices;
        int index = 0;
        for (const auto& item : game_.player().inventory()) {
            choices << QString("%1. %2 (weight %3)")
                           .arg(++index)
                           .arg(QString::fromStdString(item.name))
                           .arg(item.weight);
        }
        bool accepted = false;
        const QString choice =
            QInputDialog::getItem(this, "Drop an item", "Which item would you like to drop?",
                                  choices, 0, false, &accepted);
        if (accepted) {
            dropIndex = choices.indexOf(choice);
        }
        // Cancel is still a submitted command and therefore advances time.
    }
    const bool wasEnded = game_.ended();
    log_->append("> " + command.toHtmlEscaped());
    const std::string event = game_.command(command.toStdString(), dropIndex);
    log_->append(QString::fromStdString(event).toHtmlEscaped().replace("\n", "<br>"));
    refresh();
    if (!wasEnded && game_.ended()) {
        QMessageBox::information(
            this, "Game over",
            QString("%1\nGold collected: %2\nCommands used: %3\nChoose New game to play again.")
                .arg(game_.player().dead() ? "You have been defeated." : "You ended the adventure.")
                .arg(game_.gold())
                .arg(game_.turns()));
    }
    command_->setFocus();
}

/** @brief Display current player statistics and location.
     * Pseudocode: calculate effective attributes; display player status,
     * current location and number of commands until the next phase.
     */
void Window::refresh()
{
    const Stats s = game_.player().stats(game_.night());
    status_->setText(QString("%1 | %2 | Turn %3 | HP %4 | A %5 | D %6 | Load %7/%8 | Gold %9%10")
                         .arg(QString::fromStdString(raceName(game_.player().race())))
                         .arg(game_.night() ? "NIGHT" : "DAY")
                         .arg(game_.turns())
                         .arg(s.health)
                         .arg(s.attack)
                         .arg(s.defence)
                         .arg(game_.player().weight())
                         .arg(s.strength)
                         .arg(game_.gold())
                         .arg(game_.ended() ? " | GAME OVER" : ""));
    location_->setText(QString::fromStdString(game_.look()) +
                       QString("   |   %1 command(s) until %2")
                           .arg(5 - game_.turns() % 5)
                           .arg(game_.night() ? "day" : "night"));
}
