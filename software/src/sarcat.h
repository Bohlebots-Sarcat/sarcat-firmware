// 27. November 2023
// Bohlebots
// header für BohleBots Hauptplatinen

// Einfach in das Verzeichnis mit der .ino Datei legen und benutzen
// die vier Motor-Outputs und die vier 12Bit Analog-Inputs sind vordefiniert
// motor(nr,speed) und input(nr)
// zusätzlich gibt es den Wrapper digit, der das Analoge Signal der Inputs als
// digital-Signal True oder False zurück gibt, True bei closed, False bei open

// Die Serielle Schnittstelle sowie der I2C Bus werden automatisch gestartet

// Interessant sind sicherlich noch  EEPROM, WIFI und BLE

/*
 * Festlegung der Farben der LEDs auf der tastled-Platine
 */
#define AUS 0
#define GRUEN 1
#define ROT 2
#define GELB 3
#define BLAU 4
#define CYAN 5
#define MAGENTA 6
#define WEISS 7

#define led1r 5
#define led1g 23
#define led1b 16

#define led2r 4
#define led2g 15
#define led2b 13

#include <Wire.h>
#include <elapsedMillis.h>
#include "Pixy2I2C.h"

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

//#define io1 17
//#define io2 2

#define kicker 17
#define dribbler 2
#define IR_ADDRESS 0x55
#define PIXY_ADRESSE    0x54
#define KOMPASS_ADRESSE 0x60 // cmps11, cmps12, cmps14
#define ANGLE_8 1



int kompass_wert;
int head;
bool kompass_ena = false;

int bot_type = 3;

bool portena[] = {false, false, false, false, false, false, false, false};
int tastLedID[] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
bool taster1Array[] = {false, false, false, false, false, false, false, false};
bool taster2Array[] = {false, false, false, false, false, false, false, false};

int led1Array[] = {0, 0, 0, 0, 0, 0, 0, 0};
int led2Array[] = {0, 0, 0, 0, 0, 0, 0, 0};

elapsedMillis totzeit;
elapsedMillis wartezeit;
Pixy2I2C pixy;
int irpaket;
class BohleBots {
public:
    int ballDirection = 0;
    int ballDistance = 0;
    bool ballExists = false;

    BohleBots() {
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
        boardled(1, AUS);
        boardled(2, AUS);
    }

    void init() {
        Serial.print("Warte auf Pixy2 auf i2c 0x54...");
        pixy.init(PIXY_ADRESSE);
        Serial.println("done");
    }

    void warte(int zeit) {
        wartezeit = 0;
        pixy_read();
        i2csync();
        while (wartezeit < zeit) {
            if ((wartezeit % 10) == 0)
                i2csync();
            else
                delay(1);
        }
    }

