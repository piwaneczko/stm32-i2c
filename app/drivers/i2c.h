#ifndef IG_I2C_H
#define IG_I2C_H

#include "main.h"
#include "sci.h"

class I2C : public SCI {
   private:
    I2C_HandleTypeDef* i2c_ = nullptr;
    uint8_t devAddress_ = 0;

   public:
    static HAL_StatusTypeDef lastError;

    I2C(I2C_HandleTypeDef* i2c, uint8_t devAddress);

    void receiveCompleted(I2C_HandleTypeDef* i2c);
    void transmitCompleted(I2C_HandleTypeDef* i2c);

    void init() override;

    bool read(uint8_t* data, uint16_t len, Asynchronic async = Asynchronic::Blocking) const override;
    bool write(uint8_t* data, uint16_t len, Asynchronic async = Asynchronic::Blocking) const override;
};

#endif  // IG_I2C_H