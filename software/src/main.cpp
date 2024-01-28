#include "BohleBots.h"

BohleBots bot;

void setup() {
    bot.init();
    bot.setType(3);
    bot.boardLED(1, OFF);
    bot.boardLED(2, OFF);
}

void loop() {
    bot.sleep(5);
}