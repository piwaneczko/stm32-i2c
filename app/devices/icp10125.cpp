#include "icp10125.h"

#include "crc8maxim.h"
#include "log.h"
#include "main.h"

#define CRC8_INIT 0xFF

static Log log{Log::Level::Debug, "ICP10125"};

ICP10125::ICP10125(SCI& sci) : sci(sci) {}

uint8_t ICP10125_WRITE_REG = 0xC6;
uint8_t ICP10125_READ_REG = 0xC7;
uint8_t ICP10125_SOFT_RESET[] = {ICP10125_WRITE_REG, 0x80, 0x5D};
uint8_t ICP10125_LP_MODE[] = {ICP10125_WRITE_REG, 0x60, 0x9C};
uint8_t ICP10125_N_MODE[] = {ICP10125_WRITE_REG, 0x68, 0x25};
uint8_t ICP10125_LN_MODE[] = {ICP10125_WRITE_REG, 0x70, 0xDF};
uint8_t ICP10125_ULN_MODE[] = {ICP10125_WRITE_REG, 0x78, 0x66};
uint8_t ICP10125_SETUP_OTP_READ[] = {ICP10125_WRITE_REG, 0xC5, 0x95, 0x00, 0x69, 0x9C};
uint8_t ICP10125_OTP_READ_NEXT[] = {ICP10125_READ_REG, 0xF7};

void ICP10125::init(SCI::CompletedEventHandler completedEvent) {
    sci.receiveCompletedEvent = completedEvent;
    sci.init();
    sci.initialized = true;

    if (!sci.write(ICP10125_ULN_MODE, sizeof(ICP10125_ULN_MODE))) {
        log.error("Start sensor in Ultra-Low Noise failed");
        sci.initialized = false;
    }

    reset();

    Crc8Maxim crc;
    if (sci.write(ICP10125_SETUP_OTP_READ, sizeof(ICP10125_SETUP_OTP_READ))) {
        uint8_t buffer[3] = {0};
        for (uint8_t i = 0; i < 4; i++) {
            if (sci.write(ICP10125_OTP_READ_NEXT, sizeof(ICP10125_OTP_READ_NEXT))) {
                if (sci.read(buffer, sizeof(buffer))) {
                    if (crc.calculate(buffer, 2, CRC8_INIT) == buffer[2]) {
                        sensorConstants[i] = (float)((int16_t)(((uint16_t)buffer[0] << 8) | buffer[1]));
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
    return false;
}

void ICP10125::reset() {
    if (!sci.write(ICP10125_SOFT_RESET, sizeof(ICP10125_SOFT_RESET))) {
        log.error("Soft reset failed");
    }
}
