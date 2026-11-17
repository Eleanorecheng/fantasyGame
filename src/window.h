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
class Window final : public QWidget {
  public:
    /** @brief Build interface and start initial game. @param parent Owning widget. */
    explicit Window(QWidget* parent = nullptr);

  private:
    /** @brief Reset engine from setup controls.
     * Pseudocode: read dimensions and race; reset engine and event log; refresh widgets.
     */
    void start();
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
