#pragma once

#include <stdint.h>
#include <vector>
#include "packer.h"

struct AssemblyBuffer {
    PacketType type;
    uint8_t packetID;
    uint8_t chunkTotal;
    std::vector<Packet> chunks;
    bool complete = false;
};


bool receiveLoRaPacket(Packet& pkt) ;

bool validateCRC(const Packet& pkt);

void sendAck(uint8_t packetID, uint8_t chunkID);

bool processIncoming(const Packet& pkt, AssemblyBuffer& buffer);

bool reconstructMessage(const AssemblyBuffer& buffer, std::vector<uint8_t>& outData);
