//
// Created by Jan Schlegel on 28.01.24.
//

#include "BohleBots.h"

BohleBots::BohleBots() {
    Serial.begin(115200);
    Wire.begin(SDA, SCL);

    pinMode(DRIVE_DIS, OUTPUT);
    digitalWrite(DRIVE_DIS, LOW);
    pinMode(DRIVE1_DIR, OUTPUT);
    pinMode(DRIVE2_DIR, OUTPUT);
    pinMode(DRIVE3_DIR, OUTPUT);
    pinMode(DRIVE4_DIR, OUTPUT);

    pinMode(INPUT1, INPUT);
    pinMode(INPUT2, INPUT);
    pinMode(INPUT3, INPUT);
    pinMode(INPUT4, INPUT);

    pinMode(kicker, OUTPUT);
    digitalWrite(kicker, LOW);
    pinMode(dribbler, OUTPUT);
    digitalWrite(dribbler, LOW);

    pinMode(led1r, OUTPUT);
    digitalWrite(led1r, HIGH);
    pinMode(led1g, OUTPUT);
    digitalWrite(led1g, HIGH);
    pinMode(led1b, OUTPUT);
    digitalWrite(led1b, HIGH);
    pinMode(led2r, OUTPUT);
    digitalWrite(led2r, HIGH);
    pinMode(led2g, OUTPUT);
    digitalWrite(led2g, HIGH);
    pinMode(led2b, OUTPUT);
    digitalWrite(led2b, HIGH);

    Wire.begin(SDA, SCL);

    ledcAttachPin(DRIVE1_PWM, 1);
    ledcSetup(1, 1000, 8);
    ledcWrite(1, 0);
    ledcAttachPin(DRIVE2_PWM, 2);
    ledcSetup(2, 1000, 8);
    ledcWrite(2, 0);
    ledcAttachPin(DRIVE3_PWM, 3);
    ledcSetup(3, 1000, 8);
    ledcWrite(3, 0);
    ledcAttachPin(DRIVE4_PWM, 4);
    ledcSetup(4, 1000, 8);
    ledcWrite(4, 0);

    boardLED(1, OFF);
    boardLED(2, OFF);
}

void BohleBots::init() {
    for (int i = 0; i < 8; i++) {
        Wire.beginTransmission(_tastLedID[i]);
        byte error = Wire.endTransmission();
        if (error == 0) _portena[i] = true;
        Serial.print("LED_Tast : " + String(i) + " : ");
        if (error == 0) Serial.println("true");
        else Serial.println("false");
    }

    delay(100);
    Wire.beginTransmission(KOMPASS_ADRESSE);
    byte error = Wire.endTransmission();
    if (error == 0) _compassEna = true;
    if (error == 0) Serial.println("Kompass true");
    else Serial.println("Kompass false");
    if (_hasPixy) {
        Serial.print("Warte auf Pixy2 auf i2c 0x54...");
        pixy.init(0x54);
        Serial.println("done");
    }
}

void BohleBots::sleep(int ms) {
    sleepDuration = 0;
    i2csync();
    while (sleepDuration < ms) {
        if ((sleepDuration % 10) == 0)
            i2csync();
        else
            delay(1);
    }
}

void BohleBots::setType(int type) {
    switch (type) {
        case 2: {
            _botType = 2;
            break;
        }
        case 3: {
            _botType = 3;
            break;
        }
        default: Serial.printf("Bot type not in spectrum \n");
    }
}

void BohleBots::i2csync() {
    for (int lauf = 0; lauf < 8; lauf++) {
        if (_portena[lauf]) {
            int ledwert = 255 - _led1Array[lauf] - _led2Array[lauf];
            Wire.beginTransmission(_tastLedID[lauf]);
            Wire.write(ledwert);
            Wire.endTransmission();

            Wire.requestFrom(_tastLedID[lauf], 1);
            if (Wire.available()) {
                int tread = 255 - Wire.read();
                tread = tread % 128;
                if (tread > 63)
                    _taster2Array[lauf] = true;
                else
                    _taster2Array[lauf] = false;
                tread = tread % 2;
                if (tread > 0)
                    _taster1Array[lauf] = true;
                else
                    _taster1Array[lauf] = false;
            }
        }
    } // ENDE TastLED
    _irData = 0;
    Wire.requestFrom(IR_ADDRESS, 1);
    if (Wire.available()) {
        _irData = Wire.read();
    }

    _ballDirection = (_irData % 16) - 7;
    _ballDistance = _irData / 16;
    _seesBall = (_ballDistance != 0);
    if (!_seesBall)
        _ballDirection = 0;

    if (_compassEna == true) {
        _compassData = readCompass();
    }
    if (_hasPixy) {
        readPixy();
    }
}

int BohleBots::input(int nr) {
    if (nr == 1)
        return (analogRead(INPUT1));
    if (nr == 2)
        return (analogRead(INPUT2));
    if (nr == 3)
        return (analogRead(INPUT3));
    if (nr == 4)
        return (analogRead(INPUT4));
}

bool BohleBots::button(int device, int button) {
    if (device < 0)
        return false;
    if (device > 7)
        return false;
    if (button == 1)
        return _taster1Array[device];
    if (button == 2)
        return _taster2Array[device];
    return false;
}

bool BohleBots::boardButton(int button) {
    return (!input(button));
}

