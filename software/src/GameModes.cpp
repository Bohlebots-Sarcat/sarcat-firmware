//
// Created by Jan Schlegel on 11.12.23.
//

#include "GameModes.h"
#include "BohleBots.h"

BohleBots sarcat;

void GameModes::normal() {
    sarcat.setLed();
    sarcat.fahre(sarcat.ballFahre(3, false), sarcat.normalSpeed, 0);
}
void GameModes::debug() {
    sarcat.fahre(sarcat.ballFahre(3, true), sarcat.testSpeed, 0);
}
