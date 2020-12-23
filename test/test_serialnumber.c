#include "unity.h"
#include "serialnumber.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_serialnumber_ShouldReturnTemporarySerial(void)
{
    TEST_ASSERT_EQUAL_CHAR_ARRAY("0123456789", DeviceSerialNumber, 
        DEVICESERIALNUMBERLENGTH);
}
