// Spiellogik
# include "sarcat.h"

BohleBots bot;

void setup() {
  bot.set_type(3);
  Serial.print("Bot started");
}

void loop() {
  
}

void strafeRight() {
  bot.fahre(1, 65, 0);
  bot.warte(1800);
  normalGame(bot.ballrichtung(), bot.torrichtung() );
}

void startLeft() {
  bot.fahre(-1, 65, 0);
  bot.warte(1800);
  normalGame(bot.ballrichtung(), bot.torrichtung() );
}

void normalGame(int ballRichtung, int torrichtung, bool hat_ball, bool sehe_tor) {
  bot.fahre(ballRichtung, 50, 3);
  if (hat_ball && sehe_tor) {
    bot.fahre(torRichtung, 69, 0);
  }
  else if (hat_ball &! sehe_tor) {
    bot.fahre(ballRichtung, 50, 2);
  }
  else if (hat_ball = false, sehe_tor = false) {
    bot.fahre(ballRichtung, 50, 2);
  }
  else {
    bot.fahre(ballRichtung, 50, 1);
  }
}

