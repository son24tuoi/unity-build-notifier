#include "BuzzerManager.h"
#include <pitch.h>

int startMelody[] = {
    NOTE_C7, NOTE_G6, NOTE_G6, NOTE_A6, NOTE_G6, 0, NOTE_B6, NOTE_C7};

int startDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4};

int successMelody[] = {
    NOTE_C5, NOTE_F5, NOTE_C6, NOTE_A5, NOTE_B5, NOTE_G5};

int successDurations[] = {
    8, 8, 8, 8, 8, 2};

int failedMelody[] = {
    NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_GS4, NOTE_G4};

int failedDurations[] = {
    6, 6, 6, 6, 6, 6, 6, 2};

BuzzerManager::BuzzerManager(int speakerPin, int greenLedPin, int redLedPin)
{
    this->speakerPin = speakerPin;
    this->greenLedPin = greenLedPin;
    this->redLedPin = redLedPin;

    tempo = 1300;
    isPlaying = false;
    mode = BuzzerMode::None;
}

void BuzzerManager::begin()
{
    pinMode(speakerPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(redLedPin, OUTPUT);

    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, LOW);
}

void BuzzerManager::setLed(bool isOn)
{
    if (mode == BuzzerMode::Success)
    {
        digitalWrite(greenLedPin, isOn ? HIGH : LOW);
    }
    else if (mode == BuzzerMode::Failed)
    {
        digitalWrite(redLedPin, isOn ? HIGH : LOW);
    }
}

void BuzzerManager::playStart()
{
    if (isPlaying)
        return;

    isPlaying = true;
    mode = BuzzerMode::None;

    for (int i = 0; i < 8; i++)
    {
        int noteDuration = 1000 / startDurations[i];

        tone(speakerPin, startMelody[i], noteDuration);

        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);

        noTone(speakerPin);
    }

    isPlaying = false;
    mode = BuzzerMode::None;
}

void BuzzerManager::playSuccess()
{
    if (isPlaying)
        return;

    isPlaying = true;
    mode = BuzzerMode::Success;

    setLed(true);

    for (int i = 0; i < 6; i++)
    {
        int noteDuration = tempo / successDurations[i];

        tone(speakerPin, successMelody[i], noteDuration * 0.9);
        delay(noteDuration);

        noTone(speakerPin);
    }

    setLed(false);

    isPlaying = false;
    mode = BuzzerMode::None;
}

void BuzzerManager::playFailed()
{
    if (isPlaying)
        return;

    isPlaying = true;
    mode = BuzzerMode::Failed;

    setLed(true);

    for (int i = 0; i < 8; i++)
    {
        int noteDuration = tempo / failedDurations[i];

        tone(speakerPin, failedMelody[i], noteDuration * 0.9);
        delay(noteDuration);

        noTone(speakerPin);
    }

    setLed(false);

    isPlaying = false;
    mode = BuzzerMode::None;
}

bool BuzzerManager::getIsPlaying()
{
    return isPlaying;
}