#include "GameStrategy.h"


GameStrategy mode;

elapsedMillis cornerTimer;

int currentGoalRotation;
int currentBallRotation;
int goalDirection;
int ballDirection;
int directionBuffer;

void write();

void setup() {
    cornerTimer = 0;
    bot.init();
    bot.setType(3);

}

Acceleration acceleration;

void loop() {
    bot.sleep(5);

//    Serial.println("Ballrichtung: " + String(bot.ballDirection()));
//    Serial.println("Torrichtung: " + String(bot.goalDirection()));
//    Serial.println("Kompassrichtung " + String(bot.readCompass()));
//    Serial.println("Torrichtung " + String(goalDirection));
//    Serial.println("Tordistanz: " + String(bot.goalDistance()));
//    Serial.println("Timer: " + String(cornerTimer));

    // Serial.printf("error: %d\n", bot.readLinearAcceleration(acceleration));
    bot.readLinearAcceleration(acceleration);

    Serial.print(bot.x);
    Serial.print(",");
    Serial.print(bot.y);
    Serial.print(",");
    Serial.println(bot.z);

    if (bot.boardButton(1)) {
        mode.toggle(1);
        delay(200);
        cornerTimer = 0;
    }

    if (bot.ballDirection() != directionBuffer) cornerTimer = 0;

    directionBuffer = bot.ballDirection();

    if (bot.goalDirection() > 0) {
        currentGoalRotation = -35;
        goalDirection = -1;
    }
    if (bot.goalDirection() < 0) {
        currentGoalRotation = 35;
        goalDirection = 1;
    }
    if (bot.ballDirection() > 0) {
        currentBallRotation = -45;
        ballDirection = -1;
    }
    if (bot.ballDirection() < 0) {
        currentBallRotation = 45;
        ballDirection = 1;
    }

    if (mode.currentMode == 1) {
        if (bot.seesBall()) {
            if (bot.goalAligned()) {
                if (bot.ballDirection() > 4 || bot.ballDirection() < -4) {
                    bot.drive(4, 75, 0);
                    bot.boardLED(2, BLUE);
                }
                if (bot.ballDirection() > -4 && bot.ballDirection() < 0) {
                    bot.drive(-2, 0, 0);
                    bot.boardLED(2, GREEN);
                }
                if (bot.ballDirection() > 0 && bot.ballDirection() < 4) {
                    bot.drive(2, 0, 0);
                    bot.boardLED(2, GREEN);
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
        } else bot.drive(0, 0, 0);

        if (cornerTimer >= 1500) {
            bot.boardLED(2, WHITE);

            if (bot.ballDirection() == 0) {
                bot.drive(0, 0, currentGoalRotation);
            }
            if (bot.ballDirection() != 0) {
                if (ballDirection == 1) {
                   if (bot.ballDirection() != -1) {
                        bot.drive(0, 0, currentBallRotation);
                    }
                }
                if (ballDirection == -1) {
                    if (bot.ballDirection() != 1) {
                        bot.drive(0, 0, currentBallRotation);
                    }
                }
            }
        }
    }
    else {

    }

}