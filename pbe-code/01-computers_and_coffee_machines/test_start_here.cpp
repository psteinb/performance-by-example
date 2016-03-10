#define test_start_here
#include <cmath>

#include "gtest/gtest.h"
#include "iot.hpp"

TEST(access_works,building_present) {
  EXPECT_GT(iot::size(),0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
