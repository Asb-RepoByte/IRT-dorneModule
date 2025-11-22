#include "unpacker.h"
#include <string.h>
#include <RH_RF95.h>
#include "packer.h"


extern RH_RF95 rf95;


bool validateCRC(const Packet& pkt) {
    uint8_t temp[5 + 25];
    memcpy(temp, &pkt.packetType, 5);
    memcpy(temp + 5, pkt.payload, pkt.packetSize);

    uint16_t calc = crc16_ccitt(temp, 5 + pkt.packetSize);
    return calc == pkt.crc;
}

void sendAck(uint8_t packetID, uint8_t chunkID) {
    uint8_t ack[3];
    ack[0] = packetID;
    ack[1] = chunkID;
    ack[2] = crc16_ccitt(ack, 2);

    rf95.send(ack, 3);
    rf95.waitPacketSent();
}


bool receiveLoRaPacket(Packet& pkt) {
    if (!rf95.available()) return false;

    uint8_t buffer[32];
    uint8_t len = sizeof(buffer);

    if (!rf95.recv(buffer, &len)) return false;

    if (len < 7) return false; 

    pkt.packetType  = (PacketType)buffer[0];
    pkt.packetID    = buffer[1];
    pkt.packetSize  = buffer[2];
    pkt.chunckID     = buffer[3];
    pkt.chunckTotal  = buffer[4];

    memcpy(pkt.payload, buffer + 5, pkt.packetSize);

    pkt.crc = (buffer[5 + pkt.packetSize] << 8) |
               buffer[5 + pkt.packetSize + 1];

    if(validateCRC(pkt)) sendAck(pkt.packetID,pkt.chunckID);

    return true;
}


bool processIncoming(const Packet& pkt, AssemblyBuffer& buf) {
    if (!validateCRC(pkt)) return false;

    if (buf.chunks.empty()) {
        buf.type = pkt.packetType;
        buf.packetID = pkt.packetID;
        buf.chunkTotal = pkt.chunckTotal;
    }

    buf.chunks.push_back(pkt);

    if (buf.chunks.size() == buf.chunkTotal)
        buf.complete = true;

    return true;
}


bool reconstructMessage(const AssemblyBuffer& buffer, std::vector<uint8_t>& outData) {
    if (!buffer.complete) return false;

    outData.clear();

    for (uint8_t i = 0; i < buffer.chunkTotal; i++) {
        for (const Packet& p : buffer.chunks) {
            if (p.chunckID == i) {
                outData.insert(outData.end(), p.payload, p.payload + p.packetSize);
            }
        }
    }

    return true;
}
