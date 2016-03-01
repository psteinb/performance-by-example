#define test_accessor
#include <cmath>
#include <chrono>
#include <vector>
#include <random>

#include "gtest/gtest.h"
#include "iot.hpp"

using fp_microseconds_t = std::chrono::duration<double, std::chrono::microseconds::period>;
using fp_nanoseconds_t = std::chrono::duration<double, std::chrono::nanoseconds::period>;
static int repeats = 10;


TEST(Call_Devices, runtime_real) {
  
  EXPECT_GT(iot::devices_.size(),0);
  
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
