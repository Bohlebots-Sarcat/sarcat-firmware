#include "Bohlebots.h"
#include "GameModes.h"

GameModes game;
BohleBots bot;

void switchMode(int modul, int taster, bool print);

void executeMode();

void switchMode(int modul, int taster, bool print) {
    if (print) Serial.printf("Button Pressed");
    if (modul == 1) {
        switch (taster) {
            case 1:
                game.modus = 1;
            case 2:
                game.modus = 2;
        }
    } else if (modul == 2) {
        switch (taster) {
            case 1:
                game.modus = 3;
            case 2:
                game.modus = 4;
        }
    }
}

void executeMode() {
    switch (game.modus) {
        case 0: {
            game.LED();
            break;
        }
        case 1: {
            game.normal();
            break;
        }
        case 2: {
            game.kickOff();
            break;
        }
        case 3: {
            game.debug();
            break;
        }
        case 4: {
            game.mode4();
            break;
        }
        default: break;
    }
}

void setup() {
    game.modus = 0;
    bot.init();
    bot.setType(3);
    bot.usePixy(true);
}


void loop() {
    if (bot.taster(1, 1)) switchMode(1, 1, true);
    else if (bot.taster(1, 2)) switchMode(1, 2, true);
    else if (bot.taster(2, 1)) switchMode(2, 1, true);
    else if (bot.taster(2, 2)) switchMode(2, 1, true);

    executeMode();

    Serial.printf("Current Modus: %d\n", game.modus);
    bot.warte(20);
}