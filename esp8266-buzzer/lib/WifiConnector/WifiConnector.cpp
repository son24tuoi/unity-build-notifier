#include "WifiConnector.h"

WifiConnector::WifiConnector()
{
    state = WifiState::None;
}

void WifiConnector::begin()
{
    Serial.println("[WiFi] Initialized");
    state = WifiState::None;
}

void WifiConnector::update()
{
    // Sau này xử lý WiFi ở đây
}

bool WifiConnector::isConnected()
{
    return state == WifiState::Connected;
}

String WifiConnector::getIp()
{
    return "0.0.0.0";
}

WifiState WifiConnector::getState()
{
    return state;
}