// R2D2Master.ino
#include <SoftwareSerial.h>
#include <SendOnlySoftwareSerial.h>

#include "PS2.h"
#include "WavTrigger.h"

//---------------------------------------------------------------------------------
// PS2 DECLARATIONS
//---------------------------------------------------------------------------------
PS2 ps2 = PS2();

//---------------------------------------------------------------------------------
// WAVTRIGGER DECLARATIONS
//---------------------------------------------------------------------------------
#define WAV_PIN 6
#define WAV_RANDOM_MILLISECONDS 20000
#define WAV_STARTUP 138
#define WAV_NOTOUCH 139
SendOnlySoftwareSerial wavSerial(WAV_PIN);
WavTrigger wavTrigger(&wavSerial);
byte masterVolume;
bool enteringWavCode;
bool isWavRandomOn;
unsigned long wavLastTimeCheck;
byte wavCode;

//---------------------------------------------------------------------------------
// MOTOR DECLARATIONS
//---------------------------------------------------------------------------------
#define MOTOR_PIN 7
SendOnlySoftwareSerial motorSerial(MOTOR_PIN);

//---------------------------------------------------------------------------------
// SETUP AND LOOP
//---------------------------------------------------------------------------------
void setup() {
    // set the random seed, which is used for this such as the random audio feature
    randomSeed(analogRead(0));

    // setup the PS2 controller, 9600 baud, pin 8 receive, pin 9 transmit
    ps2.init(9600, 8, 9);

    // setup the motors
    motorSetup();

    // setup the wav trigger
    wavTriggerSetup();

    // play the startup sound
    wavCode = WAV_STARTUP;
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
    wavTrigger.ampPower(false);

    // set the master volume
    masterVolume = 150;
    wavTrigger.setMasterVolume(masterVolume);

    // set the flag for entering the wav code from the controller
    enteringWavCode = false;
}

void processWavTrigger()
{
    // play a random sound if we should
    if (isWavRandomOn) {
        // check if it is time to play
        unsigned long timeNow = millis();

        if (timeNow - wavLastTimeCheck > WAV_RANDOM_MILLISECONDS) {
            // time to play
            wavLastTimeCheck = timeNow;

            // generate a random number
            int trackToPlay = random(40) + 13;

            // play it
            wavTrigger.trackPlaySolo(trackToPlay);
        }
    }

    if (ps2.isButtonJustPressed(PS2_STATE_R1)) {
        enteringWavCode = true;
        wavCode = 0;
    }

    if (enteringWavCode) {
        // use bit shifting to bring in the code
        if (ps2.isButtonJustPressed(PS2_STATE_CROSS))
            wavCode = wavCode << 2;

        if (ps2.isButtonJustPressed(PS2_STATE_CIRCLE)) {
            wavCode = wavCode << 2;
            wavCode += 1;
        }

        if (ps2.isButtonJustPressed(PS2_STATE_TRIANGLE)) {
            wavCode = wavCode << 2;
            wavCode += 2;
        }

        if (ps2.isButtonJustPressed(PS2_STATE_SQUARE)) {
            wavCode = wavCode << 2;
            wavCode += 3;
        }
    }

    if (ps2.isButtonJustReleased(PS2_STATE_R1)) {
        enteringWavCode = false;

        if (wavCode == 0)
            wavTrigger.stopAllTracks();

        // play it here!
        wavTrigger.trackPlaySolo(wavCode);
    }

    if (ps2.isButtonPressed(PS2_STATE_PAD_UP) && masterVolume < 250) {
        masterVolume += 5;
        wavTrigger.setMasterVolume(masterVolume);
    }
    else  if (ps2.isButtonPressed(PS2_STATE_PAD_DOWN) && masterVolume > 5) {
        masterVolume -= 5;
        wavTrigger.setMasterVolume(masterVolume);
    }

    if (ps2.isButtonJustPressed(PS2_STATE_START))
        wavTrigger.trackPlaySolo(WAV_NOTOUCH);

    if (ps2.isButtonJustPressed(PS2_STATE_R2)) {
        isWavRandomOn = !isWavRandomOn;
        wavLastTimeCheck = millis() - WAV_RANDOM_MILLISECONDS;
    }
}

//---------------------------------------------------------------------------------
// MOTOR METHODS
//---------------------------------------------------------------------------------
void motorSetup() {
    motorSerial.begin(9600);
    domeMotor.autobaud();
    footMotor.autobaud();
}


