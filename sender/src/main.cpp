#include <Arduino.h>
#include <WiFi.h>
#include "immediateEsp.h"
//#include "immediateData.h"

// important variable
transferData myData; //data to be sent
uint8_t receiverMAC[] = {0xE4, 0x65, 0xB8, 0x75, 0x69, 0x18}; // Replace with actual MAC
EspSender sender(receiverMAC); // create a sender esp

const int push_button = 2;

void setup() 
{
  Serial.begin(115200);
  sender.init();
  //myData.x = false;

  pinMode(push_button, INPUT);
}

void loop() 
{
  int state = digitalRead(push_button);
delay(50); // Reduced delay for better responsiveness

if (state == HIGH) // Explicitly check for HIGH state
{
    myData.id = 1;
    myData.type = "request";
    Serial.println("Button pressed - sending data");
    sender.dataSend(myData); // sending data
    
    // Wait for button release to prevent multiple triggers
    while(digitalRead(push_button) == HIGH) {
        delay(10);
    }
    delay(50); // Small debounce delay
}
  
}