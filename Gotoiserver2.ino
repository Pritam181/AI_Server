// #include <WiFi.h>
// #include <HTTPClient.h>

// // WiFi credentials
// const char* ssid = "POCO";
// const char* password = "pritam67";

// // Flask Server Address
// const char* serverAddress = "http://192.168.4.100:5000/predict";

// // Static IP Configuration
// IPAddress local_IP(192, 168, 4, 110);
// IPAddress gateway(192, 168, 4, 1);
// IPAddress subnet(255, 255, 0, 0);

// // LED Pin
// const int ledPin = 2;

// // Global variables
// unsigned long attackStartTime = 0;
// bool attackStarted = false;
// bool wifiConnected = false;

// // Function to ensure stable WiFi connection
// void connectToWiFi() {
//     WiFi.config(local_IP, gateway, subnet);
//     WiFi.begin(ssid, password);
    
//     Serial.print("Connecting to WiFi");
//     for (int i = 0; i < 15; i++) { // Try for 15 seconds
//         if (WiFi.status() == WL_CONNECTED) {
//             wifiConnected = true;
//             Serial.println("\n✅ WiFi Connected!");
//             Serial.print("📡 ESP32 Static IP: ");
//             Serial.println(WiFi.localIP());
//             return;
//         }
//         Serial.print(".");
//         delay(1000);
//     }
    
//     Serial.println("\WiFi Connection Failed! Retrying...");
//     wifiConnected = false;
// }

// // Function to send data to Flask
// void sendToFlask(int requests, int errors) {
//     if (!wifiConnected) {
//         Serial.println("WiFi Not Connected. Skipping request...");
//         return;
//     }

//     HTTPClient http;
//     http.begin(serverAddress);
//     http.addHeader("Content-Type", "application/json");

//     String jsonPayload = "{\"requests\": " + String(requests) + ", \"errors\": " + String(errors) + "}";
//     Serial.println("📡 Sending Data to Flask: " + jsonPayload);

//     int httpResponseCode = http.POST(jsonPayload);
//     String response = http.getString();

//     Serial.print("HTTP Response Code: ");
//     Serial.println(httpResponseCode);
//     Serial.println("📨 Server Response: " + response);

//     // If an attack is detected, blink LED
//     if (response.indexOf("\"attack\": 1") >= 0) {
//         Serial.println("🚨 DDoS Attack Detected! Blinking LED...");
//         for (int i = 0; i < 5; i++) {
//             digitalWrite(ledPin, HIGH);
//             delay(200);
//             digitalWrite(ledPin, LOW);
//             delay(200);
//         }
//     } else {
//         Serial.println("No Attack. Keeping LED ON...");
//         digitalWrite(ledPin, HIGH); // Keep LED ON
//     }

//     http.end();
// }

// void setup() {
//     Serial.begin(115200);
//     pinMode(ledPin, OUTPUT);

//     // Ensure LED stays ON at startup
//     digitalWrite(ledPin, HIGH);

//     // Connect to WiFi
//     connectToWiFi();

//     // Record start time for attack delay
//     attackStartTime = millis();
// }

// void loop() {
//     // If WiFi is lost, try reconnecting
//     if (WiFi.status() != WL_CONNECTED) {
//         Serial.println("WiFi Disconnected! Reconnecting...");
//         wifiConnected = false;
//         connectToWiFi();
//     }

//     // Wait 10 seconds before starting attack simulation
//     if (!attackStarted && millis() - attackStartTime < 10000) {
//         Serial.println("⏳ Waiting 10 seconds before starting attack simulation...");
//         delay(1000);
//         return; 
//     }

//     // Start attack simulation
//     attackStarted = true;
//     int requests = random(800, 1500);
//     int errors = random(50, 200);

//     sendToFlask(requests, errors);

//     delay(5000); // Wait before next request
// }



//test -2 (without led)
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WiFiServer.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>

// // WiFi Credentials
// const char* ssid = "POCO";
// const char* password = "pritam67";

// // Flask Server Address
// const char* serverAddress = "http://192.168.4.100:5000/predict";

// // Static IP Configuration
// IPAddress local_IP(192, 168, 4, 110);
// IPAddress gateway(192, 168, 4, 1);
// IPAddress subnet(255, 255, 0, 0);

