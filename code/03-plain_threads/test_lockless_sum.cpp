#define test_lockless_sum
#include <cmath>
#include <chrono>
#include <thread>
#include <functional>
#include <algorithm>
#include <atomic>

#include "gtest/gtest.h"
#include "info.hpp"

using fp_microseconds_t = std::chrono::duration<double, std::chrono::microseconds::period>;
using fp_nanoseconds_t = std::chrono::duration<double, std::chrono::nanoseconds::period>;
static int repeats = 10;

using namespace iot;




// TEST(lockless_sum,callout_print_to_stdout) {

//   std::vector<std::thread> workers;

//   auto print_me = [](int tid){
//     std::cout << "I am thread " << tid << "\n";
//   };
//   for(size_t tid = 0;tid<std::thread::hardware_concurrency();++tid){
//     workers.push_back(std::thread(print_me,
// 				  tid
// 				  )
// 		      );
//   }
    
//   for(auto& w : workers)
//     w.join();

  
// }

void add_up(const std::vector<float>& _data,
	    size_t _thread_id,
	    float& _sum){

  const size_t items_per_thread = (_data.size() + std::thread::hardware_concurrency() - 1)/std::thread::hardware_concurrency();
  const size_t offset = _thread_id*items_per_thread;

  const size_t chunk_begin = _thread_id*items_per_thread;
  size_t chunk_end = (_thread_id+1)*items_per_thread;
  if(chunk_end>_data.size())
    chunk_end = _data.size();


  for(size_t offset = chunk_begin;offset<chunk_end;++offset)
    {
      _sum += _data[offset];
    }

}

TEST(lockless_sum,simple_sum) {

  float sum = 0;
  std::vector<float> some_values(1e5,42);
  std::vector<std::thread> workers;

  for(int r = 0;r<repeats;++r){
    for(size_t tid = 0;tid<std::thread::hardware_concurrency();++tid){
      workers.push_back(std::thread(add_up,
				    std::ref(some_values),
				    tid,
				    std::ref(sum)
				    )
			);
    }
    
    for(auto& w : workers)
      w.join();
    
    workers.clear();
    try{
      ASSERT_FLOAT_EQ(sum,some_values.size()*42.);
    }
    catch(...){
      std::cerr << "repreat" << r << "failed\n";
      throw;
    }
    sum = 0;
  }
  
}

void lockless_add_up(const std::vector<float>& _data,
		     size_t _thread_id,
		     std::atomic<float>& _sum){

  const size_t items_per_thread = (_data.size() + std::thread::hardware_concurrency() - 1)/std::thread::hardware_concurrency();
  const size_t offset = _thread_id*items_per_thread;

  const size_t chunk_begin = _thread_id*items_per_thread;
  size_t chunk_end = (_thread_id+1)*items_per_thread;
  if(chunk_end>_data.size())
    chunk_end = _data.size();

  float partial_sum = 0;
  for(size_t offset = chunk_begin;offset<chunk_end;++offset)
    {
      partial_sum = partial_sum + _data[offset];
    }

  _sum = _sum + partial_sum;
}

TEST(lockless_sum,correct) {

  std::atomic<float> sum(0);
  std::vector<float> some_values(1e5,42);
  std::vector<std::thread> workers;

  for(int r = 0;r<repeats;++r){
    for(size_t tid = 0;tid<std::thread::hardware_concurrency();++tid){
      workers.push_back(std::thread(lockless_add_up,
				    std::ref(some_values),
				    tid,
				    std::ref(sum)
				    )
			);
    }
    
    for(auto& w : workers)
      w.join();
    
    workers.clear();
    try{
      ASSERT_FLOAT_EQ(sum,some_values.size()*42.);
    }
    catch(...){
      std::cerr << "repreat" << r << "failed\n";
      throw;
    }
    sum = 0;
  }
  
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

