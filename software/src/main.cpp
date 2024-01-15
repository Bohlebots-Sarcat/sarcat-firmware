#include "sarcat.h"

BohleBots bot;

bool modus = false;

void write();

void play();

void setup() {
    bot.init();
    bot.boardled(1, AUS);
    bot.boardled(2, AUS);
}

void loop() {
    if (bot.boardtast(1)) modus = true;
    if (modus) play();

    bot.warte(5);
    write();
}

void write() {
    bot.warte(1500);
    Serial.println("Debug-Info");
    Serial.print("Ballrichtung : ");
    Serial.println(bot.ballDirection);
    Serial.print("Ballzone     : ");
    Serial.println(bot.ballDistance);
    Serial.print("Ball ist da  : ");
    Serial.println(bot.ballExists);
    Serial.println(bot.ballDirection);
    Serial.println("");

    if (bot.ballExists) bot.boardled(1, GRUEN);
    else if (!bot.ballExists) bot.boardled(1, ROT);

    if (bot.goalExists()) bot.boardled(2, GRUEN);
    else if (!bot.goalExists()) bot.boardled(2, ROT);

    bot.pixyStatus();
}

void play() {
    bot.fahre(bot.ballDirection, 30, 20);
}