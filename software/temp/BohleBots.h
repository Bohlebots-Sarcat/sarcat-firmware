
// -------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
/*
*  BOHLEBOTS HEADER
*
*  Version: 0.1.16
*  Date: 25.01.2024
*
*  Dieser Header ist eine Sammlung und Kapselung der
*  Standard-Funktionalitäten eines RoboCup-Soccer-Bots.
*  Die hier verwendeten Funktionen und Variablen sind aufgearbeitete
*  Erfahrungswerte, die die BohleBots der jahrelang im RoboCup Soccer
*  sammeln konnten.
*
*  Informationen zu den Robotern BohleBots in den letzten Jahren:
*      https://www.bohlebots.de/statisch/doku.html
*      https://github.com/stiebel/bohlebots
*/
//////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------
#ifndef BOHLEBOTS_H
#define BOHLEBOTS_H
//////////////////////////////////////////////////////////////////////
/*
*   INCLUDES / DEPENDENCIES
*/
//////////////////////////////////////////////////////////////////////
//
// Sollte hier ein Compiler-Fehler auftreten, muss die entsprechende
// Bilbliothek installiert werden.
//
#include <Arduino.h>
#include <Wire.h>
#include "elapsedMillis.h"
#include "Pixy2I2C.h"
//////////////////////////////////////////////////////////////////////
/*
*   DEFINES
*/
//////////////////////////////////////////////////////////////////////
//
// I2C Bus
//

#define led1r 5
#define led1g 23
#define led1b 16

#define led2r 4
#define led2g 15
#define led2b 13

#define SDA 21
#define SCL 22
//
// I2C Device-Adressen
//
#define PIXY_ADRESSE    0x54
#define IR_ADRESSE      0x55
#define ANGLE_8            1
//
// Motor PORTS
//
#define DRIVE_DIS 27
#define DRIVE1_PWM 18
#define DRIVE1_DIR 19
#define DRIVE2_PWM 14
#define DRIVE2_DIR 12
#define DRIVE3_PWM 25
#define DRIVE3_DIR 26
#define DRIVE4_PWM 32
#define DRIVE4_DIR 33
//
// Analog PINs
//
#define INPUT1 35
#define INPUT2 34
#define INPUT3 39
#define INPUT4 36
//
// Digitale PORTS
//
#define kicker   15
#define dribbler 23
//////////////////////////////////////////////////////////////////////
/*
*   DEKLARATIONEN
*/
//////////////////////////////////////////////////////////////////////
//
// Zuweisung: Farbwert - Name / Numerierung der Farben
//
enum FARBE {
  AUS = 0,
  GRUEN,
  ROT,
  GELB,
  BLAU,
  CYAN,
  MAGENTA,
  WEISS
};
//
// BohleBots Klasse
//
class BohleBots {
  public:
    /************************************************************
    * Alles ab hier ist public
    * also von ausserhalb dieser Klasse erreichbar
    ************************************************************/

