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

std::vector<int> synthetic_devices(const size_t& _size) {

  std::vector<int> return_value(_size,0);
  std::srand(_size);
  
  for( int & element : return_value )
    element = std::rand();

  return return_value;
}

std::size_t count_active() {

  size_t sum = 0;
  for(size_t room = 0;room < iot::n_rooms();++room){
    for(size_t level = 0;level < iot::n_levels();++level){
      sum += iot::active_devices(level,room);
    }
  }
  return sum;
}

std::size_t count_active_fast() {

  size_t sum = 0;
  for(size_t level = 0;level < iot::n_levels();++level){
    for(size_t room = 0;room < iot::n_rooms();++room){
      sum += iot::active_devices(level,room);
    }
  }

  return sum;
}


TEST(Call_Devices, runtime_real) {

  auto t_start = std::chrono::high_resolution_clock::now();
  size_t n_active = 0;
  for(int i =0;i<repeats;++i)
    n_active = count_active();
  auto t_end = std::chrono::high_resolution_clock::now();
  double time_diff_mus = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count();
  time_diff_mus /= repeats;
  
  EXPECT_GT(time_diff_mus,0);
  std::cout << "real    : " << time_diff_mus << " mus\n";
  
}


TEST(Call_Devices, runtime_training) {

  std::vector<int> training_data = synthetic_devices(iot::n_rooms()*iot::n_levels());
  auto t_start = std::chrono::high_resolution_clock::now();
  size_t n_active_synthetic = 0;

  for(int i =0;i<repeats;++i)
    for( size_t i = 0;i<training_data.size();++i)
      n_active_synthetic += training_data[i];

  auto t_end = std::chrono::high_resolution_clock::now();
  double time_diff_mus = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count()/repeats;
  
  EXPECT_GT(time_diff_mus,0);
  std::cout << "training: " << time_diff_mus
	    << " mus\n";
  
}


TEST(Call_Devices, runtime_fast) {

  auto t_start = std::chrono::high_resolution_clock::now();

  size_t n_active = 0;
  for(int i =0;i<repeats;++i)
    n_active = count_active_fast();
  
  auto t_end = std::chrono::high_resolution_clock::now();
  double time_diff_mus = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count()/repeats;

  EXPECT_GT(time_diff_mus,0);
  std::cout << "fast    : " << time_diff_mus << " mus\n";
  
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
