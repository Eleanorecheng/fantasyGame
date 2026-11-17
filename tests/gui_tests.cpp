/** @file gui_tests.cpp @brief Exercise the public Qt controls without changing game internals. */
#include "window.h"
#include <QApplication>
#include <QComboBox>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTest>
#include <QTextEdit>
#include <QTimer>
#include <iostream>
#include <stdexcept>

/** @brief Require a UI condition.
 * @param condition Expected true value. @param message Failure description.
 * @throws std::runtime_error Condition failed.
 */
void require(bool condition, const char* message)
{
    if (!condition) {
        throw std::runtime_error(message);
    }
}

/** @brief Test setup, keyboard, buttons, clock, inventory and exit.
 * @param argc Argument count. @param argv Argument vector. @return Zero on success.
 * Pseudocode: CREATE window; SELECT setup; ISSUE keyboard and button commands;
 * CHECK state displays; RESTART and test item pickup/drop; EXIT and dismiss summary.
 */
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();
    QApplication::processEvents();
    auto* width = window.findChild<QSpinBox*>("boardWidth");
    auto* height = window.findChild<QSpinBox*>("boardHeight");
    auto* race = window.findChild<QComboBox*>("raceChoice");
    auto* restart = window.findChild<QPushButton*>("newGame");
    auto* status = window.findChild<QLabel*>("playerStatus");
    auto* location = window.findChild<QLabel*>("locationStatus");
    auto* command = window.findChild<QLineEdit*>("commandInput");
    auto* send = window.findChild<QPushButton*>("sendCommand");
    auto* log = window.findChild<QTextEdit*>("eventLog");
    require(width && height && race && restart && status && location && command && send && log,
            "essential controls exist");
    width->setValue(2);
    height->setValue(1);
    race->setCurrentIndex(4);
    QTest::mouseClick(restart, Qt::LeftButton);
    require(status->text().contains("Orc | DAY | Turn 0"), "setup selected race and reset clock");
    command->setText("N");
    QTest::mouseClick(send, Qt::LeftButton);
    require(command->text().isEmpty() && status->text().contains("Turn 1"), "Send submits once");
    require(log->toPlainText().contains("board ends"), "boundary result displayed");
    command->setText("L");
    QTest::keyClick(command, Qt::Key_Return);
    require(status->text().contains("Turn 2"), "Enter submits once");
    for (const char* action : {"action_i", "action_a", "action_e"}) {
        auto* button = window.findChild<QPushButton*>(action);
        require(button != nullptr, "action button exists");
        QTest::mouseClick(button, Qt::LeftButton);
    }
    require(status->text().contains("NIGHT | Turn 5") && status->text().contains("A 45"),
            "day/night change recalculates Orc attributes");
    require(location->text().contains("(2, 1)"), "east changes location");
    require(log->toPlainText().contains("No enemy here."), "attack empty start is safe");
    // Restart a larger board and look for a non-health-reducing item via movement only.
    width->setValue(8);
    height->setValue(8);
    QTest::mouseClick(restart, Qt::LeftButton);
    require(status->text().contains("DAY | Turn 0") && status->text().contains("Gold 0"),
            "restart resets state");
    bool picked = false;
    for (int y = 0; y < 8 && !picked; ++y) {
        for (int x = 0; x < 8 && !picked; ++x) {
            if (location->text().contains("Item:") &&
                !location->text().contains("Ring of Strength")) {
                QTest::mouseClick(window.findChild<QPushButton*>("action_p"), Qt::LeftButton);
                picked = log->toPlainText().contains("Picked up");
            }
            if (!picked && x < 7) {
                QTest::mouseClick(
                    window.findChild<QPushButton*>(y % 2 == 0 ? "action_e" : "action_w"),
                    Qt::LeftButton);
            }
        }
        if (!picked && y < 7) {
            QTest::mouseClick(window.findChild<QPushButton*>("action_s"), Qt::LeftButton);
        }
    }
    if (picked) {
        bool chooserShown = false;
        QTimer::singleShot(20, &window, [&chooserShown] {
            auto* dialog = qobject_cast<QInputDialog*>(QApplication::activeModalWidget());
            if (dialog) {
                chooserShown = true;
                dialog->accept();
            }
        });
        QTest::mouseClick(window.findChild<QPushButton*>("action_d"), Qt::LeftButton);
        require(chooserShown && log->toPlainText().contains("Dropped"),
                "drop chooser commits selected item");
        require(location->text().contains("Item:"), "dropped item is visible again");
    }
    // The unlikely empty-board case skips the item dialog check; deterministic core tests
    // verify item rules independently of random board population.
    bool resultShown = false;
    QTimer::singleShot(20, &window, [&resultShown] {
        auto* dialog = qobject_cast<QMessageBox*>(QApplication::activeModalWidget());
        if (dialog) {
            resultShown = dialog->text().contains("Gold collected:");
            dialog->accept();
        }
    });
    command->setText("eXiT");
    QTest::keyClick(command, Qt::Key_Return);
    require(resultShown && status->text().contains("GAME OVER"), "exit displays final gold");
    std::cout << "GUI controls passed; item chooser " << (picked ? "exercised" : "not encountered")
              << ".\n";
}
