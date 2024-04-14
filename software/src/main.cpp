#include "GameStrategy.h"

GameStrategy mode;

void setup() {
    bot.init();
    bot.setType(3);
    nc.begin();
}

void loop() {
    if (bot.boardButton(1)) {
        mode.toggle(1);
        delay(200);
    }
    if (bot.boardButton(2)) {
        mode.toggle(2);
        delay(200);
    }
    if (bot.boardButton(3)) {
        bot.motor(4, -100);
    }
    if (!bot.boardButton(3)) {
        bot.motor(4, 0);
    }

    mode.run();
    mode.debug(BALL);
    nc.handle();
    bot.sleep(5);
}