cmake_minimum_required(VERSION 3.16)
project(assignment2_final LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# The core does not depend on Qt.
add_library(game_core STATIC
    src/game.h src/game.cpp
    src/board.h src/board.cpp src/square.h
    src/character.h src/character.cpp
    src/races.h src/races.cpp
    src/item.h src/item.cpp
    src/race.h src/race.cpp src/stats.h
)
target_include_directories(game_core PUBLIC src)

# AUTOMOC processes Q_OBJECT and the named Qt slots in Window.
add_executable(assignment2_final WIN32 MACOSX_BUNDLE
    src/main.cpp src/window.h src/window.cpp
)
set_target_properties(assignment2_final PROPERTIES AUTOMOC ON)
target_link_libraries(assignment2_final PRIVATE game_core Qt${QT_VERSION_MAJOR}::Widgets)

if(MSVC)
    target_compile_options(game_core PRIVATE /W4 /permissive-)
    target_compile_options(assignment2_final PRIVATE /W4 /permissive-)
else()
    target_compile_options(game_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(assignment2_final PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional API documentation. Neither tool is needed to build the game.
find_package(Doxygen QUIET OPTIONAL_COMPONENTS dot)
if(TARGET Doxygen::doxygen AND TARGET Doxygen::dot)
    add_custom_target(documentation
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Generating Doxygen HTML and class diagrams"
        VERBATIM
    )
endif()
