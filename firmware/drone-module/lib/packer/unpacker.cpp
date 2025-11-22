#include "unpacker.h"
#include <string.h>



void unpackBmp280(const Packet &packet, BmpData &bmpData) {
    float temp, pressure, altitude;
    memcpy(&temp, packet.payload, sizeof(temp));
    memcpy(&pressure, packet.payload + sizeof(temp), sizeof(pressure));
    memcpy(&altitude, packet.payload + sizeof(temp) + sizeof(pressure), sizeof(altitude));
    bmpData.temperature = temp;
    bmpData.pressure = pressure;
    bmpData.altitude = altitude;
}