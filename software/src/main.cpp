#include "GameModes.h"

GameModes mode;

void setup() {
    mode.init();
}

void loop() {
    if (mode.isStandby) {
    }
    else mode.play();

    mode.wait();
}