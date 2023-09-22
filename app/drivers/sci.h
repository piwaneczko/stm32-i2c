#ifndef IG_SERIAL_COMMUNICATION_INTERFACE_H
#define IG_SERIAL_COMMUNICATION_INTERFACE_H

#include <cstdint>
#include <functional>

enum class Asynchronic { Blocking, DMA, IT };

using std::function;

class SCI {
   public:
    bool initialized = false;

    using CompletedEventHandler = function<void(const SCI&)>;
    CompletedEventHandler receiveCompletedEvent;  /* Received completed event callback handler */
    CompletedEventHandler transmitCompletedEvent; /* Transmit completed event callback handler */

    virtual void init() = 0;

    virtual bool read(uint8_t* data, uint16_t len, Asynchronic async = Asynchronic::Blocking) const = 0;
    virtual bool write(uint8_t* data, uint16_t len, Asynchronic async = Asynchronic::Blocking) const = 0;

    virtual bool read(uint8_t reg, uint8_t& value) const = 0;
    virtual bool write(uint8_t reg, uint8_t value) const = 0;

    uint8_t read(uint8_t reg) const;
};

#endif  // IG_SERIAL_COMMUNICATION_INTERFACE_H