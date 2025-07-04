#include <Arduino.h>
#include "immediateEsp.h"
#include "simplerServer.h"
#include "waiter.h"
#include "web.h"

// Important variables( must )
transferData myData;
EspReceiver receiver; // create a esp receiver
SimpleServer client(htmlPage);
waiter bot;

bool arrived=false;

// WiFi credentials

// WebSocket event handler i.e what to do when we recieve data from the dispatcher
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
            
      
      if (doc["type"] == "command") {
        
        Serial.println("Action: command");

        if (doc["command"] == "stop")
        {
          Serial.println("Command: Stop");
          bot.setPaused(true);
        }
        else if (doc["command"] == "resume")
        {
          Serial.println("Command: resume");
          if(bot.getID() == 1)
          {
            arrived = bot.GO_TO_TABLE1();
            Serial.println("Location: Table 1");
          }
          else if(bot.getID() == 2)
          {
            arrived = bot.GO_TO_TABLE2();
            Serial.println("Location: Table 2");
          }
          else if(bot.getID() == 3)
          {
            arrived = bot.GO_TO_TABLE3();
            Serial.println("Location: Table 3");
          }
          else if(bot.getID() == 0)
          {
            arrived = bot.RETURN_HOMEBASE();
            Serial.println("Location: Homebase");
          }
        }
        
      }
      else if(doc["type"] == "dispatch")
      {
        Serial.println("Action: dispatch");
        Serial.println(doc["tableId"].as<int>());

        if(doc["tableId"].as<int>() == 1)
        {
          Serial.println("Location: Table 1");
          arrived = bot.GO_TO_TABLE1();
          Serial.println("Location: Table 1");
        }
        else if(doc["tableId"].as<int>() == 2)
        {
          arrived = bot.GO_TO_TABLE2();
          Serial.println("Location: Table 2");
        }
        else if(doc["tableId"].as<int>() == 3)
        {
          arrived = bot.GO_TO_TABLE3();
          Serial.println("Location: Table 3");
        }
        else if(doc["tableId"].as<int>() == 0)
        {
          arrived = bot.RETURN_HOMEBASE();
          Serial.println("Location: Homebase");
        }
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

  bot.init();

  //pinMode(5,OUTPUT);
}

void loop() 
{
  // Handle WebSocket events
  client.eventHandler();

  if(arrived)
  {
    // send arrive msg
    // type:status
    // status:arrived
    // location:(ID!=0)?"table":"homebase"
    StaticJsonDocument<100> doc;
    doc["type"] = "status";
    doc["status"] = "arrived";
    doc["location"] = (bot.getID()!=0)?"table":"homebase";
    client.updateServer(doc);
    Serial.println("status:arrived");
    // send location; (ID!=0)?"table":"homebase"
    Serial.println("location:"+(bot.getID()!=0)?"table":"homebase");

    delay(30000);
    arrived = false;
    if(bot.getID()!=0)
    {
      // if arrived?
      // type:status
      // status:returning
      // location:"homebase"
      doc["type"] = "status";
      doc["status"] = "returning";
      doc["location"] = "homebase";
      client.updateServer(doc);
      Serial.println("status:returning");

      // go to home base
      arrived = bot.RETURN_HOMEBASE();
      //ID=0;
    }
  }
 
}