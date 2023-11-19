#include "BohleBots.h"

//////////////////////////////////////////////////////////////////////
/*
*   IMPLEMENTIERUNGEN
*/
//////////////////////////////////////////////////////////////////////
BohleBots::BohleBots() {
  pinMode(kicker, OUTPUT);
  digitalWrite(kicker, LOW);
  Serial.begin(115200);
  delay(200);
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
}



void BohleBots::init() {
  digitalWrite(DRIVE_DIS, HIGH);
  delay(100);
  totZeit = 1000;
  for (int lauf = 0; lauf < 8; lauf++) {
    Wire.beginTransmission(_tastLedID[lauf]);
    byte error = Wire.endTransmission();
    if (error == 0) _portena[lauf] = true;
    Serial.print("LED_Tast : " + String(lauf) + " : ");
    if (error == 0) Serial.println("true");
    else Serial.println("false");
  }
  statusblink(ROT);
  delay(300);
  statusblink(AUS);
  delay(100);
  statusblink(GELB);
  if (_benutztKompass) {
    Wire.beginTransmission(KOMPASS_ADRESSE);
    byte error = Wire.endTransmission();
    if (error == 0) Serial.println("Kompass true");
    else {
      Serial.println("Kompass false");
      delay(1000);
    }
    delay(300);
    statusblink(AUS);
    delay(100);
  }
  statusblink(BLAU);
  if (_benutztPixy) {
    Serial.print("Warte auf Pixy2 auf i2c 0x54...");
    pixy.init(PIXY_ADRESSE);
    Serial.println("done");
  }
  delay(1);
  statusblink(AUS);
  statusblink(GRUEN);
  delay(800);
  statusblink(AUS);
  digitalWrite(DRIVE_DIS, LOW);
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
    if (_botTyp == 4) fahre4(richtung, tempo, drehung);
  if (_botTyp == 3) fahre3(richtung, tempo, drehung);
}



void BohleBots::setBotTyp(int t) {
  if (t < 4) _botTyp = 3;
  if (t > 3) _botTyp = 4;
}

void BohleBots::setLichtPin(int nummer) {
  if(nummer < 0) nummer = 0;
  _lichtPin = nummer;
}

void BohleBots::setBallSchwelle(unsigned int schwelle) {
  _ballSchwelle = schwelle;
}


  
void BohleBots::setBenutztPixy(boolean benutzt) {
  _benutztPixy = benutzt;
}



void BohleBots::setBenutztKompass(boolean benutzt) {
  _benutztKompass = benutzt;
}



int BohleBots::input(int number) {
  switch(number) {
    case 1:  return (analogRead(INPUT1));
    case 2:  return (analogRead(INPUT2));
    case 3:  return (analogRead(INPUT3));
    case 4:  return (analogRead(INPUT4));
    default: return 0;
  }
}



int BohleBots::kompassRichtung() {
  return _kompassWert;
}



void BohleBots::setKompassHeading() {
  Wire.beginTransmission(KOMPASS_ADRESSE);
  byte error = Wire.endTransmission();
  if (error == 0) {
    _head = kompass_org();
  }
}



