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

void GameModes::play() {
    if (lightOn) statusLED();

    bot.fahre(bot.ballRichtung(), 30, 0);

}

void GameModes::statusLED() {
    bot.boardLED(1, bot.siehtBall() ? GRUEN : ROT);
    bot.boardLED(2, bot.siehtTor() ? GRUEN : ROT);
}



