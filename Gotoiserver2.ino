//test 8
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Credentials
const char* ssid = "POCO";
const char* password = "pritam67";

// Flask Server Address
const char* serverAddress = "http:/******/******/predict";

// Static IP Configuration
IPAddress local_IP(192, 168, 4, 110);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 0, 0);

// WiFi Server on Port 80
WiFiServer server(80);

// Global Variables
int requests = 40;
int errors = 5;
unsigned long lastSendTime = 0;
unsigned long attackStartTime = 0;
bool attackInProgress = false;
bool resetTriggered = false;
unsigned long resetStartTime = 0;

// LED Pin
const int ledPin = 18;
bool ledState = false;

void connectToWiFi() {
    WiFi.config(local_IP, gateway, subnet);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\n WiFi Connected!");
    Serial.print("📡 ESP32 Static IP: ");
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("🌐 Server Listening on Port 80...");
}

//  Function to send data to Flask
void sendToFlask() {
    HTTPClient http;
    http.begin(serverAddress);
    http.addHeader("Content-Type", "application/json");

    // Keep sending `0,0` during reset period
    String jsonPayload = "{\"requests\": " + String(requests) + ", \"errors\": " + String(errors) + "}";

    Serial.println("📡 Sending Data to Flask: " + jsonPayload);

    int httpResponseCode = http.POST(jsonPayload);
    String response = http.getString();

    Serial.print("🔄 HTTP Response Code: ");
    Serial.println(httpResponseCode);
    Serial.println("📨 Server Response: " + response);

    http.end();
}

// Function to handle incoming client requests
void handleClientRequest() {
    WiFiClient client = server.available();
    if (!client) return;

    Serial.println("\n🌍 New Client Connected!");

    while (!client.available()) {
        delay(10);
    }

    String request = client.readStringUntil('\r');
    client.flush();

    Serial.println("📩 Received Request: " + request);

    if (resetTriggered) {
        Serial.println("🚨 Attack Recently Detected! Ignoring Requests...");
    } else {
        requests++;
        errors = requests * 0.3;
        Serial.println("📌 Total Requests: " + String(requests));
        Serial.println("❌ Total Errors: " + String(errors));
    }

    //  Detect an attack and start reset process
    if (!attackInProgress) {
        attackInProgress = true;
        attackStartTime = millis();
        Serial.println("⚠️ Attack Detected! Timer Started...");
    } 

    // ✅ If attack is active for 4 seconds, reset permanently
    if (attackInProgress && millis() - attackStartTime >= 12000) {
        Serial.println("🚨 Attack Persistent for 4 Sec! Resetting...");
        requests = 0;
        errors = 0;
        resetTriggered = true;
        resetStartTime = millis();
        attackInProgress = false;
    }

    // ✅ Respond to client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("✅ Data Received Successfully!");
    client.stop();
}

// ✅ LED Control Function
void controlLED() {
    if (resetTriggered) {
        digitalWrite(ledPin, LOW);  // ⚪ Turn OFF LED in reset mode
    } else if (attackInProgress) {
        ledState = !ledState;  // 🔴 Blink LED during attack
        digitalWrite(ledPin, ledState);
    } else {
        digitalWrite(ledPin, HIGH);  // 🟢 Keep LED ON in normal mode
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    connectToWiFi();
}

void loop() {
    handleClientRequest();

    // ✅ Keep sending `0,0` to Flask even if new requests come in
    if (resetTriggered) {
        requests = 0;
        errors = 0;

        // ✅ Reset Mode Lasts for 10 Sec Before Allowing Updates Again
        if (millis() - resetStartTime >= 20000) {
            Serial.println("✅ Reset Period Over! Resuming Normal Counting...");
            resetTriggered = false;
        }
    }

    // ✅ Send data to Flask every 2 seconds
    if (millis() - lastSendTime >= 2000) {
        sendToFlask();
        lastSendTime = millis();
    }

    // ✅ Control LED State
    controlLED();
    delay(500);
}

