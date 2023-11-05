#include "performanceTimer.h"

#include <main.h>

/**
 * @brief Gets the calculated performance timer frequency
 *
 * @return int Performance timer frequency
 */
float PerformanceTimer::freq() const {
    return int(1000 / avgDt_);
}

/**
 * @brief Updates performance timer with clock ticks
 *
 */
void PerformanceTimer::update() {
    auto t = HAL_GetTick();
    if (lastTime_ == 0) {
        lastTime_ = t;
        count_ = 0;
    } else {
        avgDt_ = ((avgDt_ * count_) + (t - lastTime_)) / (count_ + 1);
        count_++;
        dt_ = t - lastTime_;
        lastTime_ = t;
    }
    if (int(count_) > int(dt_ * 1000 * 60)) {
        count_ = 0;
        lastTime_ = 0;
    }
}

/**
 * @brief Gets the current dt in milliseconds
 *
 */
float PerformanceTimer::dt() const {
    return dt_;
}