// // WiFi Server on Port 80
// WiFiServer server(80);

// // Global Variables
// int requests = 0;
// int errors = 0;
// unsigned long lastSendTime = 0;
// unsigned long attackStartTime = 0;
// bool attackInProgress = false;
// bool resetTriggered = false;  // ✅ Flag to keep requests & errors at 0
// unsigned long resetStartTime = 0;  // ✅ Timer for reset period

// void connectToWiFi() {
//     WiFi.config(local_IP, gateway, subnet);
//     WiFi.begin(ssid, password);

//     Serial.print("Connecting to WiFi");
//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print(".");
//         delay(1000);
//     }
//     Serial.println("\n✅ WiFi Connected!");
//     Serial.print("📡 ESP32 Static IP: ");
//     Serial.println(WiFi.localIP());

//     server.begin();
//     Serial.println("🌐 Server Listening on Port 80...");
// }

// // ✅ Function to send data to Flask
// void sendToFlask() {
//     HTTPClient http;
//     http.begin(serverAddress);
//     http.addHeader("Content-Type", "application/json");

//     // ✅ Ensure `requests = 0` and `errors = 0` during reset period
//     String jsonPayload = "{\"requests\": " + String(requests) + ", \"errors\": " + String(errors) + "}";

//     Serial.println("📡 Sending Data to Flask: " + jsonPayload);

//     int httpResponseCode = http.POST(jsonPayload);
//     String response = http.getString();

//     Serial.print("🔄 HTTP Response Code: ");
//     Serial.println(httpResponseCode);
//     Serial.println("📨 Server Response: " + response);

//     http.end();
// }

// // ✅ Function to handle incoming client requests
// void handleClientRequest() {
//     WiFiClient client = server.available();
//     if (!client) return;

//     Serial.println("\n🌍 New Client Connected!");

//     while (!client.available()) {
//         delay(10);
//     }

//     String request = client.readStringUntil('\r');
//     client.flush();

//     Serial.println("📩 Received Request: " + request);

//     // ✅ If in reset mode, ignore new requests
//     if (resetTriggered) {
//         Serial.println("🚨 Attack Recently Detected! Ignoring Requests...");
//     } else {
//         // ✅ Update Counters Normally if Not in Reset Mode
//         requests++;
//         errors = requests * 0.3;
//         Serial.println("📌 Total Requests: " + String(requests));
//         Serial.println("❌ Total Errors: " + String(errors));
//     }

//     // ✅ Detect an attack and start reset process
//     if (!attackInProgress) {
//         attackInProgress = true;
//         attackStartTime = millis();
//         Serial.println("⚠️ Attack Detected! Timer Started...");
//     } 

//     // ✅ If attack is active for 4 seconds, reset permanently
//     if (attackInProgress && millis() - attackStartTime >= 12000) {
//         Serial.println("🚨 Attack Persistent for 12 Sec! Resetting...");
//         requests = 0;
//         errors = 0;
//         resetTriggered = true;  // ✅ Prevent updates for some time
//         resetStartTime = millis();  // Start reset period timer
//         attackInProgress = false;
//     }

//     //  Respond to client
//     client.println("HTTP/1.1 200 OK");
//     client.println("Content-Type: text/plain");
//     client.println("Connection: close");
//     client.println();
//     client.println(" Data Received Successfully!");
//     client.stop();
// }

// void setup() {
//     Serial.begin(115200);
//     connectToWiFi();
// }

// void loop() {
//     handleClientRequest();

//     // Keep sending `0,0` to Flask even if new requests come in
//     if (resetTriggered) {
//         requests = 0;
//         errors = 0;

//         // ✅ Reset Mode Lasts for 10 Sec Before Allowing Updates Again
//         if (millis() - resetStartTime >= 20000) {
//             Serial.println(" Reset Period Over! Resuming Normal Counting...");
//             resetTriggered = false;
//         }
//     }

//     //  Send data to Flask every 2 seconds
//     if (millis() - lastSendTime >= 2000) {
//         sendToFlask();
//         lastSendTime = millis();
//     }
// }


//test 3
// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WiFiServer.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>

// // WiFi Credentials
// const char* ssid = "POCO";
// const char* password = "pritam67";

