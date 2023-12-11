#include "BohleBots.h"
#include "GameModes.h"

GameModes mode;
BohleBots bot;

namespace sarcat {
    int modus = 0;

    void test();
    void switchMode(int modul, int taster, bool print);
    void executeMode();
}

void sarcat::switchMode(int modul, int taster, bool print) {
    if (print) Serial.printf("Button Pressed");
    if (modul == 1) {
        switch (sarcat::modus) {
            case 1:
                sarcat::modus = 0;
            case 2:
            case 3:
            case 4:
                sarcat::modus = 1;
        }
    } else if (modul == 2) {
        switch (sarcat::modus) {
            case 2:
                sarcat::modus = 0;
            case 1:
            case 3:
            case 4:
                sarcat::modus = 2;
        }
    }
}

void sarcat::executeMode() {
    switch (sarcat::modus) {
        case 1: mode.normal();
        case 2: mode.debug();
        default: break;
    }
}

void setup() {
    bot.init();
    bot.setType(3);
    bot.usePixy(true);
}


void loop() {
    if (bot.taster(1, 1)) sarcat::switchMode(1, 1, true);
    if (bot.taster(1, 2)) sarcat::switchMode(1, 2, true);
    if (bot.taster(2, 1)) sarcat::switchMode(2, 1, true);
    if (bot.taster(2, 2)) sarcat::switchMode(2, 1, true);

    sarcat::executeMode();

    Serial.printf("Current Modus: %d\n", sarcat::modus);
    bot.warte(20);
}