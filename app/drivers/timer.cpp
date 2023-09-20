#include "timer.h"

/**
 * @brief Construct a new Timer:: Timer object
 *
 * @param htim Handle to HAL timer structure - can be NULL
 * @param period Period in milliseconds
 * @param interval Interval of Core timer in milliseconds
 * @param autoStart Timer is created as is running without calling start method
 * @param autoReset Timer is automatically restarted after remaining time is elapsed
 */
Timer::Timer(TIM_HandleTypeDef* htim, uint32_t period, uint16_t interval, bool autoStart, bool autoReset)
    : tim_(htim),
      period_(period),
      interval_(interval),
      remainingTime_(period),
      autoReset_(autoReset),
      isRuning_(autoStart) {}

/**
 * @brief Updates internal timer time
 *
 * @param htim Handle to HAL timer structure
 */
void Timer::update(TIM_HandleTypeDef* htim) {
    if (isRuning_ && tim_ == htim) {
        if (period_ > interval_)
            remainingTime_ -= interval_;
        else
            remainingTime_ = 0;
        if (!remainingTime_) {
            if (autoReset_) remainingTime_ = period_;
            if (timerElapsed != nullptr) {
                timerElapsed(*this);
            }
        }
    }
}

/**
 * @brief A method to start the timer
 *
 */
void Timer::start() {
    isRuning_ = true;
}

/**
 * @brief A method to stop timer
 *
 */
void Timer::stop() {
    isRuning_ = false;
}

/**
 * @brief Stops, sets timer remaining time to 0 and re-starts timer
 *
 */
void Timer::restart() {
    stop();
    remainingTime_ = 0;
    start();
}

bool Timer::elapsed() const {
    return remainingTime_ == 0;
}
