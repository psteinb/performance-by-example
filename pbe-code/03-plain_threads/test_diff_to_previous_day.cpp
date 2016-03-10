#define test_diff_to_previous_day
#include <cmath>
#include <chrono>
#include <thread>
#include <functional>
#include <algorithm>

#include "gtest/gtest.h"
#include "info.hpp"

using fp_microseconds_t = std::chrono::duration<double, std::chrono::microseconds::period>;
using fp_nanoseconds_t = std::chrono::duration<double, std::chrono::nanoseconds::period>;
static int repeats = 10;

using namespace iot;


void diff_in_thread(const std::vector<device_t>& _lhs,
		    const std::vector<device_t>& _rhs,
		    std::vector<device_t>&_result,
		    size_t thread_id){

  const size_t items_per_thread = (_lhs.size() + std::thread::hardware_concurrency() - 1)/std::thread::hardware_concurrency();
  const size_t offset = thread_id*items_per_thread;

  for(size_t offset = thread_id*items_per_thread;
      offset<((thread_id+1)*items_per_thread);
      ++offset)
    {
      _result[offset] = _lhs[offset] - _rhs[offset];
    }
  
}

void parallel_difference(const std::vector<device_t>& _lhs,
			 const std::vector<device_t>& _rhs,
			 std::vector<device_t>&_result){

  if(_result.size()!=_rhs.size() || _result.empty())
    _result.resize(_rhs.size());
  
  std::vector<std::thread> workers;
  
  for(size_t tid = 0;tid<std::thread::hardware_concurrency();++tid){
    workers.push_back(std::thread(diff_in_thread,
				  std::ref(_lhs),
				  std::ref(_rhs),
				  std::ref(_result),
				  tid
				  )
		      );
  }

  for(auto& w : workers)
    w.join();
    
}


TEST(power_total,non_zero_device_infos_available) {
  EXPECT_GT(iot::device_info.size(),0);
  
}


TEST(power_total,diff_in_thread) {

  std::vector<device_t> initial_device_info = iot::device_info;
  std::vector<device_t> delta(iot::size());
  iot::update_device_info(device_info);
  std::vector<device_t> updated_device_info = iot::device_info;
  
  diff_in_thread(initial_device_info,updated_device_info,
		 delta, 0);

  EXPECT_NE(delta.front().power_consumption,0);
  EXPECT_LT(delta.back().power_consumption,1e-3);
  EXPECT_FLOAT_EQ(delta.front().power_consumption,initial_device_info.front().power_consumption);
  EXPECT_NE(delta.back().power_consumption,initial_device_info.back().power_consumption);
}


TEST(power_total,diff_serial) {

  std::vector<device_t> initial_device_info = iot::device_info;
  std::vector<device_t> delta(iot::size());
  iot::update_device_info(device_info);
  std::vector<device_t> updated_device_info = iot::device_info;
    
  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    std::transform(initial_device_info.begin(),
    		   initial_device_info.end(),
    		   device_info.begin(),
    		   delta.begin(),
    		   std::minus<iot::device_t>()
    		   );
  }
  auto end_t = std::chrono::high_resolution_clock::now();
  double time_diff_mus = (fp_microseconds_t(end_t - start_t)).count();
    
  EXPECT_GT(time_diff_mus,0.);
  for(size_t i = 0;i<20;++i)
    EXPECT_NE(delta[i].power_consumption,0);
}

TEST(power_total,diff_parallel) {

  std::vector<device_t> initial_device_info = iot::device_info;
  std::vector<device_t> delta(iot::size());
  iot::update_device_info(device_info);
  std::vector<device_t> updated_device_info = iot::device_info;
    
  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    parallel_difference(initial_device_info,updated_device_info,delta);
  }
  auto end_t = std::chrono::high_resolution_clock::now();
  double time_diff_mus = (fp_microseconds_t(end_t - start_t)).count();
  
  EXPECT_GT(time_diff_mus,0.);
  for(size_t i = 0;i<20;++i)
    EXPECT_NE(delta[i].power_consumption,0);

  
}

TEST(power_total,parallel_equals_sequential) {

  
  std::vector<device_t> initial_device_info = iot::device_info;
  iot::update_device_info(device_info);
  std::vector<device_t> updated_device_info = iot::device_info;
  std::vector<device_t> par_delta(iot::size());
  std::vector<device_t> seq_delta(iot::size());
  
  parallel_difference(initial_device_info,device_info,par_delta);
  
  std::transform(initial_device_info.begin(),
		 initial_device_info.end(),
		 device_info.begin(),
		 seq_delta.begin(),
		 [&](const device_t& _lhs, const device_t& _rhs){
		   return _lhs - _rhs;
		 });
  
  for(size_t i = 0;i<20;++i)
    EXPECT_FLOAT_EQ(par_delta[i].power_consumption,seq_delta[i].power_consumption);

}

// TEST(power_total,non_zero) {

//   double sum = sum_power(iot::device_info);
  
//   EXPECT_GT(sum,0.);
  
// }


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

