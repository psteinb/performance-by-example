#define test_parallel_delta
#include <cmath>
#include <chrono>
#include <thread>

#include "gtest/gtest.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

#include "info.hpp"

using fp_microseconds_t = std::chrono::duration<double, std::chrono::microseconds::period>;
using fp_nanoseconds_t = std::chrono::duration<double, std::chrono::nanoseconds::period>;
static int repeats = 10;

using namespace iot;


struct delta_apply{

  const std::vector<device_t>* lhs;
  const std::vector<device_t>* rhs;
  std::vector<device_t>* res;

  delta_apply(const std::vector<device_t>* _lhs,
	      const std::vector<device_t>* _rhs,
	      std::vector<device_t>*	   _res):
    lhs(_lhs),
    rhs(_rhs),
    res(_res){}

  ~delta_apply(){}

  void operator()(const tbb::blocked_range<size_t>& range) const {
    std::vector<device_t>* local_res = res;
    
    for( size_t i=range.begin(); i!=range.end(); ++i )
      (*local_res)[i] = (*lhs)[i]-(*rhs)[i];

  }
};


TEST(power_difference,non_zero_device_infos_available) {
  EXPECT_GT(iot::device_info.size(),0);
  
}

TEST(power_difference,test_update) {

  std::vector<device_t> initial_device_info = iot::device_info;
  std::vector<device_t> updated_device_info = iot::device_info;
  iot::update_device_info(updated_device_info);

  for(size_t i = 0;i<20;++i){
    EXPECT_NE(initial_device_info[i].power_consumption,updated_device_info[i].power_consumption);
  }
  
}

TEST(power_difference,parallel) {

  std::vector<device_t> initial_device_info = iot::device_info;
  std::vector<device_t> updated_device_info = iot::device_info;
  iot::update_device_info(updated_device_info);

  std::vector<device_t> delta(iot::size());
  
  tbb::blocked_range<size_t> brange(0,size());
  delta_apply ops(&initial_device_info,
		  &updated_device_info,
		  &delta);
  
  for(int i = 0;i<repeats;++i){
    tbb::parallel_for(brange,ops);
  }
  
  for(size_t i = 0;i<20;++i){
    EXPECT_NE(delta[i].power_consumption,0);
    EXPECT_NE(delta[i].power_consumption,initial_device_info[i].power_consumption);
  }
  
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
