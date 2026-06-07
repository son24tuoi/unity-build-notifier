#ifndef BUZZER_MANAGER_H
#define BUZZER_MANAGER_H

#include <Arduino.h>

enum class BuzzerMode
{
    None,
    Success,
    Failed
};

class BuzzerManager
{
private:
    int speakerPin;
    int greenLedPin;
    int redLedPin;

    int tempo;
    bool isPlaying;
    BuzzerMode mode;

    void setLed(bool isOn);

public:
    BuzzerManager(int speakerPin, int greenLedPin, int redLedPin);

    void begin();

    void playStart();
    void playSuccess();
    void playFailed();

    bool getIsPlaying();
};

#endif