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
    setWindowTitle("assignment2_final - Fantasy Game");
    resize(760, 520);
    QVBoxLayout* root = new QVBoxLayout(this);
    QHBoxLayout* setup = new QHBoxLayout;
    width_ = new QSpinBox(this);
    width_->setObjectName("boardWidth");
    height_ = new QSpinBox(this);
    height_->setObjectName("boardHeight");
    width_->setRange(1, 100);
    width_->setValue(8);
    height_->setRange(1, 100);
    height_->setValue(8);
    race_ = new QComboBox(this);
    race_->setObjectName("raceChoice");
    race_->addItems({"Human", "Elf", "Dwarf", "Hobbit", "Orc"});
    setup->addWidget(new QLabel("Width", this));
    setup->addWidget(width_);
    setup->addWidget(new QLabel("Height", this));
    setup->addWidget(height_);
    setup->addWidget(new QLabel("Race", this));
    setup->addWidget(race_);
    QPushButton* newGame = new QPushButton("New game", this);
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
    QGridLayout* buttons = new QGridLayout;
    QPushButton* nButton = new QPushButton("North [n]", this);
    nButton->setObjectName("action_n");
    buttons->addWidget(nButton, 0, 0);
    connect(nButton, &QPushButton::clicked, this, &Window::onNorthClicked);
    QPushButton* wButton = new QPushButton("West [w]", this);
    wButton->setObjectName("action_w");
    buttons->addWidget(wButton, 0, 1);
    connect(wButton, &QPushButton::clicked, this, &Window::onWestClicked);
    QPushButton* sButton = new QPushButton("South [s]", this);
    sButton->setObjectName("action_s");
    buttons->addWidget(sButton, 0, 2);
    connect(sButton, &QPushButton::clicked, this, &Window::onSouthClicked);
    QPushButton* eButton = new QPushButton("East [e]", this);
    eButton->setObjectName("action_e");
    buttons->addWidget(eButton, 0, 3);
    connect(eButton, &QPushButton::clicked, this, &Window::onEastClicked);
    QPushButton* pButton = new QPushButton("Pick up [p]", this);
    pButton->setObjectName("action_p");
    buttons->addWidget(pButton, 0, 4);
    connect(pButton, &QPushButton::clicked, this, &Window::onPickUpClicked);
    QPushButton* dButton = new QPushButton("Drop [d]", this);
    dButton->setObjectName("action_d");
    buttons->addWidget(dButton, 1, 0);
    connect(dButton, &QPushButton::clicked, this, &Window::onDropClicked);
    QPushButton* aButton = new QPushButton("Attack [a]", this);
    aButton->setObjectName("action_a");
    buttons->addWidget(aButton, 1, 1);
    connect(aButton, &QPushButton::clicked, this, &Window::onAttackClicked);
    QPushButton* lButton = new QPushButton("Look [l]", this);
    lButton->setObjectName("action_l");
    buttons->addWidget(lButton, 1, 2);
    connect(lButton, &QPushButton::clicked, this, &Window::onLookClicked);
    QPushButton* iButton = new QPushButton("Inventory [i]", this);
    iButton->setObjectName("action_i");
    buttons->addWidget(iButton, 1, 3);
    connect(iButton, &QPushButton::clicked, this, &Window::onInventoryClicked);
    QPushButton* xButton = new QPushButton("Exit [x]", this);
    xButton->setObjectName("action_x");
    buttons->addWidget(xButton, 1, 4);
    connect(xButton, &QPushButton::clicked, this, &Window::onExitClicked);
    root->addLayout(buttons);
    log_ = new QTextEdit(this);
    log_->setObjectName("eventLog");
    log_->setReadOnly(true);
    log_->document()->setMaximumBlockCount(1000);
    root->addWidget(log_, 2);
    command_ = new QLineEdit(this);
    command_->setObjectName("commandInput");
    command_->setPlaceholderText("Type a command and press Enter (east: e, exit: x or exit)");
    QHBoxLayout* inputRow = new QHBoxLayout;
    inputRow->addWidget(command_);
    QPushButton* send = new QPushButton("Send", this);
    send->setObjectName("sendCommand");
    inputRow->addWidget(send);
    root->addLayout(inputRow);
    connect(send, &QPushButton::clicked, this, &Window::onSendClicked);
    connect(command_, &QLineEdit::returnPressed, this, &Window::onSendClicked);
    connect(newGame, &QPushButton::clicked, this, &Window::start);
    start();
}

