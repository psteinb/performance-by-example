#define test_runtime_device_display
#include <cmath>
#include <chrono>
#include <vector>
#include <random>
#include <iostream>

#include "gtest/gtest.h"
#include "setup_fixture.hpp"

#include "render_entity.hpp"


TEST_F(iot_setup, runtime_original) {

  std::vector<iot::render_entity> current_devices;
  current_devices.reserve(iot::size());

  for( const iot::device_t& d : iot::device_info )
    current_devices.push_back(iot::render_entity(d));

  std::size_t tmp_size = 0;
  std::size_t pow_size = 0;
  
  auto start = std::chrono::high_resolution_clock::now();
  for(int r = 0;r<repeats;++r){
    for( const iot::render_entity& e : current_devices )
      tmp_size += e.temperature()->prepare();

    for( const iot::render_entity& e : current_devices )
      pow_size += e.power()->prepare();
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end-start;
  std::cout << "original: " << elapsed.count() << "ms\n";
  
  EXPECT_GT(pow_size,0.);
  EXPECT_GT(tmp_size,0.);
}


TEST_F(iot_setup, runtime_opt) {

  std::vector<int> current_power_width;
  current_power_width.reserve(iot::size());
  std::vector<int> current_temp_width;
  current_temp_width.reserve(iot::size());

  for( const iot::device_t& d : iot::device_info ){
    current_power_width.push_back(std::to_string(d.power_consumption).size());
    current_temp_width.push_back(std::to_string(d.temperature_kelvin).size());
  }

  std::size_t tmp_size = 0;
  std::size_t pow_size = 0;

  auto start = std::chrono::high_resolution_clock::now();
  
  for(int r = 0;r<repeats;++r){
    for( size_t i = 0;i<iot::size();++i )
      pow_size += current_power_width[i];

    for( size_t i = 0;i<iot::size();++i )
      tmp_size += current_temp_width[i];
  }
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end-start;
  std::cout << "optimised: " << elapsed.count() << "ms\n";
  
  EXPECT_GT(pow_size,0.);
  EXPECT_GT(tmp_size,0.);
  
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
