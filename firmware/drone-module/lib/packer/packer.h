#pragma once

#include "bmp280.h"
#include "protocol_common.h"

void packBmp280(Packet &packet, const BmpData *bmpData);
void unpackBmp280(const Packet &packet, BmpData &bmpData);