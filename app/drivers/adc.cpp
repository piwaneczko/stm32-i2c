#include "adc.h"

#include <cassert>

#define MEASUREMENT_TIMEOUT_MS 20

Adc::Adc(ADC_HandleTypeDef* adc, uint8_t resolution, float reference)
    : adc_(adc), resolution_(resolution), reference_(reference) {
    assert(adc);
    assert(resolution >= 6 && resolution <= 16 && resolution % 2 == 0);
}

void Adc::receiveCompleted(ADC_HandleTypeDef* adc) {
    if (adc_ == adc) {
        if (rawAdc_ == 0) {
            rawAdc_ = (uint16_t)HAL_ADC_GetValue(adc_);
        }
        voltage_ = rawAdc_ * reference_ / ((1 << resolution_) - 1);
    }
}

void Adc::init() {
    // Calibrate The ADC On Power-Up For Better Accuracy
    initialized = HAL_ADCEx_Calibration_Start(adc_, ADC_SINGLE_ENDED) == HAL_OK;
}

bool Adc::update(Asynchronic async) {
    switch (async) {
        case Asynchronic::Blocking:
            lastError = HAL_ADC_Start(adc_);
            if (lastError == HAL_OK) {
                lastError = HAL_ADC_PollForConversion(adc_, HAL_MAX_DELAY);
                if (lastError == HAL_OK) {
                    rawAdc_ = 0;
                    receiveCompleted(adc_);
                }
            }
            break;
        case Asynchronic::IT:
            rawAdc_ = 0;
            lastError = HAL_ADC_Start_IT(adc_);
            break;
        case Asynchronic::DMA:
            lastError = HAL_ADC_Start_DMA(adc_, (uint32_t*)&rawAdc_, sizeof(rawAdc_));
            break;
        default:
            lastError = HAL_ERROR;
            break;
    }
    return lastError == HAL_OK;
}

float Adc::voltage() const {
    return voltage_;
}