    //
    // elapsedMillis zur FREIEN VERWENDUNG!
    //
    // Idee: Kurz Schub aufzunehmen, bevor gekickt wird
    elapsedMillis strikeZeit;
    // Idee: Tracken wie lange das Tor nicht gesehen wurde
    //       und parallel immer die letzte torRichtung speichern
    elapsedMillis blindZeit;
    // Idee: Cool-Down-Timer fuer die Taster-Auslesung,
    //       da loop viel schneller als menschlisches Druecken
    elapsedMillis  tastZeit;
    //
    // Jeder Roboter besitzt auf dem InfraRot-Ring 2 steckbare Module
    // Diese muessen durch festes verloeten eine Adresse bekommen,
    // welche in diese beiden Werte gesetzt werden muessen
    //
    //////////////////////////////////////
    /*
    *   SETUP-FUNKTIONEN
    */
    //////////////////////////////////////
    //
    // Diese Funktionen sollten bei Verwendung
    // dieses Headers einmalig am Anfang des Programmes in einer
    // bestimmten Reihenfolge aufgerufen werden!
    //
    /************************************************************
    *
    * [1. Aufruf]
    *   BohleBots()
    *
    *   Beschreibung:
    *     der Konstruktor erzeugt ein neues Objekt
    *     (bzw. eine neue Instanz) der BohleBots Klasse
    *
    ************************************************************/
    BohleBots();
    /************************************************************
    *
    * [2. - 5. Aufruf]
    *   BohleBots::setType(int)
    *
    *   Beschreibung:
    *     setzt die Anzahl der anzutreibenen Raeder
    *
    *   Parameter:
    *     anzahlRaeder: Anzahl der Raeder die angetrieben
    *                   werden muessen um zu fahren
    *
    *   Achtung:
    *     Dieser Header akzeptiert nur 2, 3 oder 4 Raeder!
    *     anzahlRaeder < 2 => intern: anzahlRaeder = 2
    *     anzahlRaeder > 4 => intern: anzahlRaeder = 4
    *
    ************************************************************/
    void setType(int anzahlRaeder);
    /************************************************************
    *
    * [2. - 5. Aufruf]
    *   BohleBots::setLichtPin(int)
    *
    *   Beschreibung:
    *     setzt an welcher der 4 analogen Inputs die Lichtschranke
    *     angeschlossen ist
    *
    *   Parameter:
    *     nummer: 1 => INPUT1,
    *             2 => INPUT2,
    *             3 => INPUT3,
    *             4 => INPUT4
    *
    *   Achtung:
    *     nummer < 1 oder nummer > 4
    *       => hatBall() gibt immer false zurueck
    *
    ************************************************************/
    void setLichtPin(int nummer);
    /************************************************************
    *
    * [2. - 5. Aufruf]
    *   BohleBots::setBallSchwelle(unsigned int)
    *
    *   Beschreibung:
    *     setzt die Schwelle des analogen Wertes
    *     fuer die Lichtschranke
    *
    *   Parameter:
    *     schwelle: Analogwert der als Schwelle
    *               gesetzt werden soll
    *
    *   Achtung:
    *     Lichtschrankenwert > Schwelle => Ball in Schale
    *     Lichtschrankenwert < Schwelle => Ball nicht in Schale
    *
    ************************************************************/
    void setBallSchwelle(unsigned int schwelle);
    /************************************************************
    *
    * [2. - 5. Aufruf]
    *   BohleBots::usePixy(bool)
    *
    *   Beschreibung:
    *     setzt ob eine Pixy2 verwendet wird oder nicht
    *
    *   Parameter:
    *     benutzt: true / 1, wenn eine Pixy benutzt wird
    *              false / 0 wenn nicht
    *
    *   Achtung:
    *     Falls falsch gesetzt haengt sich der I2C Bus auf
    *     und der Roboter tut nichts!
    *
    ************************************************************/
    void usePixy(bool benutzt);
    /************************************************************
    *
    * [2. - 5. Aufruf]
    *   BohleBots::setBenutztKompass(bool)
    *
    *   Beschreibung:
    *     setzt ob eine Kompasssensor (CMPS11, CMPS12 oder CMPS14)
    *     verwendet wird oder nicht
    *
    *   Parameter:
    *     benutzt: true / 1, wenn einen Kompass benutzt wird
    *              false / 0 wenn nicht
    *
    *   Achtung:
    *     Falls falsch gesetzt haengt sich der I2C Bus auf
    *     und der Roboter tut nichts!
    *
    ************************************************************/
    void setBenutztKompass(bool benutzt);
    /************************************************************
    *
    * [6. Aufruf]
    *   BohleBots::init()
    *
    *   Beschreibung:
    *     initialisiert des Roboter-Objekt,
    *     bereitet also alle PINs, PORTs, und Verbindungen vor
    *
    ************************************************************/
    void init();
    //////////////////////////////////////
    /*
    *   LOOP-FUNKTIONEN
    */
    //////////////////////////////////////
    //
    // Diese Funktionen koennen jederzeit aufgerufen werden!
    //
    /************************************************************
    *
    * [!MUST HAVE IN JEDEM LOOP-DURCHLAUF!]
    *   BohleBots::warte(int)
    *
    *   Beschreibung:
    *     laesst den Roboter eine gewisse Zeit warten
    *
    *    Paramter:
    *     zeit: Anzahl der Millisekunden,
    *           die der Roboter warten soll
    *
    *     Achtung:
    *       Im Gegensatz zu einem delay legt diese Funktion
    *       nicht den gesamten Roboter lahm!
    *       Alle 10 ms werden die aktuellen Sensordaten gelesen!
    *
    ************************************************************/
    void warte(int zeit);
    /************************************************************
    *
    *   BohleBots::drive(int, int, int)
    *
    *   Beschreibung:
    *     laesst den Roboter in eine bestimmte Richtung,
    *     mit einem bestimmten Tempo fahren und sich dabei
    *     gleichzeitig um die eigene Achse drehen
    *
    *    Paramter:
    *     richtung: Richtung in die gefahren werden soll
    *       BotTyp:       3         |       4
    *       ------------------------+-----------------
    *          -3 :  xxxxxxxxxxx    |  135° links
    *          -2 :   120° links    |   90° links
    *          -1 :    60° links    |   45° links
    *           0 :    0° / vorne   |    0° / vorne
    *           1 :    60° rechts   |   45° rechts
    *           2 :   120° rechts   |   90° rechts
    *           3 :   180° / hinten |  135° rechts
    *           4 :  xxxxxxxxxxx    |  180° / hinten
    *     tempo:   Geschwindigkeit des Roboters in Prozent
    *     drehung: Drehgeschwingikeit des Roboters in Prozent
    *
    *     Achtung:
    *       tempo + drehung kleiner als 100 halten!
    *       Wenn richtung einen Wert besitzt der nicht
    *       in der Tabelle steht, wird kein neuer Befehl
    *       an die Motoren gesendet!
    *       Eine Implementierung mit BotTyp 2 existiert nicht!
    *
    ************************************************************/
    void drive(int richtung, int tempo, int drehung);
    /************************************************************
    *
    *   BohleBots::ballFahre()
    *
    *   Beschreibung:
    *      Wandelt die ball Richtung aus der Funktion ballRichtung()
    *      in die Richtung die für die Funktion fahre() benötigt wird
    *
    ************************************************************/
    int ballFahre(int type, bool ERROR);
    /************************************************************
    *
    *   BohleBots::ballFahre()
    *
    *   Beschreibung:
    *      Wandelt die ball Richtung aus der Funktion ballRichtung()
    *      in die Richtung die für die Funktion fahre() benötigt wird
    *
    ************************************************************/
    int ballFahre_3();
    /************************************************************
    *
    *   BohleBots::ballFahre()
    *
    *   Beschreibung:
    *      Wandelt die ball Richtung aus der Funktion ballRichtung()
    *      in die Richtung die für die Funktion fahre() benötigt wird
    *
    ************************************************************/
    int ballFahre_4();
    /************************************************************
    *
    *   BohleBots::siehtBall()
    *
    *   Beschreibung:
    *      gibt zurueck ob der InfraRot-Ring den Ball sieht
    *
    ************************************************************/
    bool siehtBall();
    /************************************************************
    *
    *   BohleBots::ballRichtung()
    *
    *   Beschreibung:
    *      gibt die BallRichtung vom InfraRot-Ring zurueck
    *      hierbei gilt:
    *       -7 => 157,5° links
    *       -6 => 135°   links
    *       -5 => 115,5° links
    *       -4 =>  90°   links
    *       -3 =>  67,5° links
    *       -2 =>  45°   links
    *       -1 =>  22,5° links
    *        0 =>   0° / vorne
    *        1 =>  22,5° rechts
    *        2 =>  45°   rechts
    *        3 =>  67,5° rechts
    *        4 =>  90°   rechts
    *        5 => 115,5° rechts
    *        6 => 135°   rechts
    *        7 => 157,5° rechts
    *        8 => 180° / hinten
    *
    *     Achtung:
    *       Wenn kein Ball gesehen wird,
    *       ist der Rueckgabewert -7!
    *
    ************************************************************/
    int ballRichtung();
    /************************************************************
    *
    *   BohleBots::hatBall()
    *
    *   Beschreibung:
    *      gibt zurueck ob der Ball in der Lichtschranke ist,
    *      indem der Analogwert der Lichtschranke mit der
    *      BallSchwelle verglichen wird
    *
    ************************************************************/
    bool hatBall();
    /************************************************************
    *
    *   BohleBots::kompassRichtung()
    *
    *   Beschreibung:
    *     gibt die Richtung des Kompasssensors
    *     von -180° bis +180° zurueck
    *
    *     Achtung:
    *       Wenn kein Kompass benutzt wird,
    *       ist der Ruckgabewert 0;
    *
    ************************************************************/
    int kompassRichtung();
    /************************************************************
    *
    *   BohleBots::setKompassHeading()
    *
    *   Beschreibung:
    *     setzt die Richtung von 0° / vorne
    *     auf die aktuelle Richtung
    *
    *     Achtung:
    *       Wenn kein Kompass benutzt wird,
    *       tut diese Funktion nichts!
    *
    ************************************************************/
    void setKompassHeading();
    /************************************************************
    *
    *   BohleBots::siehtTor()
    *
    *   Beschreibung:
    *      gibt zurueck ob die Pixy gerade etwas als Tor erkennt
    *
    *     Achtung:
    *       Wenn keine Pixy benutzt wird,
    *       ist der Rueckgabewert false;
    *
    ************************************************************/
    bool siehtTor();
    /************************************************************
    *
    *   BohleBots::torRichtung()
    *
    *   Beschreibung:
    *      gibt die Richtung des Tores per Pixy in Pixeln zurueck
    *      hierbei wird die Mitte der erkannten Signatur genommen
    *      und der x-Wert minus die halbe Bildbreite gerechnet
    *
    *     Achtung:
    *       Wenn keine weder Kompass noch Pixy benutzt werden,
    *       ist der Rueckgabewert 0!
    *       Wenn keine Pixy, aber ein Kompass benutzt wird,
    *       ist der Rueckgabewert der der Kompassrichtung!
    *
    ************************************************************/
    int torRichtung();
    /************************************************************
    *
    *   BohleBots::torEntfernung()
    *
    *   Beschreibung:
    *      gibt die Entfernung des Tores per Pixy zurueck
    *      hierbei wird von der Oberkannte des Tores die Hoehe
    *      abgezogen und das Ergebnis durch 4 geteilt
    *
    *     Achtung:
    *       Diese Funktion ist ein Relikt von aus dem Jahr 2017
    *       Team Bohlebots Quadro, daher keine Garantie auf
    *       universelle Sinnhaftigkeit!
    *
    ************************************************************/
    int torEntfernung();
    /************************************************************
    *
    *   BohleBots::input(int)
    *
    *   Beschreibung:
    *     gibt den Analogwert des PINs an
    *     INPUT1, INPUT2, INPUT3 oder INPUT4 zurueck
    *
    *    Paramter:
    *     nummer: 1 => INPUT1
    *             2 => INPUT2
    *             3 => INPUT3
    *             4 => INPUT4
    *
    *     Achtung:
    *       Wenn nummer nicht 1, 2, 3 oder 4 ist,
    *       ist der Rueckgabewert 0!
    *
    ************************************************************/
    int input(int nummer);
    /************************************************************
    *
    *   BohleBots::taster(int)
    *
    *   Beschreibung:
    *      gibt zurück ob Taster 1-4 gedrückt worden sind
    *
    *    Paramter:
    *     nummer: 1 => 1. Taster links beginnend
    *     nummer: 2 => 2. Taster links beginnend
    *     nummer: 3 => 3. Taster links beginnend
    *     nummer: 4 => 4. Taster links beginnend     *
    *
    *     Achtung:
    *       Wenn nr außerhalb des Spektrums ist der rückgabewert false!
    *
    ************************************************************/
    bool boardTaster(int nr);
    /************************************************************
    *
    *   BohleBots::setLed(int,int,FARBE)
    *
    *   Beschreibung:
    *      setzt eine LED auf einem Device und einer nummer
    *      auf eine bestimmte Farbee
    *
    *    Paramter:
    *     device: Adresse die auf den Taster-Modulen als
    *             Binaerzahl eingeloetet wurde
    *     nummer: 1 => links oder 2 => rechts
    *     farbe:  AUS, GRUEN, ROT, GELB, BLAU, CYAN,
    *             MAGENTA oder WEISS
    *
    *     Achtung:
    *       Wenn device < 0, device > 7 ist,
    *       oder nummer weder 1 noch 2 ist,
    *       passiert nichts!
    *       Die LED behaelt solange ihre Farbe,
    *       bis diese ueberschrieben oder auf AUS gesetzt wird!
    *
    ************************************************************/
    void setLed(int device, int nummer, int farbe);
    /************************************************************
    *
    *   BohleBots::kick(int)
    *
    *   Beschreibung:
    *      setzt den Kicker PORT fuer eine bestimmte Zeit HIGH,
    *      sodass der Roboter kickt
    *
    *    Paramter:
    *     zeit: Anzahl der Millisekunden, die der PORT HIGH
    *           gesetzt wird
    *
    *     Achtung:
    *       Je hoeher zeit, desto haerter der Kick,
    *       aber um brennende Roboter zu verhindern,
    *       wird der Kick auf maximal 40 Millisekunden begrenzt!
    *       Diese Funktion beinhaltet das einzige delay,
    *       dass im gesamten Programm verwendet werden sollte!
    *
    ************************************************************/
    void kick(int zeit);
    /************************************************************
    *
    *   BohleBots::boardled(int, int)
    *
    *   Beschreibung:
    *      setzt die jeweilige LED 1-4 auf farbe
    ************************************************************/
    void boardLED(int nr, FARBE farbe);

