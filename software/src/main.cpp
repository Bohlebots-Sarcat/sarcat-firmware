#include "GameModes.h"

GameModes mode;

void setup() {
    mode.init();
    mode.isStandby = true;
}

void loop() {
    BohleBots bot;

    if (bot.boardTaster(1)) mode.toggle(1);
    if (bot.boardTaster(2)) mode.toggle(2);

    mode.run();

    bot.warte(5);
}
