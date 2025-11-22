#pragma once
#include <string.h>

#include "bmp280.h"
#include "protocol_common.h"


Packet createPacket(
    PacketType type,
    uint8_t packetID,
    uint8_t chunkID,
    uint8_t chunkTotal,
    const uint8_t* chunkData,
    uint8_t chunkSize
);

template<typename T>
std::vector<Packet> packGeneric(
    PacketType type,
    const T* data,
    uint16_t size
);


bool waitAck(uint8_t expectedPacketID,
             uint8_t expectedChunkID,
             uint32_t timeoutMs);

int recvAckFunc(uint8_t* buffer, uint8_t maxLen);

void sendLoRaPacket(const Packet& pkt);

bool sendPacketsWithAck(
    const std::vector<Packet>& packets,
    void (*sendFunc)(const Packet&),
    bool (*waitAckFunc)(uint8_t packetID, uint8_t chunkID, uint32_t timeoutMs)
);


uint16_t crc16_ccitt(const uint8_t *data, uint16_t length);