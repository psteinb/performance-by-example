#define test_accessor
#include <cmath>
#include <chrono>
#include <vector>
#include <random>
#include <iostream>

#include "gtest/gtest.h"
#include "entity.hpp"
#include "iot.hpp"

using fp_microseconds_t = std::chrono::duration<double, std::chrono::microseconds::period>;
using fp_nanoseconds_t = std::chrono::duration<double, std::chrono::nanoseconds::period>;
static int repeats = 10;

TEST(Call_Devices, runtime_original) {

  iot::reset_building(8,16);

  EXPECT_LE(iot::size(),8*16);
  
  std::vector<iot::entity> current_devices;
  current_devices.reserve(iot::size());

  for( const iot::device_t& d : iot::device_info )
    current_devices.push_back(iot::entity(d));

  std::size_t tmp_size = 0;
  std::size_t pow_size = 0;
  
  for(int r = 0;r<repeats;++r){
    for( const iot::entity& e : current_devices )
      tmp_size += e.temperature()->prepare();

    for( const iot::entity& e : current_devices )
      pow_size += e.power()->prepare();
  }
  
  EXPECT_GT(pow_size,0.);
  EXPECT_GT(tmp_size,0.);
}


TEST(Call_Devices, runtime_opt) {

  iot::reset_building(8,16);


  EXPECT_LE(iot::size(),8*16);
  
  std::vector<int> current_power_width;
  current_power_width.reserve(iot::size());
  std::vector<int> current_temp_width;
  current_temp_width.reserve(iot::size());

  size_t count = 0;
  for( const iot::device_t& d : iot::device_info ){
    current_power_width.push_back(std::to_string(d.power_consumption).size());
    current_temp_width.push_back(std::to_string(d.temperature_kelvin).size());
  }

  std::size_t tmp_size = 0;
  std::size_t pow_size = 0;
  
  for(int r = 0;r<repeats;++r){
    for( size_t i = 0;i<iot::size();++i )
      pow_size += current_power_width[i];

    for( size_t i = 0;i<iot::size();++i )
      tmp_size += current_temp_width[i];
  }
  
  EXPECT_GT(pow_size,0.);
  EXPECT_GT(tmp_size,0.);
  
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
