#define test_templates_intro

#include <vector>
#include <random>

#include "gtest/gtest.h"

struct float_ops {
  
  virtual float update_by(float& _data) = 0;

  virtual ~float_ops(){};
};

struct power_sum : pulic float_ops {

  float result = 0.;

  ~power_sum(){};
  
  virtual float update_by(float& _data){
    return (result += _data);
  };

};


TEST(templates_intro,generic_functions) {

  std::vector<float> payload(1 << 15);
  std::srand(0);
  for(float & fl : payload)
    fl = std::rand()/RAND_MAX;

  power_sum total;
  for(float & fl : payload)
    total.update_by(fl);

  
  EXPECT_GT(total,0);
  
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
