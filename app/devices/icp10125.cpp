#include "icp10125.h"

#include "crc8maxim.h"
#include "log.h"
#include "main.h"

#define CRC8_INIT 0xFF

using std::bind;
using std::placeholders::_1;

static Log log{Log::Level::Debug, "ICP10125"};

ICP10125::ICP10125(SCI& sci) : sci(sci) {}

uint8_t ICP10125_READ_ID[] = {0xEF, 0xC8};          /*!< Read sensor ID */
uint8_t ICP10125_SOFT_RESET[] = {0x80, 0x5D};       /*!< Perform soft reset */
uint8_t ICP10125_MEASURE_LP_MODE[] = {0x60, 0x9C};  /*!< Start measurement in low power mode (temp first) */
uint8_t ICP10125_MEASURE_N_MODE[] = {0x68, 0x25};   /*!< Start measurement in normal power mode (temp first) */
uint8_t ICP10125_MEASURE_LN_MODE[] = {0x70, 0xDF};  /*!< Start measurement in low noise power mode (temp first) */
uint8_t ICP10125_MEASURE_ULN_MODE[] = {0x78, 0x66}; /*!< Start measurement in ultra-low noise power mode (temp first) */
uint8_t ICP10125_SETUP_OTP_READ[] = {0xC5, 0x95, 0x00, 0x66, 0x9C};
uint8_t ICP10125_OTP_READ_NEXT[] = {0xC7, 0xF7};

bool ICP10125::isConnected() const {
    bool result = false;
    uint8_t buffer[2];
    if (sci.write(ICP10125_READ_ID, sizeof(ICP10125_READ_ID))) {
        if (sci.read(buffer, sizeof(buffer))) {
            uint16_t id = ((uint16_t)buffer[0] << 8) | buffer[1];
            result = (id & 0x03F) == 0x08;
        }
    }
    return result;
}

void ICP10125::init(SCI::CompletedEventHandler completedEvent) {
    receiveEventHandler_ = completedEvent;
    sci.receiveCompletedEvent = bind(&ICP10125::receiveDataCompleted, this, _1);
    sci.init();
    bool initialized = isConnected();

    // reset();

    Crc8Maxim crc;
    if (sci.write(ICP10125_SETUP_OTP_READ, sizeof(ICP10125_SETUP_OTP_READ))) {
        uint8_t buffer[3] = {0};
        for (uint8_t i = 0; i < 4; i++) {
            if (sci.write(ICP10125_OTP_READ_NEXT, sizeof(ICP10125_OTP_READ_NEXT))) {
                if (sci.read(buffer, sizeof(buffer))) {
                    if (crc.calculate(buffer, 2, CRC8_INIT) == buffer[2]) {
                        sensorConstants_[i] = (float)((int16_t)(((uint16_t)buffer[0] << 8) | buffer[1]));
                    } else {
                        log.error("CRC calculation failed");
                        initialized = false;
                    }
                } else {
                    log.error("Reading OTP failed");
                    initialized = false;
                }
            } else {
                log.error("Writing OTP_READ_NEXT failed");
                initialized = false;
            }
        }
    } else {
        log.error("Writing SETUP_OTP_READ failed");
        initialized = false;
    }
    sci.initialized = initialized;
    if (sci.initialized) {
        log.info("init successfull");
    }
}

void ICP10125::receiveRawData(Asynchronic async) {
    switch (state_) {
        case Idle:
            if (sci.write(ICP10125_MEASURE_N_MODE, sizeof(ICP10125_MEASURE_N_MODE))) {
                state_ = Measuring;
            } else {
                state_ = Error;
            }
            break;
        case Measuring:
            sci.read(rawData, sizeof(rawData), async);
            break;
        case Processing:
            if (processData() && receiveEventHandler_ != nullptr) {
                receiveEventHandler_(sci);
            }
            state_ = Idle;
            break;
        case Error:
            state_ = Idle;
            break;
    }
}

void ICP10125::receiveDataCompleted(const SCI& sci) {
    if (state_ == Measuring) state_ = Processing;
}

bool ICP10125::processData() {
    auto result = false;
    Crc8Maxim crc;
    if (crc.calculate(&rawData[0], 2, CRC8_INIT) == rawData[2] &&
        crc.calculate(&rawData[3], 2, CRC8_INIT) == rawData[5] &&
        crc.calculate(&rawData[6], 2, CRC8_INIT) == rawData[8]) {
        int16_t tdout = (int16_t)((((uint16_t)rawData[0]) << 8) | rawData[1]);
        int32_t pdout = (int32_t)((((uint32_t)rawData[3]) << 16) | ((uint16_t)rawData[4]) << 8 | rawData[6]);

        float t = (float)(tdout - 0x8000);
        float LUT[3] = {
            lutLower_ + (float)(sensorConstants_[0] * t * t) / 0x1000000,
            0x800 * sensorConstants_[3] + (float)(sensorConstants_[1] * t * t) / 0x1000000,
            lutUpper_ + (float)(sensorConstants_[2] * t * t) / 0x1000000,
        };
        float C = (LUT[0] * LUT[1] * (paCalib_[0] - paCalib_[1]) + LUT[1] * LUT[2] * (paCalib_[1] - paCalib_[2]) +
                   LUT[2] * LUT[0] * (paCalib_[2] - paCalib_[0])) /
                  (LUT[2] * (paCalib_[0] - paCalib_[1]) + LUT[0] * (paCalib_[1] - paCalib_[2]) +
                   LUT[1] * (paCalib_[2] - paCalib_[0]));
        float A = (paCalib_[0] * LUT[0] - paCalib_[1] * LUT[1] - (paCalib_[1] - paCalib_[0]) * C) / (LUT[0] - LUT[1]);
        float B = (paCalib_[0] - A) * (LUT[0] + C);

        pressure_ = A + B / (C + pdout);
        temp_ = -45.f + 175.f / 0x10000 * tdout;
        result = true;
    }
    return result;
}

void ICP10125::reset() {
    if (!sci.write(ICP10125_SOFT_RESET, sizeof(ICP10125_SOFT_RESET))) {
        log.error("Soft reset failed");
    } else {
        HAL_Delay(1000);
    }
}