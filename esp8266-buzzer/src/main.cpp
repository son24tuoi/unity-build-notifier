#include <Arduino.h>
#include <WifiConnector.h>
#include <BuzzerManager.h>

#define AP_SSID "ESP8266_LuToTahSo"
#define AP_PASSWORD "12345678"

WifiConnector wifiConnector(AP_SSID, AP_PASSWORD);

int speakerPin = D3;
int buttonPin = D0;
int greenLedPin = D6;
int redLedPin = D7;

BuzzerManager buzzerManager(speakerPin, greenLedPin, redLedPin);

int buttonState = 0;

void setup()
{
  Serial.begin(9600);

  wifiConnector.begin();
  buzzerManager.begin();

  pinMode(buttonPin, INPUT);

  buzzerManager.playStart();
}

void readSerialCommand()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("success"))
    {
      Serial.println("Received success command");
      buzzerManager.playSuccess();
    }
    else if (command.equalsIgnoreCase("failed"))
    {
      Serial.println("Received failed command");
      buzzerManager.playFailed();
    }
  }
}

void readButtonInput()
{
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH)
  {
    buzzerManager.playStart();
  }
}

void loop()
{
  wifiConnector.update();

  readSerialCommand();

  readButtonInput();

  delay(100);
}