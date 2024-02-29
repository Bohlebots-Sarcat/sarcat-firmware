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
    MODE
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
                    default: break;
                }
                break;
            }
            case 2: {
                switch (_currentMode) {
                    case 0:
                        _currentMode = 2;
                        break;
                    case 1:
                        _currentMode = 0;
                        break;
                    default:
                        break;
                }
                break;
            }
            case 3: {
                switch (_currentMode) {
                    case 0:
                        _kickOff = true;
                        break;
                    case 1:
                        _currentMode = 0;
                        break;
                    default:
                        break;
                }
                break;
            }
        }
    }

    void run() {
        switch (_currentMode) {
            case 0:
                bot.boardLED(1, OFF);
                bot.boardLED(2, OFF);
                bot.drive(0, 0, 0);
                break;
            case 1:
                play();
                break;
            case 2: {
                function();
                break;
            }
            default: break;
        }
        statusLED();
    }

    void debug(int mode) {
        switch (mode) {
            case BALL: {
                Serial.printf("-- Ball --\n");
                Serial.printf("Ball: %d\n", bot.ballDirection());
                Serial.printf("Has Ball: %d\n", bot.input(4));
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
                Serial.printf("Corner Cache: %d\n", _ballCache);
                Serial.println("Corner Timer: " + String(cornerTimer));
                Serial.printf("-- Corner --\n");
                break;
            }
            case MODE: {
                Serial.printf("-- Mode --\n");
                Serial.printf("Mode: %d\n", _currentMode);
                Serial.printf("-- Mode --\n");
                break;
            }
        }
    }

private:
    void play() {
        if (_goalAligned) {
            if (ballIsAhead()) {
                if (ballIsRight()) {
                    if (bot.ballDirection() != 1) {
                        bot.drive(2, 70, 0);
                    }
                }
                if (ballIsLeft()) {
                    if (bot.ballDirection() != -1) {
                        bot.drive(-2, 70, 0);
                    }
                }
                if (abs(bot.ballDirection()) == 1) {
                    bot.drive(0, 90, 0);
                }
            }
            if (ballIsBehind() && !isInCorner) {
                if (ballIsBack()) {
                    if (_currentGoalRotation > 0) bot.drive(2, 80, 0);
                    if (_currentGoalRotation < 0) bot.drive(-2, 80, 0);
                }
                if (!ballIsBack()) {
                    bot.drive(4, 100, 0);
                }
            }
        }
        if (!_goalAligned) {
            if (!bot.seesGoal()) {
                bot.drive(0, 0, _currentBallRotation);
            }
        }
        updateSensors();
    }
    void updateSensors() {
        if (bot.goalAligned() && cornerTimer < 2000) isInCorner = false;

        if (bot.goalAligned()) {
            _goalAligned = true;
        }
        if (abs(bot.goalDirection()) > 65) {
            _goalAligned = false;
        }

        if (bot.goalDirection() > 0) {
            _currentGoalRotation = -25;
        }
        else {
            _currentGoalRotation = 25;
        }

        if (bot.ballDirection() > 0) {
            _currentBallRotation = -30;
            _ballDirection = -1;
        }
        else {
            _currentBallRotation = 30;
            _ballDirection = 1;
        }
        _ballCache = bot.ballDirection();
    }

    void escapeCorner() {
        updateSensors();
        if (abs(bot.ballDirection()) == 1 || bot.ballDirection() == 0)
        {
            if (!bot.hasBall()) bot.drive(0, 40, 0);
            if (bot.hasBall()) {
                if (!bot.seesGoal()) {
                    bot.drive(0, 0, _currentGoalRotation);
                }
                if (bot.goalAligned()) {
                    (bot.drive(0, 80, 0));
                }
            }
        }
        else {
            bot.drive(0, 0, _currentBallRotation);
        }
    }

    void function() {
    }

    void statusLED() {
        if (bot.seesBall()) bot.boardLED(1, GREEN);
        if (!bot.seesBall()) bot.boardLED(1, RED);
        if (ballIsBehind()) bot.boardLED(1, BLUE);

        if (bot.goalAligned()) bot.boardLED(2, GREEN);
        if (!bot.goalAligned()) bot.boardLED(2, BLUE);
        if (!bot.seesGoal()) bot.boardLED(2, RED);

        if (isInCorner) bot.boardLED(1, WHITE);
        if (isInCorner) bot.boardLED(2, WHITE);
    }

    bool ballIsAhead() {
        return (bot.ballDirection() > -4 && bot.ballDirection() < 4);
    }

    bool ballIsBehind() {
        return (bot.ballDirection() <= -4 || bot.ballDirection() >= 4);
    }

    bool ballIsBack() {
        return (bot.ballDirection() == 8);
    }

    bool ballIsLeft() {
        return (bot.ballDirection() > -3 && bot.ballDirection() < 0);
    }

    bool ballIsRight() {
        return (bot.ballDirection() > 0 && bot.ballDirection() < 3);
    }

    int _currentMode;

    bool _goalAligned;

    int _currentGoalRotation;
    int _currentBallRotation;
    int _ballDirection;
    int _ballCache;
    bool isInCorner;

    bool _kickOff;

};


#endif //SOFTWARE_GAMESTRATEGY_H
