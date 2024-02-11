#include "GameStrategy.h"

GameStrategy mode;

int currentGoalRotation;
int currentBallRotation;
int goalDirection;
int ballDirection;
int ballCache;
bool isInCorner;;
bool kickOff;

void write();

void setup() {

    cornerTimer = 0;
    kickOff = true;

    bot.init();
    bot.setType(3);

}

Acceleration acceleration;

void loop() {
    bot.sleep(5);

    Serial.println("1: " + String(bot.input(1)));
    Serial.println("2: " + String(bot.input(2)));
    Serial.println("3: " + String(bot.input(3)));
    Serial.println("4: " + String(bot.input(4)));

    if (bot.boardButton(1)) {
        mode.toggle(1);
        delay(200);
        cornerTimer = 0;
    }

    mode.run();

    if (mode.currentMode() == 1) {

        if (kickOff) {
            if (bot.boardButton(2)){
                bot.drive(-1, 90, 0);
                bot.sleep(400);
                kickOff = false;
            }
            if (bot.boardButton(3)) {
                bot.drive(1, 90, 0);
                bot.sleep(400);
                kickOff = false;
            }
        }

        if (bot.seesBall()) {
            if (bot.goalAligned()) {
                if (bot.ballDirection() > 4 || bot.ballDirection() < -4) {
                    bot.drive(4, 85, 0);
                    bot.boardLED(2, BLUE);
                }
                if (bot.ballDirection() > -4 && bot.ballDirection() < 0) {
                    bot.drive(-2, 0, 0);
                    bot.boardLED(1, GREEN);
                }
                if (bot.ballDirection() > 0 && bot.ballDirection() < 4) {
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

                bot.drive(0, 0, currentGoalRotation);
                bot.boardLED(2, RED);
            }

            if (bot.ballDirection() != ballCache) {
                cornerTimer = 0;
            }

            ballCache = bot.ballDirection();
        }

        else bot.drive(0, 0, 0);
    }
    else {

    }
}