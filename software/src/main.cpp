// Spiellogik sarcat
#include "sarcat.h"

BohleBots bot;

// Timer that resets when ball is captured
elapsedMillis ballTime;

void strafeRight();
void strafeLeft();
void normalGame(int, int, bool, bool);
void cornerBehavior();
void cornerExit();

void setup()
{
  bot.set_type(3);
  Serial.print("Bot started");
  ballTime = 0;
}

void loop()
{
  /* if else (taster) */
  normalGame(bot.ballrichtung(), bot.torrichtung(), hat_ball, seheTor);
  cornerBehavior();
  bot.warte(10);
}

void strafeRight()
{
  bot.fahre(1, 65, 0);
  bot.warte(1800);
  normalGame(bot.ballrichtung(), bot.torrichtung(), hat_ball, seheTor);
}

void strafeLeft()
{
  bot.fahre(-1, 65, 0);
  bot.warte(1800);
  normalGame(bot.ballrichtung(), bot.torrichtung(), hat_ball, seheTor);
}

void cornerExit()
{
  bot.spin(69);
}

// Checks if bot is likely to be stuck in a corner
void cornerBehavior()
{
  if (ballTime > 1150 && !seheTor)
  {
    cornerExit();
  }
}

void normalGame(int ballRichtung, int torrichtung, bool hat_ball, bool seheTor)
{
  bot.fahre(ballRichtung, 50, 3);
  if (hat_ball && seheTor)
  {
    bot.fahre(torRichtung, 69, 2);
  }
  else if (hat_ball && !seheTor)
  {
    bot.fahre(ballRichtung, 50, 2);
  }
  else if (!(hat_ball && seheTor))
  {
    bot.fahre(ballRichtung, 50, 2);
  }
  else
  {
    bot.fahre(ballRichtung, 50, 1);
  }
}
