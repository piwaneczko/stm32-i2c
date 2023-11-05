#ifndef IG_ICP10125_H
#define IG_ICP10125_H

#include "i2c.h"

class ICP10125 {
   private:
    float sensorConstants_[4];  // OTP values
    const float paCalib_[3] = {45000, 80000, 105000};
    const float lutLower_ = 3.5 * (1 << 20);
    const float lutUpper_ = 11.5 * (1 << 20);

    float temp_;
    float pressure_;

    SCI::CompletedEventHandler receiveEventHandler_;
    void receiveDataCompleted(const SCI& sci);
    bool processData();

    enum MeasurementState {
        Idle,
        Measuring,
        Processing,
        Error,
    } state_ = Idle;

   public:
    using SCI_TYPE = I2C;
    ICP10125(SCI& sci);

    const static uint8_t DEVICE_ID = 0xC6;

    void init(SCI::CompletedEventHandler completedEvent);
    void receiveRawData(Asynchronic async);
    void reset();

    bool isConnected() const;

    SCI& sci;
    uint8_t rawData[9] = {0x00};
};

#endif  // IG_ICP10125_H