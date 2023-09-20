#ifndef IG_GPIO_H
#define IG_GPIO_H

#include "main.h"

class GPIO {
   private:
    GPIO_TypeDef* port_ = {0};
    uint16_t pin_ = 0;

   public:
    GPIO(GPIO_TypeDef* port, uint16_t pin);
    virtual ~GPIO(){};
    virtual bool state() const;
    void update(bool state) const;
    void toggle() const;
};

#endif  // IG_GPIO_H