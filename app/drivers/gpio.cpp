#include "gpio.h"

#include <assert.h>

/**
 * Creates GPIO led object
 * @param  port Where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
 *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
 * @param  pin Specifies the pins to be used.
 */
GPIO::GPIO(GPIO_TypeDef* port, uint16_t pin) : port_(port), pin_(pin) {
    assert(port_);
    assert(pin_);
}

/**
 * @brief A method to return gpio pin state
 *
 * @return true GPIO_PIN_SET
 * @return false GPIO_PIN_RESET
 */
bool GPIO::state() const {
    return HAL_GPIO_ReadPin(port_, pin_) == GPIO_PIN_SET;
}
/**
 * @brief A method for set the GPIO pin state
 *
 * @param state true  - GPIO_PIN_SET
 *              false - GPIO_PIN_RESET
 */
void GPIO::update(bool state) const {
    HAL_GPIO_WritePin(port_, pin_, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief A method for toggling GPIO pin state
 */
void GPIO::toggle() const {
    HAL_GPIO_TogglePin(port_, pin_);
}
