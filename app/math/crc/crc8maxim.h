#ifndef IG_CRC8MAXIM_H
#define IG_CRC8MAXIM_H

#include "crc.h"

class Crc8Maxim : public Crc<uint8_t> {
   protected:
    uint8_t update(uint8_t crc, uint8_t data) override;
};

#endif  // IG_CRC8MAXIM_H