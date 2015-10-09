// R2D2Master.ino
#include <SoftwareSerial.h>
#include <SendOnlySoftwareSerial.h>
#include <Sabertooth.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

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
// DOME SERVO DECLARATIONS
//---------------------------------------------------------------------------------
bool areDomeServosOpen;
Adafruit_PWMServoDriver servoDriver = Adafruit_PWMServoDriver(0x40);
#define ADDRESS_OFFSET 0

//---------------------------------------------------------------------------------
// MOTOR DECLARATIONS
//---------------------------------------------------------------------------------
#define MOTOR_PIN 7
#define FOOT_MOTOR_SPEED_MAX 127 // 64 for 50%, 127 for 100%
#define DOME_MOTOR_SPEED_MAX 96
SendOnlySoftwareSerial motorSerial(MOTOR_PIN);
Sabertooth domeMotor(128, motorSerial);
Sabertooth footMotor(129, motorSerial);

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

    // close the dome servos
    domeServoSetup();

    // play the startup sound
    wavCode = WAV_STARTUP;
}

void loop() {
    // Process the PS2 controller
    processPS2();

    // Process the audio
    processWavTrigger();

    // Process the dome servos
    processDomeServos();

    // Process the dome motor
    processDomeMotor();

    // Process the foot motor
    processFootMotor();
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

    if (ps2.isButtonJustPressed(PS2_STATE_START)) {
        wavTrigger.trackPlaySolo(WAV_NOTOUCH);
    }

    if (ps2.isButtonJustPressed(PS2_STATE_R2)) {
        isWavRandomOn = !isWavRandomOn;
        wavLastTimeCheck = millis() - WAV_RANDOM_MILLISECONDS;
    }
}

//---------------------------------------------------------------------------------
// DOME SERVO METHODS
//---------------------------------------------------------------------------------
void domeServoSetup() {
    servoDriver.begin();
    servoDriver.setPWMFreq(50); 
    delay(500);

    areDomeServosOpen = true;
    toggleAllServos();
}

void processDomeServos() {
    if (ps2.isButtonJustPressed(PS2_STATE_L1)) {
        toggleAllServos();
    }
}

void toggleAllServos() {
    if (areDomeServosOpen) {
        // close all servos
        servoDriver.setPWM(0, ADDRESS_OFFSET, 350);
        servoDriver.setPWM(1, ADDRESS_OFFSET, 335);
        servoDriver.setPWM(2, ADDRESS_OFFSET, 335);
        servoDriver.setPWM(3, ADDRESS_OFFSET, 315);
        servoDriver.setPWM(4, ADDRESS_OFFSET, 300);
        servoDriver.setPWM(5, ADDRESS_OFFSET, 345);
        servoDriver.setPWM(6, ADDRESS_OFFSET, 350);
        servoDriver.setPWM(7, ADDRESS_OFFSET, 340);
        servoDriver.setPWM(8, ADDRESS_OFFSET, 315);
        servoDriver.setPWM(9, ADDRESS_OFFSET, 300);

    } else {
        // open all servos
        servoDriver.setPWM(0, ADDRESS_OFFSET, 240);
        servoDriver.setPWM(1, ADDRESS_OFFSET, 240);
        servoDriver.setPWM(2, ADDRESS_OFFSET, 240);
        servoDriver.setPWM(3, ADDRESS_OFFSET, 240);
        servoDriver.setPWM(4, ADDRESS_OFFSET, 150);
        servoDriver.setPWM(5, ADDRESS_OFFSET, 175);
        servoDriver.setPWM(6, ADDRESS_OFFSET, 205);
        servoDriver.setPWM(7, ADDRESS_OFFSET, 195);
        servoDriver.setPWM(8, ADDRESS_OFFSET, 165);
        servoDriver.setPWM(9, ADDRESS_OFFSET, 160);
    }

    areDomeServosOpen = !areDomeServosOpen;
}

//---------------------------------------------------------------------------------
// MOTOR METHODS
//---------------------------------------------------------------------------------
void motorSetup() {
    motorSerial.begin(9600);
    domeMotor.autobaud();
    footMotor.autobaud();
}

void processDomeMotor() {
    int value = ps2.getStickValue(PS2_STATE_LX);

    int mappedValue = map(value, 0, 255, -DOME_MOTOR_SPEED_MAX, DOME_MOTOR_SPEED_MAX);

    if (abs(mappedValue) <= 20)
        mappedValue = 0;

    domeMotor.motor(mappedValue);
}

void processFootMotor() {
    int driveValue = map(ps2.getStickValue(PS2_STATE_RX), 0, 255, -FOOT_MOTOR_SPEED_MAX, FOOT_MOTOR_SPEED_MAX);
    int turnValue = map(ps2.getStickValue(PS2_STATE_RY), 0, 255, -FOOT_MOTOR_SPEED_MAX, FOOT_MOTOR_SPEED_MAX);

    footMotor.drive(driveValue);
    footMotor.turn(turnValue);
}


