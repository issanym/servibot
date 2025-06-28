/** @file immediateEsp.h
 * 
 * @brief Lazy man's esp now wrapper. 
 *
 * @par       
 * COPYRIGHT NOTICE: (c) 2025 Bonobo Industries.  All rights reserved.
 */ 

#ifndef IMMEDIATEESP_H
#define IMMEDIATEESP_H

#include <functional>
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "immediateData.h"

/*
Usage Instructions:
- declare data using immediateData
- declare sender obj with mac address array
- init sender object
- send data from immediateData
*/

class EspSender {
private:
    esp_now_peer_info_t peerInfo;
    uint8_t rcvMac[6];  // FIXED: Changed from single uint8_t to array of 6 bytes

    // FIXED: Made callback static to work with ESP-NOW C API
    static void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) 
    {
        Serial.print("\r\nLast Packet Send Status:\t");
        Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    }

public:
    // FIXED: Constructor now takes array of 6 bytes instead of single byte
    EspSender(const uint8_t addr[6]) {
        memcpy(rcvMac, addr, 6);  // Copy all 6 bytes of MAC address
    }

    void init()
    {
        // Set ESP32 as a Wi-Fi Station
        WiFi.mode(WIFI_STA);
        
        // Initialize ESP-NOW (fixed typo)
        if (esp_now_init() != ESP_OK) {
            Serial.println("Error initializing ESP-NOW");
            return;
        }
        
        // Register the send callback
        esp_now_register_send_cb(OnDataSent);
        
        // Register peer
        memcpy(peerInfo.peer_addr, rcvMac, 6);
        peerInfo.channel = 0;  
        peerInfo.encrypt = false;
        
        // Add peer        
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            Serial.println("Failed to add peer");
            return;
        }
        else{
            Serial.println("Sender initialized");
        }
    }

    void dataSend(transferData obj)
    {
        // FIXED: Changed rcvMac to peerInfo.peer_addr for consistency
        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(peerInfo.peer_addr, (uint8_t *) &obj, sizeof(obj));
        
        if (result == ESP_OK) {
            Serial.println("Sending confirmed");
        }
        else {
            Serial.println("Sending error");
        }
    }
};

/*
Usage Instructions:
- declare data using immediateData
- declare receiver obj
- define a user onReceive function 
- init receiver object
- set callback with onReceive by passing the user function
*/


class EspReceiver {  // FIXED: Class name consistency
private:
 transferData *dt;
static EspReceiver* instance;  // Static pointer to current instance
    
    static void dataReceive(const uint8_t* mac, const uint8_t* incomingData, int len) {
        if (instance && instance->dt) {
            memcpy(instance->dt, incomingData, sizeof(*(instance->dt)));
        }
    }
   

public:
    EspReceiver(transferData& obj) {  // FIXED: Constructor name matches class
        instance = this;
        dt = &obj;
    }

    void init()
    {
        // Set ESP32 as a Wi-Fi Station
        WiFi.mode(WIFI_STA);
 
        // Initialize ESP-NOW (fixed typo)
        if (esp_now_init() != ESP_OK) {
            Serial.println("Error initializing ESP-NOW");
            return;
        }
        
        // FIXED: Register callback here, after ESP-NOW is initialized
        esp_now_register_recv_cb(dataReceive);
        
        Serial.println("Receiver initialized successfully");
    }
};EspReceiver* EspReceiver::instance = nullptr;

// Example usage:
/*
void setup() {
    Serial.begin(115200);
    
    // For sender:
    uint8_t receiverMAC[] = {0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC}; // Replace with actual MAC
    EspSender sender(receiverMAC);
    sender.init();
    
    // For receiver:
    EspReceiver receiver;
    receiver.onReceive([](const uint8_t* mac, const uint8_t* data, int len) {
        transferData* received = (transferData*)data;
        Serial.println("Data received!");
        // Process your received data here
    });
    receiver.init();
}
*/

#endif /* IMMEDIATEESP_H */

/*** end of file ***/
