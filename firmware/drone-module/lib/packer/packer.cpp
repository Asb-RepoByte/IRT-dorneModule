#include "packer.h"
#include <string.h>
#include<vector>
#include <RH_RF95.h>


void packBmp280(Packet &packet, const BmpData *bmpData) {
    packet.packetType = PacketType::TYPE_BMP280;
    packet.packetID = msg_id_counter++;
    packet.packetSize = sizeof(bmpData);
    packet.chunckID = 0;
    packet.chunckTotal = 1;
    memcpy(packet.payload, bmpData, sizeof(*bmpData));
}

template<typename T>
std::vector<Packet> packGeneric(PacketType type, const T &data)
{
    std::vector<Packet> packets;

    const uint8_t* raw = reinterpret_cast<const uint8_t*>(&data);
    uint16_t totalSize = sizeof(T);

    uint8_t totalChunks = (totalSize + 26) / 27;  // ceil division

    uint8_t msgID = msg_id_counter++;

    for (uint8_t i = 0; i < totalChunks; i++)
    {
        Packet p;
        p.packetType = type;
        p.packetID = msgID;
        p.packetSize = totalSize;
        p.chunkID = i;
        p.chunkTotal = totalChunks;

        uint8_t chunkStart = i * 27;
        uint8_t chunkSize = min((uint16_t)27, (uint16_t)(totalSize - chunkStart));

        memcpy(p.payload, raw + chunkStart, chunkSize);

        packets.push_back(p);
    }

    return packets;
}



bool sendPacketsWithAck(RH_RF95 &radio, const std::vector<Packet> &packets) {
    const uint8_t MAX_RETRIES = 5;
    const uint16_t ACK_TIMEOUT_MS = 1000;

    for (const Packet &pkt : packets) {

        bool ackReceived = false;

        for (uint8_t attempt = 0; attempt < MAX_RETRIES; attempt++) {

           
            radio.send((uint8_t*)&pkt, sizeof(Packet));
            radio.waitPacketSent();

            unsigned long start = millis();
            while (millis() - start < ACK_TIMEOUT_MS) {

                if (radio.available()) {
                    uint8_t buf[10];
                    uint8_t len = sizeof(buf);

                    if (radio.recv(buf, &len)) {
                        if (len == 1 && buf[0] == pkt.chunckID) {
                            ackReceived = true;
                            break;
                        }
                    }
                }
            }

            if (ackReceived)
                break;

            Serial.print("Retrying packet ");
            Serial.println(pkt.chunckID);
        }

        if (!ackReceived) {
            Serial.print("Failed to send packet ");
            Serial.println(pkt.chunckID);
            return false;
        }
    }

    return true;
}