void BohleBots::LED(int device, int LED, int color) {
    if (color < 0)
        return;
    if (color > 7)
        return;
    if (device < 0)
        return;
    if (device > 7)
        return;

    // portena[device] = true;

    if (LED == 1) {
        color = color * 2;
        _led1Array[device] = color;
    }
    if (LED == 2) {
        color = color * 16;
        if (color > 63)
            color = color + 64;
        _led2Array[device] = color;
    }
}

void BohleBots::boardLED(int LED, int color) {
    {
        if (color < 0)
            return;
        if (color > 7)
            return;

        if (LED == 1) {
            digitalWrite(led1g, !(color & 1));
            digitalWrite(led1r, !(color & 2));
            digitalWrite(led1b, !(color & 4));
        }
        if (LED == 2) {
            digitalWrite(led2g, !(color & 1));
            digitalWrite(led2r, !(color & 2));
            digitalWrite(led2b, !(color & 4));
        }
    }
}

void BohleBots::readPixy() {
    // grab blocks!
    pixy.ccc.getBlocks();
    _seesGoal=false;
    // If there are detect blocks, print them!
    if (pixy.ccc.numBlocks) {
        evaluatePixy();
        _seesGoal = true;
    } else {
        _goalDirection = compass();
    }
}

void BohleBots::evaluatePixy() {
    int my_signature = 1; // wir spielen immer auf Signatur 1
    int sieht_farbe = pixy.ccc.blocks[0].m_signature;
    if (sieht_farbe == my_signature) {
        _goalDirection = (pixy.ccc.blocks[0].m_x - 158) / 2;
        _goalWidth = pixy.ccc.blocks[0].m_width;
        _goalHeight = pixy.ccc.blocks[0].m_height;
        _rawDistance = pixy.ccc.blocks[0].m_y;
        _goalDistance = (_rawDistance - _goalHeight) / 4;
        if (_goalDistance < 0)
            _goalDistance = 0;
        if (_goalDistance > 63)
            _goalDistance = 63;
    }
}

void BohleBots::compassHeading() {
    Wire.beginTransmission(KOMPASS_ADRESSE);
    byte error = Wire.endTransmission();
    if (error == 0) {
        _compassHeading = compassOrg();
    }
}

int BohleBots::compassOrg() {
    unsigned char high_byte, low_byte, angle8;
    unsigned int angle16;
    Wire.beginTransmission(KOMPASS_ADRESSE);
    Wire.write(ANGLE_8);
    Wire.endTransmission();
    Wire.requestFrom(KOMPASS_ADRESSE, 3);
    while (Wire.available() < 3)
        ;
    angle8 = Wire.read(); // Read back the 5 bytes
    high_byte = Wire.read();
    low_byte = Wire.read();
    angle16 = high_byte; // Calculate 16 bit angle
    angle16 <<= 8;
    angle16 += low_byte;
    return angle16 / 10;
}

int BohleBots::readCompass() {
    return ((((compassOrg() - _compassHeading) + 180 + 360) % 360) - 180);
}

int BohleBots::compass() {
    return _compassData;
}

void BohleBots::motor(int number, int speed) {
    // Speed wird bei 100 und -100 gekappt
    if (speed > 100)
        speed = 100;
    if (speed < -100)
        speed = -100;
    int pwm = speedToPWm(speed);
    int dir;
    if (speed < 0)
        dir = LOW;
    else
        dir = HIGH;

    if (number == 1)
        digitalWrite(DRIVE1_DIR, dir);
    if (number == 2)
        digitalWrite(DRIVE2_DIR, dir);
    if (number == 3)
        digitalWrite(DRIVE3_DIR, dir);
    if (number == 4)
        digitalWrite(DRIVE4_DIR, dir);

    if ((number > 0) && (number < 5))
        ledcWrite(number, pwm);
}

void BohleBots::drive(int direction, int speed, int rotation) {
    switch (_botType) {
        case 2: {
            drive2(speed, rotation);
            break;
        }
        case 3: {
            drive3(direction, speed, rotation);
            break;
        }
        default: Serial.printf("Error while trying to drive!");
    }
}

void BohleBots::drive2(int speed, int rotation) {
    int maxs = abs(speed) + abs(rotation);
    if (maxs > 100) {
        speed = speed * 100 / maxs;
        rotation = rotation * 100 / maxs;
    }
    motor(1, rotation + speed);
    motor(2, rotation - speed);
}

void BohleBots::drive3(int direction, int speed, int rotation) {
    int maxs = abs(speed) + abs(rotation);
    if (maxs > 100) {
        speed = speed * 100 / maxs;
        rotation = rotation * 100 / maxs;
    }
    if (direction == 0) {
        motor(1, -speed + rotation);
        motor(2, +rotation);
        motor(3, speed + rotation);
    } else if (direction == 1) {
        motor(1, -speed + rotation);
        motor(2, speed + rotation);
        motor(3, +rotation);
    } else if (direction == 2) {
        motor(1, +rotation);
        motor(2, speed + rotation);
        motor(3, -speed + rotation);
    } else if (direction == 3) {
        motor(1, speed + rotation);
        motor(2, +rotation);
        motor(3, -speed + rotation);
    } else if (direction == -2) {
        motor(1, speed + rotation);
        motor(2, -speed + rotation);
        motor(3, +rotation);
    } else if (direction == -1) {
        motor(1, +rotation);
        motor(2, -speed + rotation);
        motor(3, speed + rotation);
    } else {
        // Handle the case when 'direction' is not any of the specified values
    }
}

int BohleBots::ballDirection() {
    return _ballDirection;
}

int BohleBots::speedToPWm(int speed) {
    if (speed < 0)
        speed *= -1;
    return ((speed * 255) / 100);
}
