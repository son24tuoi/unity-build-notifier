#ifndef OLED_DISPLAY_MANAGER_H
#define OLED_DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class OledDisplayManager
{
private:
    Adafruit_SSD1306 display;

public:
    OledDisplayManager();

    bool begin();

    void clear();
    void showText(String text);
    void showStartup();
    void showConfigMode(String apName, String ip);
    void showWifiConnected(String ssid, String ip);
    void showBuildSuccess();
    void showBuildFailed();
};

#endif