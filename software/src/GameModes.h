//
// Created by Jan Schlegel on 11.12.23.
//

#ifndef SOFTWARE_GAMEMODES_H  // Include guard
#define SOFTWARE_GAMEMODES_H

const int testSpeed = 25;
const int normalSpeed = 45;
const int StrikeSpeed = 65;

class GameModes {
public:
    void normal();
    void debug();
};

#endif // SOFTWARE_GAMEMODES_H
