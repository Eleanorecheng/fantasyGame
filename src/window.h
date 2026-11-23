/** @file window.h @brief Qt Widgets game presentation. */
#ifndef FANTASY_WINDOW_H
#define FANTASY_WINDOW_H
#include "game.h"
#include <QWidget>
class QComboBox;
class QSpinBox;
class QLabel;
class QTextEdit;
class QLineEdit;
/** @brief Main window; owns widgets through Qt parent ownership. */
class Window : public QWidget {
    Q_OBJECT
  public:
    /** @brief Build interface and start initial game. @param parent Owning widget. */
    explicit Window(QWidget* parent = nullptr);

  private slots:
    /** @brief Read settings and start a game; also handles New game clicks. */
    void start();
    /** @brief Submit typed text when Send or Enter is used. */
    void onSendClicked();
    /** @brief Handle the North button. */
    void onNorthClicked();
    /** @brief Handle the West button. */
    void onWestClicked();
    /** @brief Handle the South button. */
    void onSouthClicked();
    /** @brief Handle the East button. */
    void onEastClicked();
    /** @brief Handle the PickUp button. */
    void onPickUpClicked();
    /** @brief Handle the Drop button. */
    void onDropClicked();
    /** @brief Handle the Attack button. */
    void onAttackClicked();
    /** @brief Handle the Look button. */
    void onLookClicked();
    /** @brief Handle the Inventory button. */
    void onInventoryClicked();
    /** @brief Handle the Exit button. */
    void onExitClicked();

  private:
    /** @brief Submit a game command. @param command Command string.
     * Pseudocode: prompt for drop item when appropriate; execute exactly one command;
     * append event and refresh; show final gold on termination; restore keyboard focus.
     */
    void submit(const QString& command);
    /** @brief Display current player statistics and location.
     * Pseudocode: calculate effective attributes; display player status,
     * current location and number of commands until the next phase.
     */
    void refresh();
    Game game_;
    QSpinBox *width_, *height_;
    QComboBox* race_;
    QLabel *status_, *location_;
    QTextEdit* log_;
    QLineEdit* command_;
};
#endif
