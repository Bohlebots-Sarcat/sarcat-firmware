
//---- Header System of Sarcat - #Gamemodes ----//

#include "BohleBots.h"
#ifndef SOFTWARE_GAMEMODES_Ha
#define SOFTWARE_GAMEMODES_H

enum MODE {
    STANDBY = 0,
    PLAYING
};

enum LED {
    OFF = 0,
    ON
};

class GameModes {
public:
    /***********************************************
     *
     *  GameModes::init()
     *
     *  Description:
     *      Initializes the game mode with standard settings
     *      at the start of each setup.
     *
     ***********************************************/
    void init();
    /************************************************
     *
     *  GameModes::run()
     *
     *  Description:
     *      Implements the standard behavior of the bot.
     *
     ************************************************/
    void run();
    /************************************************
     *
     *  GameModes::write()
     *
     *  Description:
     *      writes important sensor-data to the Serial Monitor.
     *
     ***********************************************/
    void write();
    /************************************************
     *
     *  GameModes::toggle()
     *
     *  Description:
     *      Toggles the state of a button input, creating an on-off switch functionality.
     *
     */
    void toggle(int button);
private:
    /***********************************************
     *
     *  GameModes::statusLED()
     *
     *  Description:
     *      Responsible for changing LED Colors based on sensor outputs.
     *
     ***********************************************/
    void statusLED();
    /************************************************
     *
     *  GameModes::play()
     *
     *  Description:
     *      Executing strategy of bot.
     *
     ***********************************************/
    void play();

    /// Mode State Variables ///

    int currentMode;
    int currentLED;

    /// Speed Variables ///

    const int NORMALSPEED = 35;
    const int STRIKESPEED = 60;

};


#endif //SOFTWARE_GAMEMODES_H
