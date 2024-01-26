#include "GameModes.h"
#include "BohleBots.h"

GameModes mode;

void setup() {
    mode.init();
    mode.isStandby = true;
    mode.lightOn = false;
    mode.isPlaying = false;
}

void loop() {
    BohleBots bot;

    if (bot.boardTaster(1)) {
        mode.isPlaying = true;
    }

    if (mode.isPlaying) { mode.play(); }

    Serial.println(bot.ballRichtung());

    bot.warte(5);
}
