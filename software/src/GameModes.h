
//---- Header System of Sarcat - #Gamemodes ----//

#include "BohleBots.h"
#ifndef SOFTWARE_GAMEMODES_H
#define SOFTWARE_GAMEMODES_H

class GameModes {
public:
    /***********************************************
     *
     *   Status modes of the bot:
     *
     *   bool isStandby = if bot !playing
     *   bool lightOn = activates lights of bot
     *
     **********************************************/
    bool isStandby;
    bool isPlaying;
    bool lightOn;
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
     *  GameModes::play()
     *
     * Description:
     *   Implements the standard strategy for the bot.
     *
     ************************************************/
    void play();
    /************************************************
     *
     *  GameModes::
     */
     void write();
private:
    /***********************************************
     *
     *  GameModes::statusLED()
     *
     *  Description:
     *      responsible for changing LED Colors based on sensor outputs.
     *
     ***********************************************/
    void statusLED();

};


#endif //SOFTWARE_GAMEMODES_H
