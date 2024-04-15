#include <GameStrategy.h>

GameStrategy mode;

void setup() {
    Serial.begin(115200);
    bot.init();
    bot.setType(3);
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
        mode.toggle(3);
        delay(200);
    }

    mode.run();
    mode.debug(COMPASS);
}