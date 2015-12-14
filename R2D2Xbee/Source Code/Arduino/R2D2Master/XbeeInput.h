#ifndef __XBEEINPUT_H__
#define __XBEEINPUT_H__

#include <XBee.h>
#include <SoftwareSerial.h>
#include "XbeeController.h"

// controllers
#define CONTROLLER_LEFT 0
#define CONTROLLER_RIGHT 1

// buttons
#define BUTTON_SHOULDER_LEFT 0
#define BUTTON_SHOULDER_RIGHT 1
#define BUTTON_THUMBSTICK 2
#define BUTTON_UP 3
#define BUTTON_DOWN 4
#define BUTTON_LEFT 5
#define BUTTON_RIGHT 6

// stick axis
#define STICK_X 7
#define STICK_Y 8

class XbeeInput {
    private:
        XbeeController xbeeController[2];
        Rx64IoSampleResponse ioSample = Rx64IoSampleResponse();
        
    public:
        void AddController(int controller, int msb, int lsb);
        void UpdateControllers(XBee xbee);
        void OutputControllers();
    
        bool IsButtonPressed(int controller, int button);
        bool IsButtonReleased(int controller, int button);
        bool IsButtonJustPressed(int controller, int button);
        bool IsButtonJustReleased(int controller, int button);
        int GetStickValue(int controller, int stick);          
};

#endif
