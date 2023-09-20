#include "crc8maxim.h"

#define CRC8POLY 0x31  // = X^8+X^5+X^4+X^0

uint8_t Crc8Maxim::update(uint8_t crc, uint8_t data) {
    crc = crc ^ data;

    for (uint8_t i = 0; i < 8; ++i) {
        if (crc & 0x80)
            crc = (crc << 1) ^ CRC8POLY;
        else
            crc <<= 1;
    }

    return crc;
}