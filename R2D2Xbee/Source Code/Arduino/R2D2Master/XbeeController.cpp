#include "XbeeController.h"

void XbeeController::SetupController(int controllerValue, int msbValue, int lsbValue) {
    controller = controllerValue;
    msb = msbValue;
    lsb = lsbValue;
    
    for (int i=0; i<9; i++) {
        currentState[i] = 0;
        previousState[i] = 0;
    }
}

bool XbeeController::IsController(int msbValue, int lsbValue) {
    return msbValue == msb && lsbValue == lsb;
}

void XbeeController::SetStateValue(int index, int value) {
    currentState[index] = value;
}

void XbeeController::ChangeState() {
    for (int i=0; i<9; i++) {
        previousState[i] = currentState[i];
    }
}

void XbeeController::OutputController() {
    Serial.print("CS:");

    for (int i=0; i<9; i++) {
        Serial.print(currentState[i]);
        Serial.print(":");
    }

    Serial.print(" --- PS:");

    for (int i=0; i<9; i++) {
        Serial.print(previousState[i]);
        Serial.print(":");
    }

    Serial.println("");
}

// button values are inverted.. so 1 is not pressed, 0 is pressed

bool XbeeController::IsButtonPressed(int button) {
    return currentState[button] == 0;
}

bool XbeeController::IsButtonReleased(int button) {
    return !IsButtonPressed(button);
}

bool XbeeController::IsButtonJustPressed(int button) {
    return currentState[button] == 0 && previousState[button] == 1;    
}

bool XbeeController::IsButtonJustReleased(int button) {
    return currentState[button] == 1 && previousState[button] == 0;
}

int XbeeController::GetStickValue(int stick) {
    // x is inverted
    if (stick == STICK_X) {
        return 1024 - currentState[stick];
    }
    
    return currentState[stick];
}
