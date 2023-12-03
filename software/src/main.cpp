#include "BohleBots.h"

BohleBots bot;
elapsedMillis printState;

const int testSpeed = 25;
const int normalSpeed = 40;
const int strikeSpeed = 70;

const int idleModus = 0;
const int playModus = 1;

int currentModus = idleModus;

void changeModus();
void executeIdleness();
void printIdleness();
void play();

void setup()
{
    bot.setBotTyp(4);
    bot.init();
    bot.setBenutztPixy(true);
    bot.setBenutztKompass(false);
    Serial.println("Bot started");
}

void loop() {
    if (bot.taster(1, 2)) changeModus();
    switch(currentModus) {
        case idleModus:
            executeIdleness();
            break;
        case playModus:
            play();
            break;
    }
    bot.warte(20);
}

void executeIdleness() {
    if (bot.taster(1, 2)) changeModus();
    Serial.println("Bot Currently Idle " + String("Ball: ") + String(bot.ballRichtung()) + String(" ") + "Tor: " + String(bot.torRichtung()) + String(" ") + String("trueBall: " + String(bot.ballFahre())));
}

void play() {
    bot.fahre(bot.ballFahre(), testSpeed, bot.torRichtung() / 2);
    if (bot.taster(1, 2)) changeModus();
}

void changeModus() {
    switch (currentModus) {
        case idleModus:
            currentModus = playModus;
            break;
        case playModus:
            currentModus = idleModus;
            break;
        default:
            currentModus = idleModus;
            break;
    }
    Serial.println(currentModus);
}