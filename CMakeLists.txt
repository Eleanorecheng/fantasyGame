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

# A separate console program checks the core rules.
include(CTest)
if(BUILD_TESTING)
    add_executable(basic_tests tests/basic_tests.cpp)
    target_link_libraries(basic_tests PRIVATE game_core)
    add_test(NAME basic_rules COMMAND basic_tests)
endif()

if(MSVC)
    target_compile_options(game_core PRIVATE /W4 /permissive-)
    target_compile_options(assignment2_final PRIVATE /W4 /permissive-)
    if(BUILD_TESTING)
        target_compile_options(basic_tests PRIVATE /W4 /permissive-)
    endif()
else()
    target_compile_options(game_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(assignment2_final PRIVATE -Wall -Wextra -Wpedantic)
    if(BUILD_TESTING)
        target_compile_options(basic_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
