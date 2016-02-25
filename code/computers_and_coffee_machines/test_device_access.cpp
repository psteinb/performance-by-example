#define test_accessor
#include "gtest/gtest.h"
#include "iot.hpp"

TEST(access_works,in_first_room) {
  EXPECT_GT(iot::devices_active(0,0),0);
  
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
