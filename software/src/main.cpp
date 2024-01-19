#include "GameModes.h"

GameModes mode;

void setup() {
    mode.init();
}

void loop() {
    if (mode.isStandby) bot.write();
    else mode.play();

    mode.wait();
}