#include "button.h"

#include "log.h"

static Log log{Log::Level::Off, "SW"};

/**
 * Creates GPIO Input object
 * @param GPIOx Where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
 *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
 * @param GPIO_Pin Specifies the pins to be used.
 * @param normalOpened Input is normally opened in default state (GPIO_RESET)
 */
Button::Button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, bool normalOpened)
    : GPIO(GPIOx, GPIO_Pin), normalOpened_(normalOpened) {}

bool Button::state() const {
    return state_ == State::On;
}

/**
 * @brief A method for updating input state from timer (i.e. 100Hz timer)
 *
 */
void Button::debouncingStateUpdate() {
    bool pinState = GPIO::state();
    if (!normalOpened_) pinState = !pinState;
    auto state = state_;
    switch (state) {
        case State::Off:
            if (pinState) state = State::DebouncingOn;
            break;
        case State::DebouncingOn:
            if (pinState) state = State::On;
            break;
        case State::On:
            if (!pinState) state = State::DebouncingOff;
            break;
        case State::DebouncingOff:
            if (!pinState) state = State::Off;
            break;
        default:
            state = State::Off;
            break;
    }
    if (state_ != state) {
        log.debug("state %d->%d, pin:%d", state_, state, pinState);
        state_ = state;
        if (stateChanged != nullptr && (state == State::On || state == State::Off)) {
            stateChanged(*this, Button::state());
        }
    }
}