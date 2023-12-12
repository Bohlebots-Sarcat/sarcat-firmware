//
// Created by Jan Schlegel on 11.12.23.
//

#ifndef SOFTWARE_GAMEMODES_H  // Include guard
#define SOFTWARE_GAMEMODES_H

class GameModes {
public:

    void menu1();
    void menu2();
    void menu3();
    void menu4();

    int modus;
    void LED();
    void normal();
    void debug();
    void kickOff();
    void mode4();
private:
    void setLED(int primaryButton);
};

#endif // SOFTWARE_GAMEMODES_H
