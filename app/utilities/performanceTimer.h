#ifndef IG_PERFORMANCETIMER_H
#define IG_PERFORMANCETIMER_H

#include <stdint.h>

class PerformanceTimer {
    float dt_;
    float avgDt_;
    uint32_t count_;
    uint32_t lastTime_ = 0;

   public:
    void update();
    float freq() const;
    float dt() const;
};
#endif  // IG_PERFORMANCETIMER_H