// // Flask Server Address
// const char* serverAddress = "http://192.168.4.100:5000/predict";

// // Static IP Configuration (optional)
// IPAddress local_IP(192, 168, 4, 110);
// IPAddress gateway(192, 168, 4, 1);
// IPAddress subnet(255, 255, 0, 0);

// // WiFi Server on Port 80 (ESP32 as a listener)
// WiFiServer server(80);

// void connectToWiFi() {
//     WiFi.config(local_IP, gateway, subnet);
//     WiFi.begin(ssid, password);

//     Serial.print("Connecting to WiFi");
//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print(".");
//         delay(1000);
//     }
//     Serial.println("\n✅ WiFi Connected!");
//     Serial.print("📡 ESP32 Static IP: ");
//     Serial.println(WiFi.localIP());

//     server.begin(); // Start the ESP32 Server
//     Serial.println("🌐 Server Listening on Port 80...");
// }

// // Function to send data to Flask
// void sendToFlask(int requests, int errors, String senderIP) {
//     HTTPClient http;
//     http.begin(serverAddress);
//     http.addHeader("Content-Type", "application/json");

//     // Create JSON Payload
//     String jsonPayload = "{\"ip\": \"" + senderIP + "\", \"requests\": " + String(requests) + ", \"errors\": " + String(errors) + "}";

//     Serial.println("📡 Forwarding to Flask: " + jsonPayload);

//     // Send POST Request
//     int httpResponseCode = http.POST(jsonPayload);
//     String response = http.getString();

//     Serial.print("🔄 HTTP Response Code: ");
//     Serial.println(httpResponseCode);
//     Serial.println("📨 Server Response: " + response);

//     http.end();
// }

// // Function to handle incoming client requests
// void handleClientRequest() {
//     WiFiClient client = server.available(); // Check if a client has connected
//     if (!client) return;

//     Serial.println("\n🌍 New Client Connected!");

//     // Wait for request data
//     while (!client.available()) {
//         delay(10);
//     }

//     // Read the request
//     String request = client.readStringUntil('\r');
//     client.flush(); // Clear any remaining data

//     Serial.println("📩 Received Request: " + request);

//     // Extract sender IP
//     String senderIP = client.remoteIP().toString();
//     Serial.println("🌍 Sender IP: " + senderIP);

//     // Parse JSON data
//     DynamicJsonDocument doc(256);
//     DeserializationError error = deserializeJson(doc, request);

//     if (error) {
//         Serial.println("❌ JSON Parsing Failed!");
//         return;
//     }

//     int requests = doc["requests"];
//     int errors = doc["errors"];

//     Serial.println("📌 Requests: " + String(requests));
//     Serial.println("❌ Errors: " + String(errors));

//     // Forward to Flask
//     sendToFlask(requests, errors, senderIP);

//     // Respond to the client
//     client.println("HTTP/1.1 200 OK");
//     client.println("Content-Type: text/plain");
//     client.println("Connection: close");
//     client.println();
//     client.println("✅ Data Received Successfully!");
//     client.stop();
// }

// void setup() {
//     Serial.begin(115200);
//     connectToWiFi();
// }

// void loop() {
//     handleClientRequest();
// }

//test 4
// #include <WiFi.h>
// #include <HTTPClient.h>
// #include "esp_wifi.h"

// // 📡 Mobile Hotspot Credentials (ESP32 connects to this)
// const char* ssid = "POCO";  
// const char* password = "pritam67";  

// // 📡 ESP32 SoftAP Credentials (ESP32 creates this network)
// const char* apSSID = "ESP32_Honeypot";  
// const char* apPassword = "secure123";  

// // 🌍 Flask Server Address
// const char* serverAddress = "http://192.168.4.100:5000/predict";

// // Function to Connect ESP32 to Mobile WiFi
// void connectToWiFi() {
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//     }
// }

// // Function to Start ESP32 as WiFi Access Point
// void startAccessPoint() {
//     WiFi.softAP(apSSID, apPassword);
// }

// // Function to Send Data to Flask
// void sendToFlask(int requests, int errors) {
//     HTTPClient http;
//     http.begin(serverAddress);
//     http.addHeader("Content-Type", "application/json");

