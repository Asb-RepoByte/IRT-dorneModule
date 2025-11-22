#include "LoRaReceiver.h"

LoRaReceiver::LoRaReceiver(uint8_t csPin, uint8_t rstPin, uint8_t irqPin)
: radio(csPin, irqPin) {}

bool LoRaReceiver::begin(float freqMHz) {
    if (!radio.init()) return false;

    radio.setFrequency(freqMHz);
    radio.setTxPower(20, false);

    return true;
}

bool LoRaReceiver::available() {
    return radio.available();
}

LoRaPacket LoRaReceiver::readPacket() {
    LoRaPacket pkt;
   
    uint8_t buffer[LORA_PACKET_SIZE];
    uint8_t len = sizeof(buffer);

    if (!radio.recv(buffer, &len))
        return pkt;

    if (len < 2)
        return pkt;

    pkt.type = static_cast<SensorType>(buffer[0]);
    pkt.payloadLength = buffer[1];

    if (pkt.payloadLength > 29) 
        return pkt;

    memcpy(pkt.payload, &buffer[2], pkt.payloadLength);
    return pkt;
}