    void motor(int number, int speed) {
        // Speed wird bei 100 und -100 gekappt
        if (speed > 100)
            speed = 100;
        if (speed < -100)
            speed = -100;
        int pwm = spdToPWM(speed);
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

    void fahre(int richtung, int tempo, int drehung) {
        int maxs = abs(tempo) + abs(drehung);
        if (maxs > 100) {
            tempo = tempo * 100 / maxs;
            drehung = drehung * 100 / maxs;
        }
        switch (richtung) {
            case 0: // geradeaus
                motor(1, -tempo + drehung);
                motor(2, +drehung);
                motor(3, tempo + drehung);
                break;
            case 1: // 60 Grad rechts
                motor(1, -tempo + drehung);
                motor(2, tempo + drehung);
                motor(3, +drehung);
                break;
            case 2: // 120 Grad rechts
            case 3:
            case 4:
                motor(1, +drehung);
                motor(2, tempo + drehung);
                motor(3, -tempo + drehung);
                break;
            case 5: // zurück
            case 6:
            case 7:
            case -7:
            case -6:
            case -5:
                motor(1, tempo + drehung);
                motor(2, +drehung);
                motor(3, -tempo + drehung);
                break;
            case -4: // -120 Grad links
            case -3:
            case -2:
                motor(1, tempo + drehung);
                motor(2, -tempo + drehung);
                motor(3, +drehung);
                break;
            case -1: // -60 Grad links
                motor(1, +drehung);
                motor(2, -tempo + drehung);
                motor(3, tempo + drehung);
                break;
            default:
                // Handle the case when 'richtung' is not any of the specified values
                break;
        }
    }

    void set_bot_type(int t) {
        if (t < 2)
            t = 2;
        if (t > 3)
            t = 3;
    }

    void ena(bool isena) { digitalWrite(DRIVE_DIS, !isena); }

    int input(int number) {
        if (number == 1)
            return (analogRead(INPUT1));
        if (number == 2)
            return (analogRead(INPUT2));
        if (number == 3)
            return (analogRead(INPUT3));
        if (number == 4)
            return (analogRead(INPUT4));
    }

    bool taster(int device,
                int nr) // liefert von device Taster nr (1:links, 2:rechts)
    {
        if (device < 0)
            return false;
        if (device > 7)
            return false;
        // portena[device] = true;
        if (nr == 1)
            return taster1Array[device];
        if (nr == 2)
            return taster2Array[device];
        return false;
    }

    void led(int device, int nr,
             int farbe) // setzt device led nr (1:links, 2:rechts) auf farbe

    {
        if (farbe < 0)
            return;
        if (farbe > 7)
            return;
        if (device < 0)
            return;
        if (device > 7)
            return;

        // portena[device] = true;

        if (nr == 1) {
            farbe = farbe * 2;
            led1Array[device] = farbe;
        }
        if (nr == 2) {
            farbe = farbe * 16;
            if (farbe > 63)
                farbe = farbe + 64;
            led2Array[device] = farbe;
        }
    }

    void boardled(int nr,
                  int farbe) // setzt device led nr (1:links, 2:rechts) auf farbe

    {
        if (farbe < 0)
            return;
        if (farbe > 7)
            return;

        if (nr == 1) {
            digitalWrite(led1g, !(farbe & 1));
            digitalWrite(led1r, !(farbe & 2));
            digitalWrite(led1b, !(farbe & 4));
        }
        if (nr == 2) {
            digitalWrite(led2g, !(farbe & 1));
            digitalWrite(led2r, !(farbe & 2));
            digitalWrite(led2b, !(farbe & 4));
        }
    }

    bool boardtast(int nr) { return (!input(nr)); }

    void kick(int zeit) {
        if (totzeit < 1000)
            return;
        if (zeit > 40)
            zeit = 40;
        digitalWrite(kicker, HIGH);
        delay(zeit);
        digitalWrite(kicker, LOW);
        totzeit = 0;
    }

    bool digit(int number) {
        if (number == 1)
            return (analogRead(INPUT1) < 2048);
        if (number == 2)
            return (analogRead(INPUT2) < 2048);
        if (number == 3)
            return (analogRead(INPUT3) < 2048);
        if (number == 4)
            return (analogRead(INPUT4) < 2048);
    }

    int goalDirection() {
        return torRichtung;
    }
    bool goalExists() {
        return siehtTor;
    }

    void pixyStatus() {
        Serial.print("Warte auf Pixy2 auf i2c 0x54...");
        pixy.init(PIXY_ADRESSE);
        Serial.println("done");
    }

private:
    int spdToPWM(int speed) {
        if (speed < 0)
            speed *= -1;
        return ((speed * 255) / 100);
    }

    void i2csync() {
        for (int lauf = 0; lauf < 8; lauf++) {
            if (portena[lauf]) {
                int ledwert = 255 - led1Array[lauf] - led2Array[lauf];
                Wire.beginTransmission(tastLedID[lauf]);
                Wire.write(ledwert);;
                Wire.endTransmission();

                Wire.requestFrom(tastLedID[lauf], 1);
                if (Wire.available()) {
                    int tread = 255 - Wire.read();
                    tread = tread % 128;
                    if (tread > 63)
                        taster2Array[lauf] = true;
                    else
                        taster2Array[lauf] = false;
                    tread = tread % 2;
                    if (tread > 0)
                        taster1Array[lauf] = true;
                    else
                        taster1Array[lauf] = false;
                }
            }
        } // ENDE TastLED
        irpaket = 0;
        Wire.requestFrom(IR_ADDRESS, 1);
        if (Wire.available()) {
            irpaket = Wire.read();
        }

        ballDirection = (irpaket % 16) - 7;
        ballDistance = irpaket / 16;
        ballExists = (ballDistance != 0);
        if (!ballExists) ballDirection = 0;
    }

    void pixy_auswerten() { // wird nur aufgerufen, wenn die Pixy überhaupt etwas sieht
        int my_signature = 1;  // wir spielen immer auf Signatur 1
        int sieht_farbe = pixy.ccc.blocks[0].m_signature;
        if (sieht_farbe == my_signature) {
            torRichtung = -(pixy.ccc.blocks[0].m_x - 158) / 2;
            torHoehe = pixy.ccc.blocks[0].m_height;
            int tor_entfernung_roh = pixy.ccc.blocks[0].m_y;
            int _torEntfernung = (tor_entfernung_roh - torHoehe) / 4;  //-abs(tor_richtung)/10;
            if (_torEntfernung < 0) _torEntfernung = 0;
            if (_torEntfernung > 63) _torEntfernung = 63;
            siehtTor = true;
        }
    }

    void pixy_read() {
        // grab blocks!
        pixy.ccc.getBlocks();
        siehtTor = false;
        torRichtung = 0;
        if (pixy.ccc.numBlocks) {
            pixy_auswerten();
        }
        torRichtung  = 0;
    }

    int torRichtung;
    int torHoehe;
    bool siehtTor;
};