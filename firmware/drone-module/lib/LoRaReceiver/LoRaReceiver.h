#pragma once


#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>   

#define LORA_PAYLOAD_MAX 32 
#define LORA_PACKET_OVERHEAD 3 
#define LORA_PACKET_SIZE (LORA_PAYLOAD_MAX-LORA_PACKET_OVERHEAD)

enum SensorType : uint8_t {
    SENSOR_THP = 1,   // temp, humidity, pressure
    SENSOR_GPS = 2,   // lat, lon, alt
};



struct LoRaPacket {
    SensorType type;
    uint8_t payloadLength;
    uint8_t crc;
    uint8_t payload[LORA_PACKET_SIZE];
};  


class LoRaReceiver {
public:
    LoRaReceiver(uint8_t csPin, uint8_t rstPin, uint8_t irqPin);

    bool begin(float freqMHz = 868.0);
    bool available();
    LoRaPacket readPacket();

private:
    RH_RF95 radio;
};


