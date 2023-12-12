//
// Created by Jan Schlegel on 11.12.23.
//

#include "GameModes.h"
#include "BohleBots.h"

BohleBots bot;

void GameModes::menu1() {
    if (bot.taster(1, 1))
}

void GameModes::menu2() {

}
void GameModes::menu3() {

}
void GameModes::menu4() {

}

void GameModes::setLED(int primaryButton) {
    switch (primaryButton) {
        case 1: {
            bot.setLed(1, 1, bot.primaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            break;
        }
        case 2: {
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.primaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            break;
        }
        case 3: {
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.primaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            break;
        }
        case 4: {
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.primaryLED);
            break;
        }
        default: {
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            bot.setLed(1, 1, bot.secondaryLED);
            break;
        }
    }
}

void GameModes::LED() {
    setLED(0);
}

void GameModes::normal() {
    setLED(1);
    bot.fahre(bot.ballFahre(3, false), bot.normalSpeed, 0);
}

void GameModes::debug() {
    setLED(2);
    bot.fahre(bot.ballFahre(3, true), bot.testSpeed, 0);
}

void GameModes::kickOff() {
    int direction = random(1);
    Serial.printf("Direction: ");
    setLED(3);
}

void GameModes::mode4() {
    setLED(4);
}
