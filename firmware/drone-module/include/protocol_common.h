#pragma once
#include <inttypes.h>

enum PacketType: uint8_t {
    TYPE_BMP280 = 0,
    // sensor from 0 to 127 theoratical max for I2c bus
    TYPE_ACK = 128,
    TYPE_FIN = 129,
    TYPE_FIN_ACK = 130,
    TYPE_FIN_NACK = 131
};

struct Packet {
    PacketType packetType;
    uint8_t packetID;
    uint8_t packetSize;
    uint8_t chunckID;
    uint8_t chunckTotal;
    uint8_t payload[25]; 
    uint16_t crc;
};


struct AssemblyBuffer {
    PacketType type;          
    uint8_t packetID;         
    uint8_t chunkTotal;       
    bool complete = false;    

    std::vector<Packet> chunks; 
};


extern uint8_t msg_id_counter;