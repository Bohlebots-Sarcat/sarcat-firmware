//
// Created by Jan Schlegel on 11.12.23.
//

#include "GameModes.h"
#include "BohleBots.h"

BohleBots bot;

void GameModes::LED() {
    bot.setLed(1, 1, 1);
    bot.setLed(1, 1, 1);
    bot.setLed(1, 1, 1);
    bot.setLed(1, 1, 1);
}

void GameModes::normal() {
    bot.setLed(1, 1, bot.primaryLED);
    bot.fahre(bot.ballFahre(3, false), bot.normalSpeed, 0);
}
void GameModes::debug() {
    bot.fahre(bot.ballFahre(3, true), bot.testSpeed, 0);
}
