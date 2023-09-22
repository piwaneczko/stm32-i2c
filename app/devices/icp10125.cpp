#include "icp10125.h"

#include "crc8maxim.h"
#include "log.h"
#include "main.h"

#define CRC8_INIT 0xFF

static Log log{Log::Level::Debug, "ICP10125"};

ICP10125::ICP10125(SCI& sci) : sci(sci) {}

uint8_t ICP10125_READ_REG = 0xC7;
uint8_t ICP10125_READ_ID[] = {0xEF, 0xC8};
uint8_t ICP10125_SOFT_RESET[] = {0x80, 0x5D};
uint8_t ICP10125_LP_MODE[] = {0x60, 0x9C};
uint8_t ICP10125_N_MODE[] = {0x68, 0x25};
uint8_t ICP10125_LN_MODE[] = {0x70, 0xDF};
uint8_t ICP10125_ULN_MODE[] = {0x78, 0x66};
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
    sci.receiveCompletedEvent = completedEvent;
    sci.init();
    sci.initialized = isConnected();

    // reset();

    Crc8Maxim crc;
    if (sci.write(ICP10125_SETUP_OTP_READ, sizeof(ICP10125_SETUP_OTP_READ))) {
        uint8_t buffer[3] = {0};
        for (uint8_t i = 0; i < 4; i++) {
            if (sci.write(ICP10125_OTP_READ_NEXT, sizeof(ICP10125_OTP_READ_NEXT))) {
                if (sci.read(buffer, sizeof(buffer))) {
                    if (crc.calculate(buffer, 2, CRC8_INIT) == buffer[2]) {
                        sensorCal[i] = (float)((int16_t)(((uint16_t)buffer[0] << 8) | buffer[1]));
                    } else {
                        log.error("CRC calculation failed");
                        sci.initialized = false;
                    }
                } else {
                    log.error("Reading OTP failed");
                    sci.initialized = false;
                }
            } else {
                log.error("Writing OTP_READ_NEXT failed");
                sci.initialized = false;
            }
        }
    } else {
        log.error("Writing SETUP_OTP_READ failed");
        sci.initialized = false;
    }

    if (sci.initialized) {
        log.info("init successfull");
    }
}

bool ICP10125::receiveRawData(Asynchronic async) {
    return isConnected();
}

void ICP10125::reset() {
    if (!sci.write(ICP10125_SOFT_RESET, sizeof(ICP10125_SOFT_RESET))) {
        log.error("Soft reset failed");
    }
}