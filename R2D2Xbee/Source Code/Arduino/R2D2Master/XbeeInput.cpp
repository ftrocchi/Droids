#include "XbeeInput.h"

void XbeeInput::AddController(int controller, int msb, int lsb) {
    xbeeController[controller].SetupController(controller, msb, lsb);
}

void XbeeInput::UpdateControllers(XBee xbee) {
    // attempt to read a packet
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
        // got something
        
        if (xbee.getResponse().getApiId() == RX_64_IO_RESPONSE) {
            xbee.getResponse().getRx64IoSampleResponse(ioSample);
            
            int msb = ioSample.getRemoteAddress64().getMsb();
            int lsb = ioSample.getRemoteAddress64().getLsb();
            
            // determine which controller we need to associate
            int controller = -1;
            for (int i=0; i<2; i++) {
                if (xbeeController[i].IsController(msb, lsb)) {
                    controller = i;
                }
            }
            
            if (controller != -1) {
                xbeeController[controller].ChangeState();
                
                xbeeController[controller].SetStateValue(BUTTON_SHOULDER_LEFT, ioSample.isDigitalOn(7, 0));
                xbeeController[controller].SetStateValue(BUTTON_SHOULDER_RIGHT, ioSample.isDigitalOn(8, 0));
                xbeeController[controller].SetStateValue(BUTTON_THUMBSTICK, ioSample.isDigitalOn(0, 0));
                xbeeController[controller].SetStateValue(BUTTON_UP, ioSample.isDigitalOn(3, 0));
                xbeeController[controller].SetStateValue(BUTTON_DOWN, ioSample.isDigitalOn(6, 0));
                xbeeController[controller].SetStateValue(BUTTON_LEFT, ioSample.isDigitalOn(5, 0));
                xbeeController[controller].SetStateValue(BUTTON_RIGHT, ioSample.isDigitalOn(4, 0));
                xbeeController[controller].SetStateValue(STICK_X, ioSample.getAnalog(1, 0));
                xbeeController[controller].SetStateValue(STICK_Y, ioSample.getAnalog(2, 0));
            }
        }
    }
}

void XbeeInput::OutputControllers() {
    xbeeController[CONTROLLER_LEFT].OutputController();
    xbeeController[CONTROLLER_RIGHT].OutputController();
}

bool XbeeInput::IsButtonPressed(int controller, int button) {
    return xbeeController[controller].IsButtonPressed(button);
}

bool XbeeInput::IsButtonReleased(int controller, int button) {
    return xbeeController[controller].IsButtonReleased(button);
}

bool XbeeInput::IsButtonJustPressed(int controller, int button) {
    return xbeeController[controller].IsButtonJustPressed(button);
}

bool XbeeInput::IsButtonJustReleased(int controller, int button) {
    return xbeeController[controller].IsButtonJustReleased(button);
}

int XbeeInput::GetStickValue(int controller, int stick) {
    return xbeeController[controller].GetStickValue(stick);
}


