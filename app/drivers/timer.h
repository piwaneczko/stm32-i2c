#ifndef IG_TIMER_H
#define IG_TIMER_H

#include <functional>

#include "main.h"

using std::function;

class Timer {
    TIM_HandleTypeDef* tim_ = nullptr;
    const uint32_t period_ = 0;
    const uint32_t interval_ = 0;
    uint32_t remainingTime_ = 0;
    bool autoReset_ = true;
    bool isRuning_ = true;

   public:
    using TimerElapsedEventHandler = function<void(const Timer&)>;
    TimerElapsedEventHandler timerElapsed; /* Handler for timer elapsed event */

    Timer(TIM_HandleTypeDef* htim, uint32_t period, uint16_t interval, bool autoStart = true, bool autoReset = true);
    Timer& operator=(const Timer& rhs) = delete;

    void update(TIM_HandleTypeDef* htim);

    void start();
    void stop();
    void restart();
    bool elapsed() const;
};

#endif  // IG_TIMER_H