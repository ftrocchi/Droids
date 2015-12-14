// PS2.cpp
#include "PS2.h"

PS2::PS2() {
}

void PS2::init(long baudrate, byte receivePin, byte transmitPin) {
    // set the pins modes
    pinMode(receivePin, INPUT);
    pinMode(transmitPin, OUTPUT);

    // create the serial connection
    ps2Serial = new SoftwareSerial(receivePin, transmitPin);
    ps2Serial->begin(baudrate);

    pinMode(A1, OUTPUT);
    digitalWrite(A1, HIGH);
}

void PS2::update() {
    // copy the current to the previous before reading the state of the controller
    strncpy(previousState, currentState, PS2_STATE_LENGTH);

    // now read the current state of the controller
    getAllValues();
}

void PS2::getAllValues() {
    long waitCount = 0;

    // request all controller information
    uart_Send(PS2_GET_ALL);

    // wait for enough data
    while (ps2Serial->available() < PS2_STATE_LENGTH) {
        waitCount++;
        if (waitCount > 50000) 
            return;
    }

    // read data into current state
    for (int i=0; i<PS2_STATE_LENGTH; i++)
        currentState[i] = ps2Serial->read();
}

void PS2::uart_Send(byte data) {
    while(ps2Serial->available() > 0)
        ps2Serial->read();

    ps2Serial->write(data);
}

bool PS2::getButtonBit(char* state, byte button)
{
    if (button > PS2_STATE_SQUARE) 
        return false;
        
    return (state[button / 8] & (1 << button % 8)) == 0;
}

bool PS2::isButtonPressed(byte button)
{
    return getButtonBit(currentState, button);
}

bool PS2::isButtonReleased(byte button)
{
    return !isButtonPressed(button);
}

bool PS2::isButtonJustPressed(byte button)
{
    bool current = getButtonBit(currentState, button);
    bool previous = getButtonBit(previousState, button);
    
    return (current && !previous);
}

bool PS2::isButtonJustReleased(byte button)
{
    bool current = getButtonBit(currentState, button);
    bool previous = getButtonBit(previousState, button);

    return (!current && previous);    
}

byte PS2::getStickValue(byte stick) {
    if (stick != PS2_STATE_RX && stick != PS2_STATE_RY && stick != PS2_STATE_LX && stick != PS2_STATE_LY)
        return 0;

    return currentState[stick];
}

