// R2D2Master.ino
#include <SoftwareSerial.h>

#include "PS2.h"

//---------------------------------------------------------------------------------
// PS2 DECLARATIONS
//---------------------------------------------------------------------------------
PS2 ps2 = PS2();

//---------------------------------------------------------------------------------
// SETUP AND LOOP
//---------------------------------------------------------------------------------
void setup() {
    // set the random seed, which is used for this such as the random audio feature
    randomSeed(analogRead(0));

    // setup the PS2 controller, 9600 baud, pin 8 receive, pin 9 transmit
    ps2.init(9600, 8, 9);
}

void loop() {
    // Process the PS2 controller
    processPS2();
}

//---------------------------------------------------------------------------------
// PS2 METHODS
//---------------------------------------------------------------------------------
void processPS2() {
    ps2.update();
}

