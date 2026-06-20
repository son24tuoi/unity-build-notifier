#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <functional>

enum class WifiState
{
    None,
    Connecting,
    Connected,
    ConfigMode
};

class WifiConnector
{
private:
    WifiState state;

    ESP8266WebServer server;

    const char *apSsid;
    const char *apPassword;

    void startAccessPoint();

    void setupWebServer();
    void setupFileSystem();

    void handleRoot();

    void handleScanWifi();
    void handleSaveWifi();
    void handleCommandApi();
    void handleClearData();
    void handleRestart();

    bool handleFileRead(String path);
    String getContentType(String path);

    bool connectToWifi(String ssid, String password);
    void saveWifiConfig(String ssid, String password);

    bool loadWifiConfig(String &ssid, String &password);

    std::function<void(String)> commandCallback;

public:
    WifiConnector(char *apSsid, char *apPassword);

    void begin();
    void update();

    bool isConnected();
    String getIp();

    WifiState getState();

    void setCommandCallback(std::function<void(String)> callback);
};

#endif