bool BohleBots::taster(int device, int nr) {  // liefert von device Taster nr (1:links, 2:rechts)
  if (device < 0) return false;
  if (device > 7) return false;
  //_portena[device] = true;
  if (nr == 1) return _taster1Array[device];
  if (nr == 2) return _taster2Array[device];
  return false;
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



void BohleBots::setLed(int device, int nr, FARBE farbe) {  // setzt device led nr (1:links, 2:rechts) auf farbe
  //if (farbe < 0) return;
  //if (farbe > 7) return;
  if (device < 0) return;
  if (device > 7) return;
  //_portena[device] = true;
  int farbe_int = (int)farbe;
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



int BohleBots::spdToPWM(int speed) {
  if (speed < 0) speed *= -1;
  return ((speed * 255) / 100);
}



void BohleBots::motor(int number, int speed) {
  // Speed wird bei 100 und -100 gekappt
  if (speed > 100) speed = 100;
  if (speed < -100) speed = -100;
  int pwm = spdToPWM(speed);
  int dir;
  if (speed < 0) dir = LOW;
  else dir = HIGH;
  if (number == 1) digitalWrite(DRIVE1_DIR, dir);
  if (number == 2) digitalWrite(DRIVE2_DIR, dir);
  if (number == 3) digitalWrite(DRIVE3_DIR, dir);
  if (number == 4) digitalWrite(DRIVE4_DIR, dir);
  if ((number > 0) && (number < 5)) ledcWrite(number, pwm);
}



int BohleBots::kompass_org() {
  unsigned char high_byte, low_byte, angle8;
  unsigned int angle16;
  Wire.beginTransmission(KOMPASS_ADRESSE);
  Wire.write(ANGLE_8);
  Wire.endTransmission();
  Wire.requestFrom(KOMPASS_ADRESSE, 3);
  while (Wire.available() < 3)
    ;
  angle8 = Wire.read();  // Read back the 5 bytes
  high_byte = Wire.read();
  low_byte = Wire.read();
  angle16 = high_byte;  // Calculate 16 bit angle
  angle16 <<= 8;
  angle16 += low_byte;
  return angle16 / 10;
}



int BohleBots::kompass_lesen() {
  return ((((kompass_org() - _head) + 180 + 360) % 360) - 180);
}



void BohleBots::i2csync() {
  for (int lauf = 0; lauf < 8; lauf++) {
    if (_portena[lauf]) {
      int ledwert = 255 - _led1Array[lauf] - _led2Array[lauf];
      Wire.beginTransmission(_tastLedID[lauf]);
      Wire.write(ledwert);
      ;
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
  }  // ENDE TastLED
  if (_benutztKompass == true) _kompassWert = kompass_lesen();
  else _kompassWert = 0;
  int irpaket = 0;
  Wire.requestFrom(IR_ADRESSE, 1);
  if (Wire.available()) irpaket = Wire.read();

int _zone;
bool _ballVorhanden;
  _ballRichtung   = (irpaket % 16) - 7;
  _zone =  irpaket / 16;
  _ballVorhanden  = (_zone != 0);
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
  if (richtung == 0) { // geradeaus
    motor(1, -tempo + drehung);
    motor(2, +drehung);
    motor(3, tempo + drehung);
  }
  if (richtung == 1) { // 60 Grad rechtss
    motor(1, +drehung);
    motor(2, -tempo + drehung);
    motor(3, tempo + drehung);
  }
  if (richtung == -1) { // -60 Grad links
    motor(1, -tempo + drehung);
    motor(2, tempo + drehung);
    motor(3, +drehung);
  }
  if (richtung == 3) { // zurück
    motor(1, tempo + drehung);
    motor(2, +drehung);
    motor(3, -tempo + drehung);
  }
  if (richtung == -2) { // -120 Grad links
    motor(1, +drehung);
    motor(2, tempo + drehung);
    motor(3, -tempo + drehung);
  }
  if (richtung == 2) { // 120 Grad rechts
    motor(1, tempo + drehung);
    motor(2, -tempo + drehung);
    motor(3, +drehung);
  }
}


void BohleBots::fahre4(int richtung, int tempo, int drehung) {
  int maxs = abs(tempo) + abs(drehung);
  if (maxs > 100) {
    tempo = tempo * 100 / maxs;
    drehung = drehung * 100 / maxs;
  }
  //Serial.println("Richtung: "+String(richtung)+" tempo: "+String(tempo)+" drehung: "+drehung);
  if (richtung == 0) { // geradeaus
    motor(1, -tempo + drehung);
    motor(2, tempo + drehung);
    motor(3, tempo + drehung);
    motor(4, -tempo + drehung);
  }
  if (richtung == 1) { // 45 Grad rechts
    motor(1, drehung);
    motor(2, drehung);
    motor(3, tempo + drehung);
    motor(4, -tempo + drehung);
  }
  if (richtung == 2) { // rechts
    motor(1, tempo + drehung);
    motor(2, -tempo + drehung);
    motor(3, tempo + drehung);
    motor(4, -tempo + drehung);
  }
  if (richtung == 3) { // 135 Grad rechts
    motor(1, tempo + drehung);
    motor(2, -tempo + drehung);
    motor(3, drehung);
    motor(4, drehung);
  }
  if (richtung == 4) { // zurück
    motor(1, tempo + drehung);
    motor(2, -tempo + drehung);
    motor(3, -tempo + drehung);
    motor(4, tempo + drehung);
  }
  if (richtung == -3) { // 135 Grad links
    motor(1, drehung);
    motor(2, drehung);
    motor(3, -tempo + drehung);
    motor(4, tempo + drehung);
  }
  if (richtung == -2) { // links
    motor(1, -tempo + drehung);
    motor(2, tempo + drehung);
    motor(3, -tempo + drehung);
    motor(4, tempo + drehung);
  }
  if (richtung == -1) { // 45 Grad links
    motor(1, -tempo + drehung);
    motor(2, tempo + drehung);
    motor(3, drehung);
    motor(4, drehung);
  }
}

int BohleBots::ballFahre() {
    int richtung = 0;
    switch (ballRichtung()) {
        case -7: richtung = -2;
        case -6: richtung = -2;
        case -5: richtung = -2;
        case -4: richtung = -2;
        case -3: richtung = -1;
        case -2: richtung = -1;
        case -1: richtung = -1;
        case 0: richtung = 0;
        case 1: richtung = 1;
        case 2: richtung = 1;
        case 3: richtung = 1;
        case 4: richtung = 2;
        case 5: richtung = 2;
        case 6: richtung = 2;
        case 7: richtung = 2;
        case 8: richtung = 3;
        default: richtung = 0;
            break;
    }
    return richtung;
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
  if ((_siehtTor == false) && (_benutztKompass)) _torRichtung = kompassRichtung();  //wenn die Kamera nicht funktioniert -> Kompass
}
  


void BohleBots::statusblink(FARBE farbe) {
  setLed(linkesModul,  1, farbe);
  setLed(linkesModul,  2, farbe);
  setLed(rechtesModul, 1, farbe);
  setLed(rechtesModul, 2, farbe);
  i2csync();
}