// PS2.h
#ifndef __PS2_H__
#define __PS2_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

#define PS2_STATE_LENGTH 6
#define PS2_GET_ALL 31

class PS2 {
    private:
        char currentState[PS2_STATE_LENGTH];
        char previousState[PS2_STATE_LENGTH];

        void getAllValues();

    public:
        PS2();

        void init(long baudrate, byte receivePin, byte transmitPin);

        void update();

    protected:
        SoftwareSerial* ps2Serial;
        void uart_Send(byte data);
    
};


#endif
