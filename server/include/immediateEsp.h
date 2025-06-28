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
        // Set the device as a Station and Soft Access Point simultaneously
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
    static EspReceiver* instance; // FIXED: Matching class name
    std::function<void(const uint8_t*, const uint8_t *, int)> receive_callback;

    // Static bridge functions
    static void receive_bridge(const uint8_t * mac, const uint8_t *incomingData, int len) {
        if (instance && instance->receive_callback) {
            instance->receive_callback(mac, incomingData, len);
        }
    }

public:
    EspReceiver() {  // FIXED: Constructor name matches class
        instance = this;
        // FIXED: Moved registration to init() method for better control
    }
    
    /// @brief Sets the callback function for received data
    /// @param callback Function to call when data is received
    void onReceive(std::function<void(const uint8_t*, const uint8_t *, int)> callback) {
        receive_callback = callback;
    }

    void init()
    {
        // Set ESP32 as a Wi-Fi Station
        WiFi.mode(WIFI_AP_STA);
  
        // Set device as a Wi-Fi Station
        //WiFi.begin(, );
        //while (WiFi.status() != WL_CONNECTED) {
          //  delay(1000);
         //   Serial.println("Setting as a Wi-Fi Station..");
        //}
        WiFi.softAP("AutoServer", "12345678");
        IPAddress IP = WiFi.softAPIP();
        Serial.println(IP.toString());
 
        // Initialize ESP-NOW (fixed typo)
        if (esp_now_init() != ESP_OK) {
            Serial.println("Error initializing ESP-NOW");
            return;
        }
        
        // FIXED: Register callback here, after ESP-NOW is initialized
        esp_now_register_recv_cb(receive_bridge);
        
        Serial.println("Receiver initialized successfully");
    }
};

// FIXED: Static member declaration with correct syntax
EspReceiver* EspReceiver::instance = nullptr;

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
