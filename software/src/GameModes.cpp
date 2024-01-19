//
// Created by Jan Schlegel on 19.01.24.
//

#include "GameModes.h"

void GameModes::init() {
    bot.init();
    bot.boardled(1, AUS);
    bot.boardled(2, AUS);
}

void GameModes::wait() {
    bot.warte(5);
}

void GameModes::play() {
    bot.fahre(bot.ballDirection, 30, bot.currentRotation());
}


