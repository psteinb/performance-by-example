
#include <vector>
#include <random>
#include <chrono>

#include "gtest/gtest.h"
#include "stats_fixture.hpp"

using mus_t = std::chrono::duration<double, std::chrono::microseconds::period>;
static int repeats = 10;

template <typename it_type>
double simple_mean(it_type begin, it_type end){

  size_t n_items = end - begin;
  double result = 0;
  for(;begin!=end;++begin)
    result += *begin;

  return result/n_items;
}

struct statistics_observable {
  
  virtual void update_by(float& _data) = 0;
  virtual void update_by(int& _data) = 0;

  virtual ~statistics_observable(){};
};

struct mean_fp32 : public statistics_observable {

  float		result = 0.;
  std::size_t	size_sample = 0;

  ~mean_fp32(){};
  
  void update_by(float& _data) final override {
    (result += _data);
    size_sample++;
  };

  void update_by(int& _data) final override {
    float casted = float(_data);
    return update_by(casted);
  };

  float value() const {
    return result/size_sample;
  }
  

};


TEST_F(stats_fixture,mean_through_oop) {

  float mean = 0;
  auto start_t = std::chrono::high_resolution_clock::now();
  for(int r =0;r<repeats;++r){
    
    mean_fp32 my_mean;
    for(float & fl : fp_data)
      my_mean.update_by(fl);
    mean = my_mean.value();

  }
  auto end_t = std::chrono::high_resolution_clock::now();
  auto diff = mus_t(end_t - start_t).count();
  std::cout << "mean_through_oop: " << diff << "ms\n";
  
  EXPECT_GT(mean,fp_mean - .1*fp_sigma);
  EXPECT_LT(mean,fp_mean + .1*fp_sigma);
  
}

TEST_F(stats_fixture,mean_through_function) {

  auto start_t = std::chrono::high_resolution_clock::now();
  float mean = 0;

  for(int r =0;r<repeats;++r){
    mean = simple_mean(fp_data.begin(),fp_data.end());
  }
  auto end_t = std::chrono::high_resolution_clock::now();
  auto diff = mus_t(end_t - start_t).count();
  std::cout << "mean_through_function: " << diff << "ms\n";

  
  EXPECT_GT(mean,fp_mean - .1*fp_sigma);
  EXPECT_LT(mean,fp_mean + .1*fp_sigma);
  
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
