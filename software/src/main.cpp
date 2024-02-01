#include "BohleBots.h"

BohleBots bot;

int currentModus;
int currentRotation;

void write();

void setup() {
    bot.init();

    bot.setType(3);

    currentModus = 0;
    currentRotation = 50;
    bot.boardLED(1, MAGENTA);
    bot.boardLED(2, MAGENTA);
    bot.sleep(125);
    bot.boardLED(1, OFF);
    bot.boardLED(2, OFF);
}

void loop() {
    bot.sleep(5);

    if (bot.boardButton(1)) currentModus = 1;

    if (bot.goalDirection() > 0) currentRotation = -35;
    if (bot.goalDirection() < 0) currentRotation = 35;

    if (currentModus == 1) {
        if (bot.seesBall()) {
            if (bot.goalAligned()) {
                if (bot.ballDirection() > 4 || bot.ballDirection() < -4) {
                    bot.drive(4, 75, 0);
                    bot.boardLED(2, BLUE);
                }
                if (bot.ballDirection() > -4 && bot.ballDirection() < 0) {
                    bot.drive(-2, 0, 0);
                }
                if (bot.ballDirection() > 0 && bot.ballDirection() < 4) {
                    bot.drive(2, 0 , 0);
                }

                if (bot.goalDirection() < 0) {
                    if (bot.ballDirection() == 0 || bot.ballDirection() == 1) {
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

                bot.drive(0, 0, currentRotation);
                bot.boardLED(1, RED);
            }
        }
        else bot.drive(0, 0, 0);
    }

    else {
        Serial.println("Ballrichtung: " + String(bot.ballDirection()));
        Serial.println("Torrichtung: " + String(bot.goalDirection()));
    }

}
