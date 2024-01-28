#include "bohlebots.h"

BohleBots bot;

int modus = 0;
int tastled = 1;
int Torfahrgeschwindigkeit = 0;

void schreibe();
void fahre();

void setup()
{
    bot.set_bot_type(3);
    bot.init();
    bot.set_pixy(true);
    bot.boardled(1, AUS);
    bot.boardled(2, AUS);
}

void loop()
{
    bot.warte(5);
    if (bot.boardtast(1)) {
        modus = 1;
    }

    if (modus == 0)
    {
        bot.boardled(1, AUS);
        bot.boardled(2, AUS);
        bot.led(tastled, 2, AUS);
        bot.led(tastled, 1, AUS);
        bot.fahre(0, 0, 0);
    }

    if (modus == 1)
    {
        bot.boardled(1, BLAU);
        bot.boardled(2, BLAU);
        bot.led(tastled, 2, BLAU);
        bot.led(tastled, 1, BLAU);
        fahre();
    }

    schreibe();
}

void schreibe()
{
    Serial.println("Debug-Info");
    Serial.print("Ballrichtung : "); Serial.println(bot.ballDirection);
    Serial.print("Ballzone     : "); Serial.println(bot.ballDistance);
    Serial.print("Ball ist da  : "); Serial.println(bot.ballExists);
    Serial.print("Torrichtung : "); Serial.println(bot.goalDirection);
    Serial.print("Torentfernung : "); Serial.println(bot.goalDistance);

    Serial.println("");
}


void fahre() {

    int currentRotation;

    if (bot.goalDirection < 0) currentRotation = 40;
    if (bot.goalDirection > 0) currentRotation = -40;

    if (bot.goalExists) {
        if (bot.ballDirection == 1 && bot.ballDistance == 1.5) {
            // Left strike
            bot.fahre(1, 60, bot.ballDirection * 1.5);
            if (bot.ballDirection == 0) {
                if (abs(bot.ballDirection) == 1 || bot.ballDirection == 0) bot.fahre(0, 80, 0);
            }
        }
        if (bot.ballDirection == -1 && bot.ballDirection == -1.5) {
            // Right strike
            bot.fahre(-1, 60, bot.ballDirection * 1.5);
            if (bot.ballDirection == 0) {
                if (abs(bot.ballDirection) == 1 || bot.ballDirection == 0) bot.fahre(0, 80, 0);
            }
        }
    }
}