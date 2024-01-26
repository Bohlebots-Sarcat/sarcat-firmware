#include "BohleBots.h"

//////////////////////////////////////////////////////////////////////
/*
*   IMPLEMENTIERUNGEN
*/
//////////////////////////////////////////////////////////////////////
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
    boardLED(1, AUS);
    boardLED(2, AUS);
}

void BohleBots::init() {
    Serial.print("Warte auf Pixy2 auf i2c 0x54...");
    pixy.init(PIXY_ADRESSE);
    Serial.println("done");
}

void BohleBots::warte(int zeit) {
    warteZeit = 0;
    i2csync();
    if (_benutztPixy) pixy_read();
    else _torRichtung = kompassRichtung();
    while (warteZeit < zeit) {
        if ((warteZeit % 10) == 0) i2csync();
        else delay(1);
    }
}


void BohleBots::fahre(int richtung, int tempo, int drehung) {
    fahre3(richtung, tempo, drehung);
}

void BohleBots::setLichtPin(int nummer) {
    if (nummer < 0) nummer = 0;
    _lichtPin = nummer;
}

void BohleBots::setBallSchwelle(unsigned int schwelle) {
    _ballSchwelle = schwelle;
}


void BohleBots::usePixy(boolean benutzt) {
    _benutztPixy = benutzt;
}


void BohleBots::setBenutztKompass(boolean benutzt) {
    _benutztKompass = benutzt;
}


int BohleBots::input(int number) {
    switch (number) {
        case 1:
            return (analogRead(INPUT1));
        case 2:
            return (analogRead(INPUT2));
        case 3:
            return (analogRead(INPUT3));
        case 4:
            return (analogRead(INPUT4));
        default:
            return 0;
    }
}


int BohleBots::kompassRichtung() {
    return _kompassWert;
}


bool BohleBots::boardTaster(int nr) {
    return (!input(nr));
}


int BohleBots::ballRichtung() {
    return _ballRichtung;
}


bool BohleBots::siehtBall() {
    if (_ballVorhanden > 0) return true;
    return false;
}


bool BohleBots::hatBall() {
    return _hatBall;
}


int BohleBots::torRichtung() {
    return -_torRichtung;
}


int BohleBots::torEntfernung() {
    return _torEntfernung;
}


bool BohleBots::siehtTor() {
    return _siehtTor;
}


void BohleBots::setLed(int device, int nr, int farbe) {  // setzt device led nr (1:links, 2:rechts) auf farbe
    //if (farbe < 0) return;
    //if (farbe > 7) return;
    if (device < 0) return;
    if (device > 7) return;
    //_portena[device] = true;
    int farbe_int = (int) farbe;
    if (nr == 1) {
        farbe_int = farbe_int * 2;
        _led1Array[device] = farbe_int;
    }
    if (nr == 2) {
        farbe_int = farbe_int * 16;
        if (farbe_int > 63) farbe_int = farbe_int + 64;
        _led2Array[device] = farbe_int;
    }
}


void BohleBots::kick(int zeit) {
    if (totZeit < 1000) return;
    if (zeit > 40) zeit = 40;
    digitalWrite(kicker, HIGH);
    delay(zeit);
    digitalWrite(kicker, LOW);
    totZeit = 0;
}

void BohleBots::motor(int number, int speed) {
    // Ensure speed is within valid range [-100, 100]
    speed = constrain(speed, -100, 100);

    // Convert speed to PWM value
    int pwm = spdToPWM(speed);

    // Set direction based on speed sign
    int dir = (speed < 0) ? LOW : HIGH;

    // Set direction for the specified motor number
    if (number == 1) digitalWrite(DRIVE1_DIR, dir);
    else if (number == 2) digitalWrite(DRIVE2_DIR, dir);
    else if (number == 3) digitalWrite(DRIVE3_DIR, dir);
    else if (number == 4) digitalWrite(DRIVE4_DIR, dir);

    // Write PWM value to the specified motor number if valid
    if (number > 0 && number < 5) ledcWrite(number, pwm);
}


