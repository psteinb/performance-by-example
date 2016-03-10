#define test_start_here
#include <cmath>
#include <thread>

#include "gtest/gtest.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

#include "timing.hpp"
#include "info.hpp"

static int repeats = 10;


TEST(power_difference,non_zero_device_infos_available) {
  EXPECT_GT(iot::device_info.size(),0);
  
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
