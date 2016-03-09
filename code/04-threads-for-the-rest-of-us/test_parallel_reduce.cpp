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


double serial_power_sum(const std::vector<device_t>& _data){

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
    result(0)
  {}

  ~power_sum(){}

  void operator()(const tbb::blocked_range<device_t*>& range) {
    
    double local = 0;
    
    for( device_t* i=range.begin(); i!=range.end(); ++i )
      local += i->power_consumption;

    result += local;

  }

  void join(power_sum& _rhs){ result += _rhs.result; }
};

struct index_based_sum{

  double result;
  std::vector<iot::device_t>* data;

  index_based_sum(std::vector<iot::device_t>* _data):
    result(0),
    data(_data)
  {}
  
  index_based_sum(index_based_sum& _rhs, tbb::split ):
    result(0),
    data(_rhs.data)
  {}

  ~index_based_sum(){}

  void operator()(const tbb::blocked_range<size_t>& range) {
    
    double local = 0;
    
    for( size_t i=range.begin(); i!=range.end(); ++i )
      local += (*data)[i].power_consumption;

    result += local;

  }

  void join(index_based_sum& _rhs){ result += _rhs.result; data = _rhs.data; }
};


TEST(power_total,non_zero_device_infos_available) {
  EXPECT_GT(iot::device_info.size(),0);
  
}

TEST(power_total,serial) {

  double sum = 0;
  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    sum = serial_power_sum(iot::device_info);
    
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

  const size_t items_per_thread = (32 + std::thread::hardware_concurrency() - 1)/std::thread::hardware_concurrency();
  tbb::blocked_range<size_t> brange(0,32,items_per_thread);

  index_based_sum functor(&dinfo);

  auto start_t = std::chrono::high_resolution_clock::now();
  for(int i = 0;i<repeats;++i){
    tbb::parallel_reduce(brange,functor);
  }

  EXPECT_GT(functor.result,0.);

  double serial_sum = sum_iterators(&dinfo[0],
				    &dinfo[0] + 32);

  int iserial = serial_sum;
  int iparallel = functor.result;
  ASSERT_EQ(iserial,iparallel);
  EXPECT_FLOAT_EQ(functor.result,serial_sum);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
