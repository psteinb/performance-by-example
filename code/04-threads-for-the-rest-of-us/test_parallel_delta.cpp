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
      (*res)[i] = (*lhs)[i]-(*rhs)[i];

  }
};


TEST(power_total,non_zero_device_infos_available) {
  EXPECT_GT(iot::device_info.size(),0);
  
}



TEST(power_total,diff_parallel) {

  std::vector<device_t> initial_device_info = iot::device_info;
  std::vector<device_t> delta(iot::size());
  iot::update_device_info(device_info);
  std::vector<device_t> updated_device_info = iot::device_info;

  tbb::blocked_range<size_t> brange(0,size());
  delta_apply ops(&initial_device_info,
		  &updated_device_info,
		  &delta);
  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    tbb::parallel_for(brange,ops);
  }
  auto end_t = std::chrono::high_resolution_clock::now();
  double time_diff_mus = (fp_microseconds_t(end_t - start_t)).count();
  
  EXPECT_GT(time_diff_mus,0.);
  
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
