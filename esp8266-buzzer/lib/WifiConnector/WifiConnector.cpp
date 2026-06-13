#include "WifiConnector.h"

void WifiConnector::startAccessPoint()
{
    Serial.println("[WiFi] Starting access point...");

    WiFi.mode(WIFI_AP);

    bool success = WiFi.softAP(apSsid, apPassword);

    if (success)
    {
        state = WifiState::ConfigMode;

        Serial.println("[WiFi] Access Point started");
        Serial.print("[WiFi] SSID:");
        Serial.println(apSsid);
        Serial.print("[WiFi] Password:");
        Serial.println(apPassword);

        Serial.print("[WiFi] IP:");
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        state = WifiState::None;

        Serial.println("[WiFi] Failed to start Access Point");
    }
}

void WifiConnector::setupWebServer()
{
    server.on("/", [this]()
              { this->handleRoot(); });

    server.on("/scan", HTTP_GET, [this]()
              { this->handleScanWifi(); });

    server.on("/save", HTTP_POST, [this]()
              { this->handleSaveWifi(); });

    server.on(
        "/command",
        HTTP_POST,
        std::bind(
            &WifiConnector::handleCommandApi,
            this));

    server.onNotFound([this]()
                      {
                        if (!this->handleFileRead(server.uri()))
                        {
                            server.sendHeader("Access-Control-Allow-Origin", "*");
                            server.send(404, "text/plain", "File Not Found");
                        } });

    server.begin();

    Serial.println("[WebServer] Started");
}

void WifiConnector::handleRoot()
{
    handleFileRead("/index.html");
}

void WifiConnector::handleScanWifi()
{
    Serial.println("[WiFi] Scanning...");

    int networkCount = WiFi.scanNetworks();

    String json = "[";

    for (int i = 0; i < networkCount; i++)
    {
        if (i > 0)
        {
            json += ",";
        }

        json += "{";
        json += "\"ssid\":\"";
        json += WiFi.SSID(i);
        json += "\",";
        json += "\"rssi\":";
        json += WiFi.RSSI(i);
        json += ",";
        json += "\"secure\":";
        json += WiFi.encryptionType(i) == ENC_TYPE_NONE ? "false" : "true";
        json += "}";
    }

    json += "]";

    WiFi.scanDelete();

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", json);
}

void WifiConnector::handleSaveWifi()
{
    if (!server.hasArg("ssid") || !server.hasArg("password"))
    {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Password or SSID is missing\"}");
        return;
    }

    String ssid = server.arg("ssid");
    String password = server.arg("password");

    Serial.println("[WiFi] Received config");
    Serial.print("[WiFi] SSID: ");
    Serial.println(ssid);

    bool connected = connectToWifi(ssid, password);

    if (!connected)
    {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(
            400,
            "application/json",
            "{\"success\":false,\"message\":\"Unable to connect to WiFi. Please double-check your password.\"}");
        return;
    }

    saveWifiConfig(ssid, password);

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(
        200,
        "application/json",
        "{\"success\":true,\"message\":\"WiFi connection successful and configuration saved.\"}");
}

void WifiConnector::handleCommandApi()
{
    if (!server.hasArg("command"))
    {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(
            400,
            "text/plain",
            "Missing command");

        return;
    }

    String command = server.arg("command");

    Serial.print("[HTTP] ");
    Serial.println(command);

    if (commandCallback)
    {
        commandCallback(command);
    }

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "OK");
}

bool WifiConnector::handleFileRead(String path)
{
    if (path.endsWith("/"))
    {
        path += "index.html";
    }

    String contentType = getContentType(path);

    if (!LittleFS.exists(path))
    {
        return false;
    }

    File file = LittleFS.open(path, "r");

    server.streamFile(file, contentType);

    file.close();

    return true;
}

String WifiConnector::getContentType(String path)
{
    if (path.endsWith(".html"))
    {
        return "text/html";
    }

    if (path.endsWith(".css"))
    {
        return "text/css";
    }

    if (path.endsWith(".js"))
    {
        return "application/javascript";
    }

    if (path.endsWith(".png"))
    {
        return "image/png";
    }

    if (path.endsWith(".jpg"))
    {
        return "image/jpeg";
    }

    if (path.endsWith(".ico"))
    {
        return "image/x-icon";
    }

    return "text/plain";
}

bool WifiConnector::connectToWifi(String ssid, String password)
{
    Serial.println("[WiFi] Connecting...");

    WiFi.mode(WIFI_AP_STA);

    WiFi.begin(ssid.c_str(), password.c_str());

    unsigned long startTime = millis();
    const unsigned long timeout = 15000;

    while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        state = WifiState::Connected;

        Serial.println("[WiFi] Connected");
        Serial.print("[WiFi] SSID: ");
        Serial.println(ssid);
        Serial.print("[WiFi] IP: ");
        Serial.println(WiFi.localIP());

        return true;
    }

    Serial.println("[WiFi] Connect failed");

    WiFi.disconnect();

    state = WifiState::ConfigMode;

    return false;
}

void WifiConnector::saveWifiConfig(String ssid, String password)
{
    File file = LittleFS.open("/wifi_config.txt", "w");

    if (!file)
    {
        Serial.println("[WiFi] Failed to open config file");
        return;
    }

    file.println(ssid);
    file.println(password);

    file.close();

    Serial.println("[WiFi] Config saved");
}

bool WifiConnector::loadWifiConfig(String &ssid, String &password)
{
    if (!LittleFS.exists("/wifi_config.txt"))
    {
        return false;
    }

    File file = LittleFS.open("/wifi_config.txt", "r");

    if (!file)
    {
        return false;
    }

    ssid = file.readStringUntil('\n');
    password = file.readStringUntil('\n');

    ssid.trim();
    password.trim();

    file.close();

    if (ssid.length() == 0)
    {
        return false;
    }

    return true;
}

void WifiConnector::setupFileSystem()
{
    if (LittleFS.begin())
    {
        Serial.println("[LittleFS] Initialized");
    }
    else
    {
        Serial.println("[LittleFS] Failed to initialize");
    }
}

WifiConnector::WifiConnector(char *apSsid, char *apPassword) : server(80)
{
    state = WifiState::None;

    this->apSsid = apSsid;
    this->apPassword = apPassword;
}

void WifiConnector::begin()
{
    Serial.println("[WiFi] Initialized");

    setupFileSystem();

    String ssid;
    String password;

    if (loadWifiConfig(ssid, password))
    {
        Serial.println("[WiFi] Saved config found");

        bool connected = connectToWifi(ssid, password);

        if (connected)
        {
            setupWebServer();
            return;
        }

        Serial.println("[WiFi] Saved config failed");
    }
    else
    {
        Serial.println("[WiFi] No saved config");
    }

    startAccessPoint();

    setupWebServer();
}

void WifiConnector::update()
{
    server.handleClient();
}

bool WifiConnector::isConnected()
{
    return state == WifiState::Connected;
}

String WifiConnector::getIp()
{
    if (state == WifiState::ConfigMode)
    {
        return WiFi.softAPIP().toString();
    }

    if (state == WifiState::Connected)
    {
        return WiFi.localIP().toString();
    }

    return "0.0.0.0";
}

WifiState WifiConnector::getState()
{
    return state;
}

void WifiConnector::setCommandCallback(std::function<void(String)> callback)
{
    commandCallback = callback;
}
