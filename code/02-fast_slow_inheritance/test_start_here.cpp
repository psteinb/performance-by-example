#define test_start_here
#include <cmath>
#include <chrono>
#include <vector>
#include <random>
#include <iostream>

#include "gtest/gtest.h"

#include "setup_fixture.hpp"
#include "render_entity.hpp"
#include "info.hpp"

TEST_F(iot_setup, runtime_original) {

  EXPECT_EQ(iot::device_info.size(),iot::size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
