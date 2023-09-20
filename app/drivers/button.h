#ifndef IG_GPIO_INPUT_H
#define IG_GPIO_INPUT_H

#include <functional>

#include "gpio.h"

using std::function;

class Button : public GPIO {
   public:
    using StateChangedEventHandler = function<void(const Button&, bool)>;
    StateChangedEventHandler stateChanged; /* Function callback for state changed event handling */

   private:
    enum class State { Off, DebouncingOn, On, DebouncingOff } state_ = State::Off;
    bool normalOpened_;

   public:
    Button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, bool normalOpened = true);

    bool state() const override;

    void debouncingStateUpdate();
};

#endif  // IG_GPIO_INPUT_H