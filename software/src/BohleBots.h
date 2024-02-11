//
// Created by Jan Schlegel on 28.01.24.
//

#ifndef SOFTWARE_BOHLEBOTS_H
#define SOFTWARE_BOHLEBOTS_H

//#define AUS 0
//#define GRUEN 1
//#define ROT 2
//#define GELB 3
//#define BLAU 4
//#define CYAN 5
//#define MAGENTA 6
//#define WEISS 7

#define led1r 5
#define led1g 23
#define led1b 16

#define led2r 4
#define led2g 15
#define led2b 13

#include "Pixy2I2C.h"
#include <Wire.h>
#include "elapsedMillis.h"

#define SDA 21
#define SCL 22

#define DRIVE_DIS 27
#define DRIVE1_PWM 18
#define DRIVE1_DIR 19
#define DRIVE2_PWM 14
#define DRIVE2_DIR 12
#define DRIVE3_PWM 25
#define DRIVE3_DIR 26
#define DRIVE4_PWM 32
#define DRIVE4_DIR 33

#define INPUT1 35
#define INPUT2 34
#define INPUT3 39
#define INPUT4 36

#define kicker 17
#define dribbler 2
#define IR_ADDRESS 0x55
#define COMPASS_ADDRESS 0x60 // cmps11, cmps12, cmps14
#define ANGLE_8 1

// COMPASS COMMANDS
const uint8_t COMPASS_LINEAR_ACCELERATION_X_HIGH = 0x0C;
const uint8_t COMPASS_LINEAR_ACCELERATION_Y_HIGH = 0x0E;
const uint8_t COMPASS_LINEAR_ACCELERATION_Z_HIGH = 0x10;

enum COLOR {
    OFF = 0,
    GREEN = 1,
    RED = 2,
    YELLOW = 3,
    BLUE = 4,
    CYAN = 5,
    MAGENTA = 6,
    WHITE = 7
};

struct Acceleration {
    int x;
    int y;
    int z;
};

class BohleBots {
public:

    // ++++++++ Initializer and fundamental functions ++++++++ //

    /************************************************************
     *
     *  BohleBots()
     *
     *  Description:
     *      This Constructor creates a new object
     *      of the BohleBots class.
     *
     ***********************************************************/
    BohleBots();

    /***********************************************************
     *
     *  BohleBots::init()
     *
     *  Description:
     *       Initialises all Sensors and i2csync.
     *
     **********************************************************/
    void init();

    /***********************************************************
     *
     *  BohleBots::sleep(int)
     *
     *  Description:
     *     Sleeps for an amount of ms without interrupting i2c input.
     *
     **********************************************************/
    void sleep(int ms);

    /***********************************************************
     *
     *  BohleBots::setType(int)
     *
     *  Description:
     *     Defines the bot type.
     *
     *  Parameters:
     *     type = 2 => 2 Wheels
     *     type = 3 => 3 Wheels
     *
     **********************************************************/
    void setType(int type);

    /***********************************************************
     *
     *  BohleBots::i2csync()
     *
     *  Description:
     *     Reads out all i2c devices e.g.:
     *
     *     -> Infrared Ring
     *     -> Pixy Cam
     *
     **********************************************************/
    void i2csync();

    /***********************************************************
     *
     *  BohleBots::input(int)
     *
     *  Description:
     *     Returns the analogue value of PIN:
     *
     *  Parameters(nr):
     *
     *  1 => INPUT1
     *  2 => INPUT2
     *  3 => INPUT3
     *  4 => INPUT4
     *
     *  If (nr) is not 1, 2, 3 or 4 return value is 0!
     *
     **********************************************************/
    int input(int nr);

    /***********************************************************
     *
     *  BohleBots::button(int, int)
     *
     *  Description:
     *     Returns input of selected button as a boolean.
     *
     *  Parameters:
     *     (int device):
     *         1 => 1st board
     *         2 => 2nd board
     *
     *     (int button):
     *         1 => left button
     *         2 => right button
     *
     **********************************************************/
    bool button(int device, int button);

    /***********************************************************
     *
     *  BohleBots::boardButton(int)
     *
     *  Description:
     *     Returns input of selected button as a boolean.
     *
     *  Parameters:
     *     (int button):
     *         1 => 1st button
     *         2 => 2st button
     *         3 => 3st button
     *         4 => 4st button
     *
     **********************************************************/
    bool boardButton(int button);

