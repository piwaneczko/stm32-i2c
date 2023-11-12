#ifndef IG_APP_H
#define IG_APP_H

#include "adc.h"
#include "gpio.h"
#include "timer.h"

class App {
   private:
    friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);
    friend void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c);
    friend void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c);

    Adc adc_;
    Timer updateTimer_;

    GPIO leds_[2];

    void timerElapsed(const Timer& timer);

   public:
    App();

    void init();
    void process();
    void error();
};

#endif  // IG_APP_H