//     // ✅ Only the required JSON format
//     String jsonPayload = "{\"requests\": " + String(requests) + ", \"errors\": " + String(errors) + "}";

//     http.POST(jsonPayload);
//     http.end();
// }

// void setup() {
//     Serial.begin(115200);
//     connectToWiFi();   // Connect to mobile hotspot
//     startAccessPoint(); // Create ESP32 WiFi
// }

// void loop() {
//     int requests = WiFi.softAPgetStationNum(); // Devices connected to ESP32 WiFi
//     int errors = random(1, 10); // Simulated error count

//     sendToFlask(requests, errors); // Send to Flask
//     delay(5000);
// }

//test 5
// #include <WiFi.h>
// #include <HTTPClient.h>
// #include "esp_wifi.h"

// // 📡 Mobile Hotspot Credentials (ESP32 connects to this)
// const char* ssid = "POCO";  
// const char* password = "pritam67";  

// // 📡 ESP32 SoftAP Credentials (ESP32 creates this network)
// const char* apSSID = "ESP32_Honeypot";  
// const char* apPassword = "secure123";  

// // 🌍 Flask Server Address
// const char* serverAddress = "http://192.168.4.100:5000/predict";

// // 📌 Static IP Configuration
// IPAddress local_IP(192, 168, 4, 110);
// IPAddress gateway(192, 168, 4, 1);
// IPAddress subnet(255, 255, 0, 0);

// // Function to Connect ESP32 to Mobile WiFi
// void connectToWiFi() {
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//     }
// }

// // Function to Start ESP32 as WiFi Access Point with Static IP
// void startAccessPoint() {
//     WiFi.softAP(apSSID, apPassword);
//     WiFi.softAPConfig(local_IP, gateway, subnet);
// }

// // Function to Send Data to Flask
// void sendToFlask(int requests, int errors) {
//     HTTPClient http;
//     http.begin(serverAddress);
//     http.addHeader("Content-Type", "application/json");

//     // ✅ Only the required JSON format
//     String jsonPayload = "{\"requests\": " + String(requests) + ", \"errors\": " + String(errors) + "}";

//     http.POST(jsonPayload);
//     http.end();
// }

// void setup() {
//     Serial.begin(115200);
//     connectToWiFi();   // Connect to mobile hotspot
//     startAccessPoint(); // Create ESP32 WiFi
// }

// void loop() {
//     int requests = WiFi.softAPgetStationNum(); // Devices connected to ESP32 WiFi
//     int errors = random(1, 10); // Simulated error count

//     sendToFlask(requests, errors); // Send to Flask
//     delay(5000);
// }


// #include <WiFi.h>
// #include <HTTPClient.h>
// #include "esp_wifi.h"

// // 📡 Mobile Hotspot Credentials (ESP32 connects to this)
// const char* ssid = "POCO";  
// const char* password = "pritam67";  

// // 📡 ESP32 SoftAP Credentials (ESP32 creates this network)
// const char* apSSID = "ESP32_Honeypot";  
// const char* apPassword = "secure123";  

// // 🌍 Flask Server Address
// const char* serverAddress = "http://192.168.4.100:5000/predict";

// // 📌 Static IP Configuration for SoftAP Mode
// IPAddress local_IP(192, 168, 4, 110);
// IPAddress gateway(192, 168, 4, 1);
// IPAddress subnet(255, 255, 0, 0);

// // 🔴 LED Pin (Attack Indicator)
// const int ledPin = 2;

// // 🌐 Global Variables
// unsigned long attackStartTime = 0;
// bool attackStarted = false;
// bool wifiConnected = false;

// // 📡 Connect ESP32 to Mobile WiFi (Station Mode)
// void connectToWiFi() {
//     WiFi.mode(WIFI_MODE_APSTA);  // Enable both STA & SoftAP
//     WiFi.begin(ssid, password);
    
//     Serial.print("Connecting to WiFi");
//     for (int i = 0; i < 15; i++) { // Try for 15 seconds
//         if (WiFi.status() == WL_CONNECTED) {
//             wifiConnected = true;
//             Serial.println("\n✅ WiFi Connected!");
//             Serial.print("📡 ESP32 STA IP: ");
//             Serial.println(WiFi.localIP());
//             return;
//         }
//         Serial.print(".");
//         delay(1000);
//     }
    
