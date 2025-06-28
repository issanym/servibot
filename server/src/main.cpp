#include <Arduino.h>
#include "immediateEsp.h"
#include "simplerServer.h"
#include "web.h"

// Important variables( must )
transferData myData;
EspReceiver receiver; // create a esp receiver
SimpleServer client(htmlPage);
// WiFi credentials

// WebSocket event handler
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("Client [%u] Disconnected\n", num);
      break;
    case WStype_CONNECTED:
      Serial.printf("Client [%u] Connected\n", num);
      break;
    case WStype_TEXT:
      // Parse JSON message
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);
            
      if (error) {
        Serial.println("Failed to parse JSON");
        return;
      }
            
      // Handle toggle command
      if (doc["type"] == "command") {
        //toggleLEDState();
        Serial.println("WebSocket command");
      }
      else if(doc["type"] == "dispatch")
      {
        Serial.println("WebSocket dispatch");
      }
      break;
  }
}

void dataReceive(const uint8_t* mac, const uint8_t* incomingData, int len)
{
  memcpy(&myData, incomingData, sizeof(myData)); // copies received data to myData

  // Create JSON message0
    StaticJsonDocument<100> doc;
    doc["type"] = myData.type;
    doc["id"] = myData.id;

  client.updateServer(doc);
  Serial.println("Received and webed");
}



void setup() 
{
  Serial.begin(115200);
  
  receiver.onReceive(dataReceive);
  receiver.init(); // init the receiver(data is actually received here)

  // Initialize the server
  client.init(onWebSocketEvent);

  //pinMode(5,OUTPUT);
}

void loop() 
{
  // Handle WebSocket events
  client.eventHandler();

  // use the data here
  //digitalWrite(5,myData.x);
  //Serial.println(myData.x?"hello":"by");

  
 
}