#include "packer.h"
#include <string.h>
#include<vector>
#include <RH_RF95.h>

extern RH_RF95 rf95;

uint8_t PACKET_ID_COUNTER = 1;

const uint8_t   MAX_CHUNCK_SIZE= 25;
const uint8_t HEADER_SIZE= 5;



Packet createPacket(
    PacketType type,
    uint8_t packetID,
    uint8_t chunkID,
    uint8_t chunkTotal,
    const uint8_t* chunkData,
    uint8_t chunkSize
) {
    Packet pkt;
    pkt.packetType = type;
    pkt.packetID   = packetID;
    pkt.packetSize = chunkSize;
    pkt.chunckID    = chunkID;
    pkt.chunckTotal = chunkTotal;

    memcpy(pkt.payload, chunkData, chunkSize);

    uint8_t temp[HEADER_SIZE + MAX_CHUNCK_SIZE]; 
    memcpy(temp, &pkt.packetType, HEADER_SIZE);
    memcpy(temp + HEADER_SIZE, pkt.payload, chunkSize);

    pkt.crc = crc16_ccitt(temp, HEADER_SIZE + chunkSize);

    return pkt;

}


template<typename T>
std::vector<Packet> packGeneric(
    PacketType type,
    const T* data,
    uint16_t size
) {
    std::vector<Packet> packets;
    const uint8_t* raw = (const uint8_t*)data;

    uint8_t packetID = msg_id_counter++;

    const uint8_t CHUNK_SIZE = 25;
    uint8_t chunkTotal = (size + CHUNK_SIZE - 1) / CHUNK_SIZE;

    for (uint8_t chunkID = 0; chunkID < chunkTotal; chunkID++) {
        uint16_t start = chunkID * CHUNK_SIZE;
        uint8_t len = (start + CHUNK_SIZE <= size)
                        ? CHUNK_SIZE
                        : size - start;

        Packet pkt = createPacket(
            type,
            packetID,
            chunkID,
            chunkTotal,
            raw + start,   
            len
        );

        packets.push_back(pkt);
    }
    return packets;
}


void sendLoRaPacket(const Packet& pkt) {
    uint8_t buffer[32];

   
    memcpy(buffer, &pkt.packetType, 1);
    buffer[1] = pkt.packetID;
    buffer[2] = pkt.packetSize;
    buffer[3] = pkt.chunckID;
    buffer[4] = pkt.chunckTotal;

    memcpy(buffer + 5, pkt.payload, pkt.packetSize);

    
    buffer[5 + pkt.packetSize]     = pkt.crc >> 8;
    buffer[5 + pkt.packetSize + 1] = pkt.crc & 0xFF;

    uint8_t totalSize = 5 + pkt.packetSize + 2;

    rf95.send(buffer, totalSize);
    rf95.waitPacketSent();
}

int recvAckFunc(uint8_t* buffer, uint8_t maxLen) {
    if (rf95.available()) {
        uint8_t len = maxLen;
        if (rf95.recv(buffer, &len)) {
            return len;  // number of received bytes
        }
    }
    return 0; // no data received
}



bool waitAck(uint8_t expectedPacketID,
             uint8_t expectedChunkID,
             uint32_t timeoutMs)
{
    uint8_t buf[3];
    uint32_t start = millis();

    while (millis() - start < timeoutMs) {

        int len = recvAckFunc(buf, 3);

        if (len == 3) {
            if (crc16_ccitt(buf, 2) == buf[2] &&
                buf[0] == expectedPacketID &&
                buf[1] == expectedChunkID) 
            {
                return true; // ACK OK
            }
        }
    }
    return false; 
}



bool sendPacketsWithAck( const std::vector<Packet>& packets ) {
    for (const Packet& pkt : packets) {

        const uint8_t MAX_RETRIES = 5;
        bool acked = false;

        for (uint8_t retry = 0; retry < MAX_RETRIES; retry++) {
            sendLoRaPacket(pkt);

            if (waitAck(pkt.packetID, pkt.chunckID, 1000)) {
                acked = true;
                break;
            }
        }

        if (!acked) return false;
    }

    return true;
}



uint16_t crc16_ccitt(const uint8_t* data, uint16_t len) {
    uint16_t crc = 0xFFFF;
    
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            crc = (crc & 0x8000)
                ? (crc << 1) ^ 0x1021
                : (crc << 1);
        }
    }
    return crc;
}
