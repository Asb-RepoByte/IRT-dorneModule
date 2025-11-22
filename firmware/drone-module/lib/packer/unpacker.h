#pragma once

#include "bmp280.h"
#include "protocol_common.h"


void unpackBmp280(const Packet &packet, BmpData &bmpData);