#include "packer.h"
#include <string.h>

void packBmp280(Packet &packet, const BmpData *bmpData) {
    packet.packetType = PacketType::TYPE_BMP280;
    packet.packetID = msg_id_counter++;
    packet.packetSize = sizeof(bmpData);
    packet.chunckID = 0;
    packet.chunckTotal = 1;
    memcpy(packet.payload, bmpData, sizeof(*bmpData));
}

void unpackBmp280(const Packet &packet, BmpData &bmpData) {
    float temp, pressure, altitude;
    memcpy(&temp, packet.payload, sizeof(temp));
    memcpy(&pressure, packet.payload + sizeof(temp), sizeof(pressure));
    memcpy(&altitude, packet.payload + sizeof(temp) + sizeof(pressure), sizeof(altitude));
    bmpData.temperature = temp;
    bmpData.pressure = pressure;
    bmpData.altitude = altitude;
}