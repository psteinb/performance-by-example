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

template <typename it>
double sum_iterators(it _begin, it _end){

  double result = 0;

  for(;_begin!=_end;++_begin)
    result += _begin->power_consumption;

  return result;
}


double sum_power(const std::vector<device_t>& _data){

  double result = 0;

  for(const device_t& item : _data)
    result += item.power_consumption;

  return result;
}

struct power_sum{

  double result;

  power_sum():
    result(0){}
  
  power_sum(power_sum& _rhs, tbb::split ):
    result(0.)
  {}

  ~power_sum(){}

  void operator()(const tbb::blocked_range<device_t*>& range) {
    
    double local = result;
    
    for( device_t* i=range.begin(); i!=range.end(); ++i )
      local += i->power_consumption;

    result = local;

  }

  void join(power_sum& _rhs){ result += _rhs.result; }
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

TEST(power_total,parallel) {

  std::vector<device_t> dinfo = iot::device_info;
  
  tbb::blocked_range<device_t*> brange(&dinfo[0],
				       &dinfo[0] + size());

  power_sum functor;

  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    tbb::parallel_reduce(brange,functor);
  }

  EXPECT_GT(functor.result,0.);
}

TEST(power_total,parallel_same_as_seq) {

  std::vector<device_t> dinfo = iot::device_info;


  
  tbb::blocked_range<device_t*> brange(&dinfo[0],
				       &dinfo[0] + 128);

  power_sum functor;

  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    tbb::parallel_reduce(brange,functor);
  }

  EXPECT_GT(functor.result,0.);

  double serial_sum = sum_iterators(&dinfo[0],
				    &dinfo[0] + 128);
  ASSERT_EQ((int)functor.result,(int)serial_sum);
  EXPECT_FLOAT_EQ(functor.result,serial_sum);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