//     Serial.println("\n🚨 WiFi Connection Failed! Retrying...");
//     wifiConnected = false;
// }

// // 📡 Start ESP32 as WiFi Access Point (SoftAP Mode)
// void startAccessPoint() {
//     WiFi.softAP(apSSID, apPassword);
//     WiFi.softAPConfig(local_IP, gateway, subnet);
//     Serial.print("📡 SoftAP IP: ");
//     Serial.println(WiFi.softAPIP());
// }

// // 📨 Send Data to Flask Server
// void sendToFlask(int requests, int errors) {
//     if (!wifiConnected) {
//         Serial.println("🚨 WiFi Not Connected. Skipping request...");
//         return;
//     }

//     HTTPClient http;
//     http.begin(serverAddress);
//     http.addHeader("Content-Type", "application/json");

//     // ✅ JSON format
//     String jsonPayload = "{\"requests\": " + String(requests) + ", \"errors\": " + String(errors) + "}";
//     Serial.println("📡 Sending Data to Flask: " + jsonPayload);

//     int httpResponseCode = http.POST(jsonPayload);
//     String response = http.getString();

//     Serial.print("HTTP Response Code: ");
//     Serial.println(httpResponseCode);
//     Serial.println("📨 Server Response: " + response);

//     // 🚨 If an attack is detected, blink LED
//     if (response.indexOf("\"attack\": 1") >= 0) {
//         Serial.println("🚨 DDoS Attack Detected! Blinking LED...");
//         for (int i = 0; i < 5; i++) {
//             digitalWrite(ledPin, HIGH);
//             delay(200);
//             digitalWrite(ledPin, LOW);
//             delay(200);
//         }
//     } else {
//         Serial.println("✅ No Attack. Keeping LED ON...");
//         digitalWrite(ledPin, HIGH);
//     }

//     http.end();
// }

// // 📌 List Connected Clients in SoftAP Mode
// void listConnectedClients() {
//     wifi_sta_list_t wifi_sta_list;
//     esp_netif_sta_list_t netif_sta_list;

//     // Get the list of connected stations (clients)
//     esp_wifi_ap_get_sta_list(&wifi_sta_list);
//     esp_netif_get_sta_list(&wifi_sta_list, &netif_sta_list);

//     Serial.println("\n📋 Connected Clients in SoftAP Mode:");
//     for (int i = 0; i < netif_sta_list.num; i++) {
//         esp_netif_sta_info_t station = netif_sta_list.sta[i];
//         Serial.printf("📍 Client %d MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", i + 1,
//                       station.mac[0], station.mac[1], station.mac[2],
//                       station.mac[3], station.mac[4], station.mac[5]);
//     }
// }


// void setup() {
//     Serial.begin(115200);
//     pinMode(ledPin, OUTPUT);

//     // Ensure LED stays ON at startup
//     digitalWrite(ledPin, HIGH);

//     // 📡 Connect to WiFi (STA Mode)
//     connectToWiFi();

//     // 📡 Start SoftAP Mode
//     startAccessPoint();

//     // Record start time for attack delay
//     attackStartTime = millis();
// }

// void loop() {
//     // 📡 Reconnect WiFi if disconnected
//     if (WiFi.status() != WL_CONNECTED) {
//         Serial.println("🚨 WiFi Disconnected! Reconnecting...");
//         wifiConnected = false;
//         connectToWiFi();
//     }

//     // ⏳ Wait 10 seconds before starting attack simulation
//     if (!attackStarted && millis() - attackStartTime < 10000) {
//         Serial.println("⏳ Waiting 10 seconds before starting attack simulation...");
//         delay(1000);
//         return;
//     }

//     // 🔥 Start attack simulation
//     attackStarted = true;
//     int requests = random(800, 1500);
//     int errors = random(50, 200);

//     // 📨 Send attack data to Flask
//     sendToFlask(requests, errors);

//     // 📋 List connected clients in SoftAP mode
//     listConnectedClients();

//     delay(5000); // Wait before next request
// }

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
const char* serverAddress = "http://192.168.4.100:5000/predict";

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

