//
// Created by Jan Schlegel on 19.01.24.
//

#include "GameModes.h"

BohleBots bot;

void GameModes::init() {
    bot.init();
    bot.boardLED(1, AUS);
    bot.boardLED(2, AUS);
}

void GameModes::wait() {
    bot.warte(5);
}

void GameModes::play() {
    bot.fahre(bot.ballRichtung(), 30, 0);
}


