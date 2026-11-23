/** @file main.cpp @brief Start the basic Qt Widgets application. */
#include "window.h"
#include <QApplication>

/** @brief Program entry point.
 * @param argc Command-line argument count. @param argv Command-line arguments.
 * @return Qt application exit code.
 * Pseudocode: CREATE Qt application; CREATE and SHOW window; PROCESS user events.
 */
int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    Window window;
    window.show();
    return application.exec();
}
