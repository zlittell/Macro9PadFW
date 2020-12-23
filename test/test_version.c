#include "unity.h"
#include "version.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_version_ShouldReturnCorrectLength(void)
{
    uint8_t size = sizeof(DeviceVersion);
    TEST_ASSERT_EQUAL_UINT8(3, size);
}

void test_version_ShouldReturnSomeVersion(void)
{
    TEST_ASSERT_TRUE(((DeviceVersion[0]>0) | 
        (DeviceVersion[1]>0) | (DeviceVersion[2]>0)));
}