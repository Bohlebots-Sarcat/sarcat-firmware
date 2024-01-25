#include "GameModes.h"

GameModes mode;

void setup() {
    mode.init();
    mode.isStandby = true;
}

void loop() {
    BohleBots bot;

    if (bot.boardTaster(1)) {
        mode.play();
    }
    if (bot.boardTaster(2)) {
        mode.lightOn = true;
    }

    bot.warte(5);
}
