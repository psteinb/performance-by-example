#define test_parallel_reduce
#include <cmath>
#include <chrono>
#include <thread>

#include "gtest/gtest.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"

#include "info.hpp"

using fp_microseconds_t = std::chrono::duration<double, std::chrono::microseconds::period>;
using fp_nanoseconds_t = std::chrono::duration<double, std::chrono::nanoseconds::period>;
static int repeats = 10;

using namespace iot;

double sum_power(const std::vector<device_t>& _data){

  double value = 0;

  for(const device_t& item : _data)
    value += item.power_consumption;

  return value;
}

struct power_sum{

  double sum;

  power_sum():
    sum(0){}
  
  power_sum(power_sum& _rhs, tbb::split ):
    sum(0.)
  {}

  ~power_sum(){}

  void operator()(const tbb::blocked_range<device_t*>& range) {
    
    double local = sum;
    
    for( device_t* i=range.begin(); i!=range.end(); ++i )
      local += i->power_consumption;

    sum = local;

  }

  void join(power_sum& _rhs){ sum += _rhs.sum; }
};


TEST(power_total,non_zero_device_infos_available) {
  EXPECT_GT(iot::device_info.size(),0);
  
}

TEST(power_total,sum_seq) {

  double sum = 0;
  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    sum = sum_power(iot::device_info);
    
  }
  auto end_t = std::chrono::high_resolution_clock::now();

  double time_diff_mus = (fp_microseconds_t(end_t - start_t)).count();
  
  EXPECT_GT(time_diff_mus,0.);

}

TEST(power_total,sum_parallel) {

  std::vector<device_t> initial_device_info = iot::device_info;
  
  tbb::blocked_range<device_t*> brange(&device_info[0],
				       &device_info[0] + size());

  power_sum functor;

  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    tbb::parallel_reduce(brange,functor);
  }
  auto end_t = std::chrono::high_resolution_clock::now();
  double time_diff_mus = (fp_microseconds_t(end_t - start_t)).count();
  
  EXPECT_GT(time_diff_mus,0.);
  
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
