#include "WavTrigger.h"

WavTrigger::WavTrigger(SendOnlySoftwareSerial *wavSendOnlySoftwareSerial) {
    wavSerial = wavSendOnlySoftwareSerial;
}

// 0x05
void WavTrigger::setMasterVolume(int volume) {
    // map the volume to the correct range
    int mappedVolume = map(volume, 0, 255, -70, 10);

    wavSerial->write(0xf0);
    wavSerial->write(0xaa);
    wavSerial->write(0x07);
    wavSerial->write(CMD_VOLUME);
    wavSerial->write((byte)mappedVolume);
    wavSerial->write((byte)(mappedVolume >> 8));
    wavSerial->write(0x55);
}

// 0x09
void WavTrigger::ampPower(byte isOn) {
    wavSerial->write(0xf0);
    wavSerial->write(0xaa);
    wavSerial->write(0x06);
    wavSerial->write(CMD_AMP_POWER);
    wavSerial->write(isOn);
    wavSerial->write(0x55);
}

