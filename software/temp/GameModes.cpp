//
// Created by Jan Schlegel on 19.01.24.
//

#include "GameModes.h"

BohleBots bot;

void GameModes::init() {
    bot.init();
    bot.boardLED(1, AUS);
    bot.boardLED(2, AUS);

    currentMode = STANDBY;
    currentLED = OFF;
}

void GameModes::run() {
    switch (currentMode) {
        case STANDBY: {
            write();
            currentLED = ON;
            break;
        }
        case PLAYING: {
            play();
            break;
        }
    }
    write();
    if (currentLED == ON) {
        statusLED();
    }

}

void GameModes::write() {
    Serial.println(bot._irpaket);
}

void GameModes::toggle(int button) {
    switch (button) {
        case 1: {
            switch (currentMode) {
                case STANDBY:
                    currentMode = PLAYING;
                    break;
                case PLAYING:
                    currentMode = STANDBY;
                    break;
            }
            break;
        }
        case 2: {
            switch (currentLED) {
                case OFF:
                    currentLED = ON;
                    break;
                case ON:
                    currentLED = OFF;
                    break;
            }
            break;
        }
    }
}

void GameModes::statusLED() {
    bot.boardLED(1, bot.siehtBall() ? GRUEN : ROT);
    bot.boardLED(2, bot.siehtTor() ? GRUEN : ROT);
}

void GameModes::play() {
    bot.drive(bot.ballRichtung(), NORMALSPEED, bot.torRichtung() / 2);
}


