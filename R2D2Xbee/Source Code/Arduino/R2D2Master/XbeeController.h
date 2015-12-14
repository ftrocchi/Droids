#ifndef __XBEECONTROLLER_H__
#define __XBEECONTROLLER_H__

#include "Arduino.h"

#define STICK_X 7

class XbeeController {
    private:
        int controller;
        int msb;
        int lsb;
        int currentState[9];
        int previousState[9];
        
    public:
        void SetupController(int controllerValue, int msbValue, int lsbValue);
        bool IsController(int msbValue, int lsbValue);
        void SetStateValue(int index, int value);
        void ChangeState();
        void OutputController();

        bool IsButtonPressed(int button);
        bool IsButtonReleased(int button);
        bool IsButtonJustPressed(int button);
        bool IsButtonJustReleased(int button);
        int GetStickValue(int stick);        
};

#endif
