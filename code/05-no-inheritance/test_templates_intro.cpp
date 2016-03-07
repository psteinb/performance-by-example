
#include <vector>
#include <random>

#include "gtest/gtest.h"
#include "stats_fixture.hpp"

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


TEST_F(stats_fixture,mean_example) {


  mean_fp32 my_mean;
  for(float & fl : fp_data)
    my_mean.update_by(fl);

  const float mean = my_mean.value();
  EXPECT_GT(mean,fp_mean - fp_sigma);
  EXPECT_LT(mean,fp_mean + fp_sigma);
  
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