    /***********************************************************
     *
     *  BohleBots::buttonLED(int, int, COLOR)
     *
     *  Description:
     *      Sets the chosen LED to the current state or color.
     *
     *  Parameters:
     *      (int device):
     *          1 => 1st board
     *          2 => 2nd board
     *      (int button):
     *          1 => left button
     *          2 => right button
     *      (COLOR):
     *          OFF = 0,
     *          GREEN = 1,
     *          RED = 2,
     *          YELLOW = 3,
     *          BLUE = 4,
     *          CYAN = 5,
     *          MAGENTA = 6,
     *          WHITE = 7
     *
     *  Disclaimer:
     *      COLOR is an enum. You can either use numbers:
     *
     *      + numbers 1-7
     *      + names written in documentation
     *
     **********************************************************/
    void LED(int device, int LED, int color);

    /***********************************************************
     *
     *  BohleBots::boardLED(int, int)
     *
     *  Description:
     *      Sets the chosen LED to the current state or color.
     *
     *  Parameters:
     *      (int LED):
     *          1 => left button
     *          2 => right button
     *      (color):
     *          OFF = 0,
     *          GREEN = 1,
     *          RED = 2,
     *          YELLOW = 3,
     *          BLUE = 4,
     *          CYAN = 5,
     *          MAGENTA = 6,
     *          WHITE = 7
     *
     *  Disclaimer:
     *      color is also an enum. You can either use numbers:
     *
     *      + numbers 1-7
     *      + names written in documentation
     *
     **********************************************************/
    void boardLED(int LED, int color);


    // +++++++++++++++++ Pixy and compass +++++++++++++++++++ //

    /***********************************************************
     *
     *  BohleBots::readPixy()
     *
     *  Description:
     *      Checks if blocks are found in the picture and
     *      calls evaluatePixy() to locate them.
     *
     **********************************************************/
    void readPixy();

    /***********************************************************
     *
     *  BohleBots::evaluatePixy()
     *
     *  Description:
     *      Analyzes the blocks in the picture and determines:
     *
     *      + _goalHeight
     *      + _goalWidth
     *      + _goalDirection
     *      + _goalDistance
     *
     **********************************************************/
    void evaluatePixy(int signature);

    /***********************************************************
     *
     *  BohleBots::compassHeading()
     *
     *  Description:
     *       Sets the heading to the
     *       current Aim of the compass.
     *
     **********************************************************/
    void compassHeading();

    /***********************************************************
     *
     *  BohleBots::compassOrg()
     *
     *  Description:
     *       Nur Stiebel weiß, was los ist <3.
     *
     **********************************************************/
    int compassOrg();

    /***********************************************************
     *
     *  BohleBots::readCompass()
     *
     *  Description:
     *      Calculates compass heading.
     *
     **********************************************************/
    int readCompass();

    /***********************************************************
     *
     *  BohleBots::compass()
     *
     *  Description:
     *      Returns current compass heading.
     *
     **********************************************************/
    int compass();

    // +++++++++++++ Motors, driving and kicking +++++++++++++ //

    void motor(int number, int speed);

    void drive(int direction, int speed, int rotation);

    int readLinearAcceleration(Acceleration &acceleration);

    float convertToLinearAcceleration(int16_t rawValue);

    // ++++++++++++ Return's of private Variables ++++++++++++ //

    int ballDirection();

    bool seesBall();

    int goalDirection();

    int goalDistance();

    bool seesGoal();

    bool goalAligned();

    bool goalLeft();

    bool goalRight();

    bool isInCorner(int ms, int ballCache, int cornerDirection);

    float x;
    float y;
    float z;

private:

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

    // elapsedMillis
    elapsedMillis coolDown;
    elapsedMillis sleepDuration;
    // Ports and Addresses
    int _tastLedID[8] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
    bool _portena[8] = {false, false, false, false, false, false, false, false};
    bool _taster1Array[8] = {false, false, false, false, false, false, false, false};
    bool _taster2Array[8] = {false, false, false, false, false, false, false, false};
    int _led1Array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int _led2Array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    // Bot
    int _botType;
    int _irData;
    int _ballDirection;
    int _ballDistance;
    bool _seesBall;
    bool _isInCorner;

    int speedToPWm(int speed);

    void drive2(int speed, int rotation);

    void drive3(int direction, int speed, int rotation);

    // Pixy
    Pixy2I2C pixy;
    bool _hasPixy;
    bool _seesGoal;
    int _goalDirection;
    int _goalWidth;
    int _goalHeight;
    int _rawDistance;
    int _goalDistance;
    // Compass
    int _compassData;
    int _compassHeading;
    int _compassEna;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ //

};


#endif //SOFTWARE_BOHLEBOTS_H