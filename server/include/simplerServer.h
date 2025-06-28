#include <functional>
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

AsyncWebServer server(80);
WebSocketsServer webSocket(81);
const char* wpage;

class SimpleServer {  // FIXED: Class name consistency
private:
    

public:
    SimpleServer(const char* pg) {  // FIXED: Constructor name matches class
        wpage = pg;
    }

    void init(std::function<void(uint8_t num, WStype_t type, uint8_t * payload, size_t length)> f)
    {
        // Set up web server routes
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(200, "text/html", wpage);
        });
    
        // Start server
        server.begin();
        Serial.println("HTTP server started");
    
        // Start WebSocket server
        webSocket.begin();
        webSocket.onEvent(f);
        Serial.println("WebSocket server started on port 81");
    }

    void updateServer(StaticJsonDocument<100> doc) {
        // Create JSON message
        String message;
        serializeJson(doc, message);
    
        // Send to all connected clients
        webSocket.broadcastTXT(message);
    }

    void eventHandler()
    {
        webSocket.loop();
    }
};