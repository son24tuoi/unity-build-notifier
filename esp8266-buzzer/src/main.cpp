#include <Arduino.h>
#include <WifiConnector.h>
#include <BuzzerManager.h>
#include <OledDisplayManager.h>

#define AP_SSID "ESP8266_LuToTahSo"
#define AP_PASSWORD "12345678"

int speakerPin = D3;
int buttonPin = D0;
int greenLedPin = D6;
int redLedPin = D7;

WifiConnector wifiConnector(AP_SSID, AP_PASSWORD);

BuzzerManager buzzerManager(speakerPin, greenLedPin, redLedPin);

OledDisplayManager oledDisplay;

int buttonState = 0;

void handleCommand(String command);

void setup()
{
  Serial.begin(9600);

  buzzerManager.begin();
  oledDisplay.begin();
  oledDisplay.showStartup();

  pinMode(buttonPin, INPUT);

  wifiConnector.begin();
  wifiConnector.setCommandCallback(handleCommand);

  buzzerManager.playStart();

  oledDisplay.showText("LuToTahSo");
}

void handleCommand(String command)
{
  command.trim();

  Serial.println(command);

  int separatorIndex = command.indexOf('|');

  String code;
  String message;

  if (separatorIndex >= 0)
  {
    code = command.substring(0, separatorIndex);
    message = command.substring(separatorIndex + 1);
    message.trim();
  }
  else
  {
    code = command;
  }

  bool playSuccess = false;
  bool playFailed = false;

  if (code == "100")
  {
    oledDisplay.showText("LuToTahSo");
  }
  else if (code == "101")
  {
    oledDisplay.showText("Hello World!");
  }
  else if (code == "102")
  {
    oledDisplay.showConfigMode(
        AP_SSID,
        WiFi.softAPIP().toString());
  }
  else if (code == "103")
  {
    oledDisplay.showWifiConnected(
        WiFi.SSID(),
        WiFi.localIP().toString());
  }
  else if (code == "104")
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      oledDisplay.showWifiConnected(
          WiFi.SSID(),
          WiFi.localIP().toString());
    }
    else
    {
      oledDisplay.showConfigMode(
          AP_SSID,
          WiFi.softAPIP().toString());
    }
  }
  else if (code == "200")
  {
    oledDisplay.showBuildSuccess();
    playSuccess = true;
  }
  else if (code == "201")
  {
    oledDisplay.showBuildFailed();
    playFailed = true;
  }
  else if (code == "300")
  {
    oledDisplay.showText(message);
    playSuccess = true;
  }
  else if (code == "301")
  {
    oledDisplay.showText(message);
    playFailed = true;
  }
  else
  {
    oledDisplay.showText(command);
  }

  if (playSuccess || playFailed)
  {
    delay(100);

    if (playSuccess)
    {
      buzzerManager.playSuccess();
    }
    else
    {
      buzzerManager.playFailed();
    }
  }
}

void readSerialCommand()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');

    handleCommand(command);
  }
}

void readButtonInput()
{
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH)
  {
    handleCommand("104");
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