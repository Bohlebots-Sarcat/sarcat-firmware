//
// Created by Jan Schlegel on 01.02.24.
//

#include "BohleBots.h"

BohleBots bot;

elapsedMillis cornerTimer;

#ifndef SOFTWARE_GAMESTRATEGY_H
#define SOFTWARE_GAMESTRATEGY_H

enum DEBUG {
    BALL,
    GOAL,
    COMPASS,
    GYRO,
    CORNER,
};

class GameStrategy {

public :
    void toggle(int button) {
        switch (button) {
            case 1: {
                switch (_currentMode) {
                    case 0:
                        _currentMode = 1;
                        break;
                    case 1:
                        _currentMode = 0;
                        break;
                }
                break;
            }
            case 2: {
                switch (_currentLED) {
                    case 0:
                        _currentLED = 1;
                        break;
                    case 1:
                        _currentLED = 0;
                        break;
                }
                break;
            }
        }
    }

    void run() {
        updateSensors();
        switch (_currentMode) {
            case 0:
                bot.boardLED(1, OFF);
                bot.boardLED(2, OFF);
                bot.drive(0, 0, 0);
                break;
            case 1:
                bot.boardLED(1, MAGENTA);
                bot.boardLED(2, MAGENTA);
                play();
                break;
        }
    }

    void debug(int mode) {
        switch (mode) {
            case BALL: {
                Serial.printf("-- Ball --\n");
                Serial.printf("Ball: %d\n", bot.ballDirection());
                Serial.printf("-- Ball --\n");
                break;
            }
            case GOAL: {
                Serial.printf("-- Goal --\n");
                Serial.printf("Goal: %d\n", bot.goalDirection());
                Serial.printf("Goal Aligned: %d\n", bot.goalAligned());
                Serial.printf("-- Goal --\n");
                break;
            }
            case COMPASS: {
                Serial.printf("-- Compass --\n");
                Serial.printf("Compass: %d\n", bot.compass());
                Serial.printf("-- Compass --\n");
                break;
            }
            case GYRO: {
                Serial.printf("-- Gyro --\n");
                Serial.printf("Gyro: \n");
                Serial.printf("-- Gyro --\n");
                break;
            }
            case CORNER: {
                Serial.printf("-- Corner --\n");
                Serial.printf("Corner: %d\n", isInCorner);
                Serial.printf("Corner Cache: %d\n", cornerCache);
                Serial.printf("-- Corner --\n");
                break;
            }
        }
    }

private:
    void play() {
        if (_kickOff) {
            if (bot.boardButton(2)){
                bot.drive(-1, 80, 0);
                bot.sleep(400);
                _kickOff = false;
            }
            if (bot.boardButton(3)) {
                bot.drive(1, 80, 0);
                bot.sleep(400);
                _kickOff = false;
            }
        }

        if (bot.seesBall()) {
            if (bot.goalAligned()) {

                cornerCache = false;

                if (ballIsBehind()) {
                    bot.drive(4, 85, 0);
                    bot.boardLED(2, BLUE);
                }
                if (ballIsLeft()) {
                    bot.drive(-2, 0, 0);
                    bot.boardLED(1, GREEN);
                }
                if (ballIsRight()) {
                    bot.drive(2, 0, 0);
                    bot.boardLED(1, GREEN);
                }
                if (bot.goalDirection() < 0) {
                    if (bot.ballDirection() == 0 || bot.ballDirection() == 1) {
                        bot.boardLED(2, GREEN);
                        bot.drive(0, 80, 0);
                    }
                }
                if (bot.goalDirection() > 0) {
                    if (bot.ballDirection() == 0 || bot.ballDirection() == -1) {
                        bot.drive(0, 90, 0);
                    }
                }

                bot.boardLED(1, GREEN);
            }

            if (!bot.goalAligned()) {
                if (bot.goalLeft() && bot.ballDirection() == 0) {
                    bot.drive(0, 80, 0);
                }

                if (bot.goalRight() && bot.ballDirection() == 0) {
                    bot.drive(0, 80, 0);
                }

                bot.drive(0, 0, _currentGoalRotation);
                bot.boardLED(2, RED);

                if (isInCorner || cornerCache) {
                    cornerCache = true;
                    bot.boardLED(1, WHITE);
                    bot.boardLED(2, WHITE);

                    if (_currentGoalRotation == -35) {
                        bot.drive(0, 2, 0);
                    }
                    if (_currentGoalRotation == 35) {
                        bot.drive(0, -2, 0);
                    }
                    debug(CORNER);
                }

            }

        }

        else bot.drive(0, 0, 0);

    }

    void updateSensors() {
        if (bot.isInCorner(cornerTimer, ballCache, bot.compass())) {
            isInCorner = true;
        }
        else {
            isInCorner = false;
        }

        if (bot.goalDirection() > 0) {
            _currentGoalRotation = -35;
        }
        else {
            _currentGoalRotation = 35;
        }

        if (bot.ballDirection() > 0) {
            _currentBallRotation = -45;
            ballDirection = -1;
        }
        else {
            _currentBallRotation = 45;
            ballDirection = 1;
        }
    }

    bool ballIsAhead() {
        return (bot.ballDirection() > -4 && bot.ballDirection() < 4);
    }

    bool ballIsBehind() {
        return (bot.ballDirection() < -4 || bot.ballDirection() > 4);
    }

    bool ballIsLeft() {
        return (bot.ballDirection() > -4 && bot.ballDirection() < 0);
    }

    bool ballIsRight() {
        return (bot.ballDirection() > 0 && bot.ballDirection() < 4);
    }

    int _currentMode;
    int _currentLED;

    int _currentGoalRotation;
    int _currentBallRotation;
    int ballDirection;
    int ballCache;
    bool isInCorner;
    bool cornerCache;
    bool _kickOff;

};


#endif //SOFTWARE_GAMESTRATEGY_H
