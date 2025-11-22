#pragma once
#include <string.h>

#include "bmp280.h"
#include "protocol_common.h"

void packBmp280(Packet &packet, const BmpData *bmpData);
bool sendPacketsWithAck(RH_RF95 &radio, const std::vector<Packet> &packets);

template<typename T>
std::vector<Packet> packGeneric(PacketType type, const T &data);