int BohleBots::spdToPWM(int speed) {
    if (speed < 0) speed *= -1;
    return ((speed * 255) / 100);
}

void BohleBots::i2csync() {
    for (int lauf = 0; lauf < 8; lauf++) {
        if (_portena[lauf]) {
            int ledWert = 255 - _led1Array[lauf] - _led2Array[lauf];
            Wire.beginTransmission(_tastLedID[lauf]);
            Wire.write(ledWert);;
            Wire.endTransmission();
            Wire.requestFrom(_tastLedID[lauf], 1);
            if (Wire.available()) {
                int tread = 255 - Wire.read();
                tread = tread % 128;
                if (tread > 63) _taster2Array[lauf] = true;
                else _taster2Array[lauf] = false;
                tread = tread % 2;
                if (tread > 0) _taster1Array[lauf] = true;
                else _taster1Array[lauf] = false;
            }
        }
    }
    int irPaket = 0;
    Wire.requestFrom(IR_ADRESSE, 1);
    if (Wire.available()) irPaket = Wire.read();

    int _zone;
    bool _ballVorhanden;
    _ballRichtung = (irPaket % 16) - 7; // -10, da neuer IR Ring von 7 bis 18 und nicht von -7 - 8
    _zone = irPaket / 16;
    _ballVorhanden = (_zone != 0);
    if (!_ballVorhanden) _ballRichtung = 0;


    // Lichtschranke (nicht wirklich i2c...)
    int licht = input(_lichtPin);
    _hatBall = (licht > _ballSchwelle);
}

void BohleBots::fahre3(int richtung, int tempo, int drehung) {
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

void BohleBots::pixy_auswerten() { // wird nur aufgerufen, wenn die Pixy überhaupt etwas sieht
    int my_signature = 1;  // wir spielen immer auf Signatur 1
    int sieht_farbe = pixy.ccc.blocks[0].m_signature;
    if (sieht_farbe == my_signature) {
        _torRichtung = -(pixy.ccc.blocks[0].m_x - 158) / 2;
        //int tor_breite = pixy.ccc.blocks[0].m_width;
        int tor_hoehe = pixy.ccc.blocks[0].m_height;
        //tor_entfernung_roh =  pixy.ccc.blocks[0].m_y-80;
        int tor_entfernung_roh = pixy.ccc.blocks[0].m_y;
        _torEntfernung = (tor_entfernung_roh - tor_hoehe) / 4;  //-abs(tor_richtung)/10;
        if (_torEntfernung < 0) _torEntfernung = 0;
        if (_torEntfernung > 63) _torEntfernung = 63;
        _siehtTor = true;
    }
}

void BohleBots::pixy_read() {
    // grab blocks!
    pixy.ccc.getBlocks();
    _siehtTor = false;
    _torRichtung = 0;
    if (pixy.ccc.numBlocks) {
        pixy_auswerten();
    }
    if ((_siehtTor == false) && (_benutztKompass))
        _torRichtung = kompassRichtung();  //wenn die Kamera nicht funktioniert → Kompass
}

void BohleBots::boardLED(int nr, FARBE farbe) {
    switch (nr) {
        case 1:
            digitalWrite(led1g, !(farbe & 1));
            digitalWrite(led1r, !(farbe & 2));
            digitalWrite(led1b, !(farbe & 4));
            break;
        case 2:
            digitalWrite(led2g, !(farbe & 1));
            digitalWrite(led2r, !(farbe & 2));
            digitalWrite(led2b, !(farbe & 4));
            break;
        default: return;
    }
}

void BohleBots::statusBlink(FARBE farbe) {
    boardLED(1, farbe);
    boardLED(2, farbe);
    boardLED(3, farbe);
    boardLED(4, farbe);
    i2csync();
}