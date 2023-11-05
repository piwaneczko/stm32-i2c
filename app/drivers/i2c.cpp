#include "i2c.h"

#include <string.h>

#include <cassert>

HAL_StatusTypeDef I2C::lastError = HAL_OK;

/**
 * @brief Construct a new I2C::I2C object
 *
 * @param i2c I2C handle
 * @param devAddress Device address
 */
I2C::I2C(I2C_HandleTypeDef* i2c, uint8_t devAddress) : i2c_(i2c), devAddress_(devAddress) {
    assert(i2c);
}

/**
 * @brief Receive completed handler, need to be called from HAL_I2C_MemRxCpltCallback
 *
 * @param i2c I2C handle
 */
void I2C::receiveCompleted(I2C_HandleTypeDef* i2c) {
    if (i2c_ == i2c && receiveCompletedEvent) receiveCompletedEvent(*this);
}

/**
 * @brief Transmit completed handler, need to be called from HAL_I2C_MemTxCpltCallback
 *
 * @param i2c I2C handle
 */
void I2C::transmitCompleted(I2C_HandleTypeDef* i2c) {
    if (i2c_ == i2c && transmitCompletedEvent) transmitCompletedEvent(*this);
}

/**
 * @brief I2C additional initialization
 *
 */
void I2C::init() {}

/**
 * @brief Read method which calls HAL_I2C_Mem_Read_? depending on async type
 *
 * @param data Input/Output data buffer. First byte shall be set with register address and will be modified with MSB
 * @param len Data buffers length
 * @param async Asynchronic method
 * @return true HAL_OK
 * @return false !HAL_OK
 */
bool I2C::read(uint8_t* data, uint16_t len, Asynchronic async) const {
    switch (async) {
        case Asynchronic::Blocking:
            lastError = HAL_I2C_Master_Receive(i2c_, (uint16_t)devAddress_, data, len, HAL_MAX_DELAY);
            if (receiveCompletedEvent) receiveCompletedEvent(*this);
            break;
        case Asynchronic::DMA:
            lastError = HAL_I2C_Master_Receive_DMA(i2c_, (uint16_t)devAddress_, data, len);
            break;
        case Asynchronic::IT:
            lastError = HAL_I2C_Master_Receive_IT(i2c_, (uint16_t)devAddress_, data, len);
            break;
        default:
            break;
    }
    return lastError == HAL_OK;
}

/**
 * @brief Write method which calls HAL_I2C_Mem_Write_? depending on async type
 *
 * @param data Write data buffer. First byte shall be set with register address
 * @param len Data buffers length
 * @param async Asynchronic method
 * @return true HAL_OK
 * @return false !HAL_OK
 */
bool I2C::write(uint8_t* data, uint16_t len, Asynchronic async) const {
    assert(len > 1);
    switch (async) {
        case Asynchronic::Blocking:
            lastError = HAL_I2C_Master_Transmit(i2c_, (uint16_t)devAddress_, data, len, HAL_MAX_DELAY);
            break;
        case Asynchronic::DMA:
            lastError = HAL_I2C_Master_Transmit_DMA(i2c_, (uint16_t)devAddress_, data, len);
            break;
        case Asynchronic::IT:
            lastError = HAL_I2C_Master_Transmit_IT(i2c_, (uint16_t)devAddress_, data, len);
            break;
        default:
            break;
    }
    return lastError == HAL_OK;
}

bool I2C::read(uint8_t reg, uint8_t& value) const {
    return HAL_I2C_Mem_Read(i2c_, devAddress_, reg | 0b10000000, 1, &value, 1, HAL_MAX_DELAY) == HAL_OK;
}

bool I2C::write(uint8_t reg, uint8_t value) const {
    return HAL_I2C_Mem_Write(i2c_, devAddress_, reg, 1, &value, 1, HAL_MAX_DELAY) == HAL_OK;
}
