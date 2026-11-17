cmake_minimum_required(VERSION 3.16)
project(FantasyGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# List declarations as well as definitions so Qt Creator displays all modules.
add_library(game_core STATIC
    src/game.h src/game.cpp
    src/board.h src/board.cpp
    src/square.h
    src/character.h src/character.cpp
    src/races.h src/races.cpp
    src/race.h src/race.cpp
    src/item.h src/item.cpp
    src/stats.h
)
target_include_directories(game_core PUBLIC src)

add_executable(FantasyGame MACOSX_BUNDLE WIN32
    src/main.cpp src/window.h src/window.cpp
)
target_link_libraries(FantasyGame PRIVATE game_core Qt${QT_VERSION_MAJOR}::Widgets)

# All targets use the same compiler checks, including the tests.
function(enable_project_warnings target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()
enable_project_warnings(game_core)
enable_project_warnings(FantasyGame)

include(CTest)
if(BUILD_TESTING)
    add_executable(core_tests tests/core_tests.cpp)
    target_link_libraries(core_tests PRIVATE game_core)
    enable_project_warnings(core_tests)
    add_test(NAME core_rules COMMAND core_tests)

    # Qt Test is optional. The application itself only requires Widgets.
    find_package(Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Test)
    if(TARGET Qt${QT_VERSION_MAJOR}::Test)
        add_executable(gui_tests tests/gui_tests.cpp src/window.h src/window.cpp)
        target_link_libraries(gui_tests PRIVATE game_core
            Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)
        enable_project_warnings(gui_tests)
        add_test(NAME gui_controls COMMAND gui_tests)
        set_tests_properties(gui_controls PROPERTIES
            ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 30)
    endif()
endif()
