#ifndef IG_ICP10125_H
#define IG_ICP10125_H

#include "i2c.h"

class ICP10125 {
   private:
    float sensorConstants[4];  // OTP value
   public:
    using SCI_TYPE = I2C;
    ICP10125(SCI& sci);

    const static uint8_t DEVICE_ID = 0x63;

    void init(SCI::CompletedEventHandler completedEvent);
    bool receiveRawData(Asynchronic async);
    void reset();

    SCI& sci;
    uint8_t rawData[1] = {0x00};
};

#endif  // IG_ICP10125_H