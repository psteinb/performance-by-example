#define test_accessor
#include <cmath>

#include "gtest/gtest.h"
#include "iot.hpp"

TEST(access_works,in_first_room) {
  EXPECT_GT(iot::active_devices(0,0),0);
  
}

TEST(access_works,sum_of_all) {

	size_t sum = 0;
	for(size_t room = 0;room < iot::n_rooms();++room){
		for(size_t level = 0;level < iot::n_levels();++level){
			sum += iot::active_devices(level,room);
		}
	}
	
	EXPECT_LT(sum,std::pow(2,31));
	EXPECT_GT(sum,0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
