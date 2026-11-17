/** @file main.cpp @brief Cross-platform Qt Widgets entry point. */
#include "window.h"
#include <QApplication>
#include <QPixmap>
#include <QTimer>
/** @brief Launch the application.
 * @param argc Argument count. @param argv Argument vector. @return Qt event-loop result.
 * Pseudocode: initialise Qt; show window; optionally schedule smoke-test exit; run events.
 */
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();
    if (app.arguments().contains("--smoke-test")) {
        QTimer::singleShot(250, &app, &QApplication::quit);
    }
    const int capture = app.arguments().indexOf("--screenshot");
    if (capture >= 0 && capture + 1 < app.arguments().size()) {
        const QString path = app.arguments()[capture + 1];
        QTimer::singleShot(150, &window, [&window, path] { window.grab().save(path); });
    }
    return app.exec();
}
