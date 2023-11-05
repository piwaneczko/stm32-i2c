#include "app.h"

#include <main.h>

#include <cassert>

#include "log.h"

static Log log{Log::Level::Debug, "APP"};
static App app;

extern TIM_HandleTypeDef htim2;
extern I2C_HandleTypeDef hi2c1;

void AppInit() {
    app.init();
}

void AppMainRoutineProcess() {
    app.process();
}

void AppErrorHandler() {
    app.error();
}

/**
 * @brief Period elapsed callback in non-blocking mode
 *
 * @param htim TIM handle
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    app.updateTimer_.update(htim);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
    app.baroSci_.receiveCompleted(hi2c);
}
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c) {
    app.baroSci_.transmitCompleted(hi2c);
}

static uint32_t i2cError = 0;
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c) {
    log.error("!! I2C ERROR %d !!", ++i2cError);
    // Restart I2C
    HAL_I2C_DeInit(hi2c);
    HAL_I2C_Init(hi2c);
}

App::App()
    : baroSci_(&hi2c1, ICP10125::DEVICE_ID),
      baro_(baroSci_),
      updateTimer_(&htim2, 100, 10),
      leds_{
          {LD2_GPIO_Port, LD2_Pin},
          {LD3_GPIO_Port, LD3_Pin},
      } {
    updateTimer_.timerElapsed = [&](const Timer& t) { timerElapsed(t); };
}

void App::init() {
    baro_.init([&](const SCI& sci) {
        leds_[0].toggle();
        leds_[1].toggle();
    });

    HAL_TIM_Base_Start_IT(&htim2);

    if (baroSci_.initialized) {
        log.info("initialized");
        leds_[0].toggle();
    }
}

void App::process() {
    HAL_Delay(100);
}

void App::error() {
    log.error("!!! Error occured !!!");
}

void App::timerElapsed(const Timer& timer) {
    if (&timer == &updateTimer_) {
        baro_.receiveRawData();
    }
}
