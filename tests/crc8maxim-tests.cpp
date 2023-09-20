#include <gtest/gtest.h>

#include "crc8maxim.h"

struct Crc8MaximTest : public testing::Test {
    Crc8Maxim crc;
};

TEST_F(Crc8MaximTest, TEST1) {
    uint8_t buffer[] = {0x00};
    EXPECT_EQ(0xAC, crc.calculate(buffer, sizeof(buffer), 0xFF));
}

TEST_F(Crc8MaximTest, TEST2) {
    uint8_t buffer[] = {0xBE, 0xEF};
    EXPECT_EQ(0x92, crc.calculate(buffer, sizeof(buffer), 0xFF));
}

TEST_F(Crc8MaximTest, TEST3) {
    uint8_t buffer[] = {0x00, 0x66};
    EXPECT_EQ(0x9C, crc.calculate(buffer, sizeof(buffer), 0xFF));
}