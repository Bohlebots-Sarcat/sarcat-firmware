#include "GameModes.h"

BohleBots bot;

void GameModes::init() {
    bot.init();
    bot.boardLED(1, OFF);
    bot.boardLED(2, OFF);

    bot.setType(3);

    currentMode = STANDBY;
    currentLED = 1;
}

void GameModes::run() {
    switch (currentMode) {
        case STANDBY: {
            write();
            currentLED = 1;
            break;
        }
        case PLAYING: {
            play();
            break;
        }
    }
    write();
    if (currentLED == 1) {
        statusLED();
    }

}

void GameModes::write() {
    Serial.println(bot.ballDirection());
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
                case 0:
                    currentLED = 1;
                    break;
                case 1:
                    currentLED = 0;
                    break;
            }
            break;
        }
    }
}

void GameModes::statusLED() {
    if (bot.seesBall()) bot.boardLED(1, GREEN); else bot.boardLED(1, RED);
    if (bot.seesGoal()) bot.boardLED(2, GREEN); else bot.boardLED(2, RED);
}

void GameModes::play() {

}



