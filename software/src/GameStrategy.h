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
    EDGE,
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
                        cornerTimer = 0;
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
                Serial.printf("Goal Height: %d\n", bot.goalDistance());
                Serial.printf("Goal Aligned: %d\n", bot.goalAligned());
                Serial.printf("-- Goal --\n");
                break;
            }
            case EDGE: {
                Serial.printf("-- Edge --\n");
                Serial.printf("Edge: %d\n", edge);
                Serial.printf("Ball Direction: %d\n", bot.ballDirection());
                Serial.printf("Goal Direction: %d\n", bot.goalDirection());
                Serial.printf("-- Edge --\n");
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
        if (isInCorner) {
            escapeCorner();
        }
        if (!isInCorner) {
            if (bot.seesGoal() || absoluteAligned()) {
                if (ballIsAhead()) {
                    if (!striker) {
                        if (edge) {
                            bot.drive(0, 60, 0);
                        }
                        if (!edge) {
                            if (ballIsLeft()) bot.drive(-2, 60, 0);
                            if (ballIsRight()) bot.drive(2, 60, 0);
                        }
                        if (bot.ballDirection() == 0) striker = true;
                    }

                    if (striker) {
                        if (bot.goalAligned()) {
                            if (abs(bot.ballDirection()) <= 1) {
                                bot.drive(0, 80, -bot.goalDirection() / 3);
                            }
                        }

                        if (abs(bot.ballDirection()) >= 2 || abs(bot.goalDirection()) > 30) {
                            striker = false;
                        }
                    }
                }
                if (ballIsBehind()) {
                    bot.drive(5, 70, compasRotation());
                }
            }
            if (!bot.seesGoal() && !absoluteAligned()) {
                bot.drive(0, 0, compasRotation());
            }
        }
        updateSensors();
    }
    void updateSensors() {
        if (abs(bot.goalDirection()) < 40 || ballIsBehind() || !bot.seesGoal()) edge = false;

        if (cornerTimer > 1250 && !absoluteAligned() && bot.goalDirection() < 60 && !bot.hasBall()) isInCorner = true;

        if (bot.goalAligned() && cornerTimer < 2000) isInCorner = false;

        if (!bot.seesGoal() && bot.hasBall()) isInCorner = true;

        if (cornerTimer > 2000) isInCorner = true;

        if (abs(bot.goalDirection()) > 40 && abs(bot.ballDirection()) <= 1) edge = true;

        if (bot.ballDirection() != _ballCache) {
            cornerTimer = 0;
        }

        if (bot.goalAligned()) {
            _goalAligned = true;
        }
        if (abs(bot.goalDirection()) > 65) {
            _goalAligned = false;
        }

        if (bot.goalDirection() > 0) {
            _currentGoalRotation = -40;
        }
        else {
            _currentGoalRotation = 40;
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
        if (abs(bot.ballDirection()) == 1 || bot.ballDirection() == 0) {
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
        bot.drive(0, 0, bot.compass() / 4);
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

    bool absoluteAligned() {
        if (abs(bot.compass()) < 15) {
            return true;
        }
        else {
            return false;
        }
    }

    int compasRotation() {
        if (abs(bot.compass()) < 10) {
            return 0;
        }

        if (abs(bot.compass()) > 60) {
            if (bot.compass() > 0) {
                return 30;
            }
            else {
                return -30;
            }
        }
        if (abs(bot.compass()) < 30) {
            if (bot.compass() > 0) {
                return 15;
            }
            else {
                return -15;
            }
        }
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
        return (bot.ballDirection() > -7 && bot.ballDirection() <= -1);
    }

    bool ballIsRight() {
        return (bot.ballDirection() >= 1 && bot.ballDirection() < 7);
    }

    int _currentMode;

    bool _goalAligned;
    bool edge;
    bool striker;

    int _currentGoalRotation;
    int _currentBallRotation;
    int _ballDirection;
    int _ballCache;
    bool isInCorner;

    bool _kickOff;

};


#endif //SOFTWARE_GAMESTRATEGY_H
