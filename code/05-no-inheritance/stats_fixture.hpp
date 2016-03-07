#ifndef _STATS_FIXTURE_H_
#define _STATS_FIXTURE_H_




#include <random>
#include "gtest/gtest.h"

struct stats_fixture : public ::testing::Test
{

  static const int size = 1 << 20;

  std::vector<int> int_data;
  std::vector<float> fp_data;

  const float fp_mean;
  const float fp_sigma;

  const int int_mean;
  const int int_sigma;

  stats_fixture():
    int_data(size),
    fp_data(size),
    fp_mean(5),
    fp_sigma(2),
    int_mean(256),
    int_sigma(42)
  {
  }

  ~stats_fixture(){};
  
  virtual void SetUp(){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::normal_distribution<float> fp_normal(fp_mean,fp_sigma);
    std::normal_distribution<float> int_normal(int_mean,int_sigma);
    
    for( int& i : int_data )
      i = int_normal(gen);

    for( float& f : fp_data )
      f = fp_normal(gen);
    
  }
};

  #endif /* _STATS_FIXTURE_H_ */
