#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <Arduino.h>

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

public:
    WifiConnector();

    void begin();
    void update();

    bool isConnected();
    String getIp();

    WifiState getState();
};

#endif