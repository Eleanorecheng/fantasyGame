QT += widgets
CONFIG += c++17 warn_on
TEMPLATE = app
TARGET = assignment2_final
SOURCES += src/main.cpp src/window.cpp src/game.cpp src/board.cpp \
           src/character.cpp src/races.cpp src/item.cpp src/race.cpp
HEADERS += src/window.h src/game.h src/board.h src/square.h \
           src/character.h src/races.h src/item.h src/race.h src/stats.h