/** @brief Reset engine from setup controls.
     * Pseudocode: read dimensions and race; reset engine and event log; refresh widgets.
     */
void Window::start()
{
    int width = width_->value();
    int height = height_->value();
    Race selectedRace = static_cast<Race>(race_->currentIndex());
    game_.start(width, height, selectedRace);
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
        for (const Item& item : game_.player().inventory()) {
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
    std::string text = command.toStdString();
    std::string event = game_.command(text, dropIndex);
    log_->append(QString::fromStdString(event).toHtmlEscaped().replace("\n", "<br>"));
    refresh();
    if (!wasEnded && game_.ended()) {
        QString outcome = "You ended the adventure.";
        if (game_.player().dead()) {
            outcome = "You have been defeated.";
        }
        QMessageBox::information(
            this, "Game over",
            QString("%1\nGold collected: %2\nCommands used: %3\nChoose New game to play again.")
                .arg(outcome)
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
    bool isNight = game_.night();
    const Character& player = game_.player();
    Stats s = player.stats(isNight);
    QString phase = "DAY";
    QString nextPhase = "night";
    QString endMessage = "";
    if (isNight) {
        phase = "NIGHT";
        nextPhase = "day";
    }
    if (game_.ended()) {
        endMessage = " | GAME OVER";
    }
    status_->setText(QString("%1 | %2 | Turn %3 | HP %4 | A %5 | D %6 | Load %7/%8 | Gold %9%10")
                         .arg(QString::fromStdString(raceName(game_.player().race())))
                         .arg(phase)
                         .arg(game_.turns())
                         .arg(s.health)
                         .arg(s.attack)
                         .arg(s.defence)
                         .arg(game_.player().weight())
                         .arg(s.strength)
                         .arg(game_.gold())
                         .arg(endMessage));
    location_->setText(
        QString::fromStdString(game_.look()) +
        QString("   |   %1 command(s) until %2").arg(5 - game_.turns() % 5).arg(nextPhase));
}

/** @brief Submit typed text from Send or Enter.
 * Pseudocode: READ input; CLEAR field; SEND one command to the shared handler.
 */
void Window::onSendClicked()
{
    QString input = command_->text();
    command_->clear();
    submit(input);
}

/** @brief Submit the North command from its button. */
void Window::onNorthClicked()
{
    submit("n");
}

/** @brief Submit the West command from its button. */
void Window::onWestClicked()
{
    submit("w");
}

/** @brief Submit the South command from its button. */
void Window::onSouthClicked()
{
    submit("s");
}

/** @brief Submit the East command from its button. */
void Window::onEastClicked()
{
    submit("e");
}

/** @brief Submit the PickUp command from its button. */
void Window::onPickUpClicked()
{
    submit("p");
}

/** @brief Submit the Drop command from its button. */
void Window::onDropClicked()
{
    submit("d");
}

/** @brief Submit the Attack command from its button. */
void Window::onAttackClicked()
{
    submit("a");
}

/** @brief Submit the Look command from its button. */
void Window::onLookClicked()
{
    submit("l");
}

/** @brief Submit the Inventory command from its button. */
void Window::onInventoryClicked()
{
    submit("i");
}

/** @brief Submit the Exit command from its button. */
void Window::onExitClicked()
{
    submit("x");
}
