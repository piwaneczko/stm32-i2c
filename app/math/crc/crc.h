#ifndef IG_CRC_H
#define IG_CRC_H

#include <cstdint>

template <typename T>
class Crc {
   protected:
    virtual T update(T crc, T data) = 0;

   public:
    virtual T calculate(T* data, uint16_t len, T crc) {
        for (uint16_t i = 0; i < len; i++) {
            crc = update(crc, data[i]);
        }
        return crc;
    }
};

#endif  // IG_CRC_H