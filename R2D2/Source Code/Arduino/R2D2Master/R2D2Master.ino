// R2D2Master.ino
#include <SoftwareSerial.h>
#include <SendOnlySoftwareSerial.h>

#include "PS2.h"

//---------------------------------------------------------------------------------
// PS2 DECLARATIONS
//---------------------------------------------------------------------------------
PS2 ps2 = PS2();

//---------------------------------------------------------------------------------
// WAVTRIGGER DECLARATIONS
//---------------------------------------------------------------------------------
#define WAV_PIN 6
SendOnlySoftwareSerial wavSerial(WAV_PIN);
byte masterVolume;
bool enteringWavCode;

//---------------------------------------------------------------------------------
// SETUP AND LOOP
//---------------------------------------------------------------------------------
void setup() {
    // set the random seed, which is used for this such as the random audio feature
    randomSeed(analogRead(0));

    // setup the PS2 controller, 9600 baud, pin 8 receive, pin 9 transmit
    ps2.init(9600, 8, 9);

    // setup the motors

    // setup the wav trigger
    wavTriggerSetup();
}

void loop() {
    // Process the PS2 controller
    processPS2();

    // Process the audio
    processWavTrigger();
}

//---------------------------------------------------------------------------------
// PS2 METHODS
//---------------------------------------------------------------------------------
void processPS2() {
    ps2.update();
}

//---------------------------------------------------------------------------------
// WAVTRIGGER METHODS
//---------------------------------------------------------------------------------
void wavTriggerSetup() {
    // wav trigger runs at 57600
    wavSerial.begin(57600);

    // wait for 2 seconds
    delay(2000);

    // turn off the onboard amp
    //wavTrigger.ampPower(false);

    // set the master volume
    masterVolume = 150;
    //wavTrigger.setMasterVolume(masterVolume);

    // set the flag for entering the wav code from the controller
    enteringWavCode = false;
}

void processWavTrigger() {
    // TODO
    
}


