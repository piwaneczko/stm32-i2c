#ifndef IG_ADC_H
#define IG_ADC_H

#include "async.h"
#include "main.h"

class Adc {
   private:
    ADC_HandleTypeDef* adc_ = {0};
    uint8_t resolution_ = 8;
    float reference_ = 3.3f;

    float voltage_ = 0.0f;
    uint16_t rawAdc_ = 0;

   public:
    HAL_StatusTypeDef lastError;
    bool initialized = false;

    void receiveCompleted(ADC_HandleTypeDef* adc);

    Adc(ADC_HandleTypeDef* adc, uint8_t resolution, float reference = 3.3f);
    void init();
    bool update(Asynchronic async);
    float voltage() const;
};

#endif  // IG_ADC_H