#include "OledDisplayManager.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

OledDisplayManager::OledDisplayManager()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
{
}

bool OledDisplayManager::begin()
{
    Wire.begin(D2, D1); // SDA = D2, SCL = D1

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
    {
        Serial.println("[OLED] Start failed");
        return false;
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("OLED Ready");
    display.display();

    Serial.println("[OLED] Started");

    return true;
}

void OledDisplayManager::clear()
{
    display.clearDisplay();
    display.display();
}

void OledDisplayManager::showText(String text)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(text);
    display.display();
}

void OledDisplayManager::showStartup()
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Build Notifier");

    display.setCursor(0, 20);
    display.println("Starting...");

    display.display();
}

void OledDisplayManager::showConfigMode(String apName, String ip)
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Config Mode");

    display.setCursor(0, 18);
    display.println("SSID:");
    display.println(apName);

    display.setCursor(0, 45);
    display.println(ip);

    display.display();
}

void OledDisplayManager::showWifiConnected(String ssid, String ip)
{
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("WiFi Connected");

    display.setCursor(0, 18);
    display.println(ssid);

    display.setCursor(0, 36);
    display.println(ip);

    display.display();
}

void OledDisplayManager::showBuildSuccess()
{
    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(0, 8);
    display.println("BUILD");

    display.setCursor(0, 32);
    display.println("SUCCESS");

    display.display();
}

void OledDisplayManager::showBuildFailed()
{
    display.clearDisplay();

    display.setTextSize(2);
    display.setCursor(0, 8);
    display.println("BUILD");

    display.setCursor(0, 32);
    display.println("FAILED");

    display.display();
}