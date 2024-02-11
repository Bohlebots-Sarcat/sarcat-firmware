#include "GameStrategy.h"

GameStrategy mode;

void setup() {
    bot.init();
    bot.setType(3);
}

void loop() {
    bot.sleep(5);

    if (bot.boardButton(1)) {
        mode.toggle(1);
        delay(200);
        cornerTimer = 0;
    }

    mode.run();
}