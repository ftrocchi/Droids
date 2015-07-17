// PS2.h
#ifndef __PS2_H__
#define __PS2_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

// defines here
#define PS2_STATE_LENGTH 6
#define PS2_CONNECTION_STATUS 28
#define PS2_SMALL_MOTOR 29
#define PS2_LARGE_MOTOR 30
#define PS2_GET_ALL 31

// stick states
#define PS2_STATE_RX 2
#define PS2_STATE_RY 3
#define PS2_STATE_LX 4
#define PS2_STATE_LY 5

// buttonstates
#define PS2_STATE_SELECT 0
#define PS2_STATE_L3 1
#define PS2_STATE_R3 2
#define PS2_STATE_START 3
#define PS2_STATE_PAD_UP 4
#define PS2_STATE_PAD_RIGHT 5
#define PS2_STATE_PAD_DOWN 6
#define PS2_STATE_PAD_LEFT 7
#define PS2_STATE_L2 8
#define PS2_STATE_R2 9
#define PS2_STATE_L1 10
#define PS2_STATE_R1 11
#define PS2_STATE_TRIANGLE 12
#define PS2_STATE_CIRCLE 13
#define PS2_STATE_CROSS 14
#define PS2_STATE_SQUARE 15

// tolerances
#define PS2_STICK_CENTERED_TOLERANCE 20
#define PS2_STICK_CENTERED_VALUE 127


class PS2 {
    private:
        char currentState[PS2_STATE_LENGTH];
        char previousState[PS2_STATE_LENGTH];

        void getAllValues();
        bool getButtonBit(char* state, byte button);

    public:
        PS2();

        void init(long baudrate, byte receivePin, byte transmitPin);

        void update();

        bool isButtonPressed(byte button);
        bool isButtonReleased(byte button);
        bool isButtonJustPressed(byte button);
        bool isButtonJustReleased(byte button);

    protected:
        SoftwareSerial* ps2Serial;
        void uart_Send(byte data);
    
};


#endif
