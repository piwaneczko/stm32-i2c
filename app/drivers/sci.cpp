#include "sci.h"
/**
 * @brief Write/Read method which calls read(uint8_t, uint8_t&) in blocking mode
 *
 * @param reg Register address - will be modified with MSB 1
 * @return uint8_t Output value or 0xFF if failure
 */
uint8_t SCI::read(uint8_t reg) const {
    uint8_t value;
    read(reg, value);
    return value;
}