    bool _ballVorhanden = false;
    int  _ballRichtung  = 0;
    int _ballDistanz = 0;
    bool _hatBall       = false;

    int _irpaket;

    //////////////////////////////////////////////////////////////////////////////
  // ---------------------------------------------------------------------------
  // ENDE DESSEN, WAS DER USER DIESES HEADERS WISSEN MUSS!
  // Ab hier folgen private Variablen und Funktionen.
  // ---------------------------------------------------------------------------
  //////////////////////////////////////////////////////////////////////////////
  private:
    //
    // Ports, Adressen und Zustaende
    //
    int  _tastLedID[8]    = {  0x20,  0x21,   0x22,  0x23,  0x24,  0x25,  0x26, 0x27 };
    bool _portena[8]      = { false, false, false, false, false, false, false, false };
    bool _taster1Array[8] = { false, false, false, false, false, false, false, false };
    bool _taster2Array[8] = { false, false, false, false, false, false, false, false };
    int  _led1Array[8]    = {     0,     0,     0,     0,     0,     0,     0,     0 };
    int  _led2Array[8]    = {     0,     0,     0,     0,     0,     0,     0,     0 };
    //
    // Flags und Variablen zur Zwischenspeicherung
    //
    // BotTyp
    // Ball
    int  _lichtPin      = 0;
    unsigned int _ballSchwelle = 500;
    // Kompass
    bool _benutztKompass = false;
    int  _head           = 0;
    int  _kompassWert    = 0;
    // Pixy
    Pixy2I2C pixy;
    bool _benutztPixy   = true;
    bool _siehtTor      = false;
    int  _torRichtung   = 0;
    int  _torEntfernung = 0;
    // IR
    // Zeiten
    elapsedMillis totZeit;    // CoolDown-Timer, damit der Kicker nicht ueberhitzt
    elapsedMillis warteZeit;  // Timer fuer die warte()-Funktion
    //
    // Funktionen
    //
    // I2C
    void i2csync();
    void statusBlink(FARBE farbe);
    // Pixy
    void pixy_auswerten();
    void pixy_read();
    // Motoren
    void motor(int number, int speed);
    int spdToPWM(int speed);
    void fahre3(int richtung, int tempo, int drehung);
};
#endif // BOHLEBOTS_H