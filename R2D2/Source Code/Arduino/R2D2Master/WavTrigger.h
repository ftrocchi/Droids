// WavTrigger.h
#ifndef __WAV_TRIGGER_H__
#define __WAV_TRIGGER_H__

#include <arduino.h>
#include <SendOnlySoftwareSerial.h>

#define CMD_VOLUME 5
#define CMD_AMP_POWER 9

class WavTrigger {
    private:
        SendOnlySoftwareSerial *wavSerial;

    public:
        WavTrigger(SendOnlySoftwareSerial *wavSendOnlySoftwareSerial);

        void setMasterVolume(int volume); // 0x05
        void ampPower(byte isOn); // 0x09
    
};

#endif
