#ifndef __SETUP_FIXTURE_HPP__
#define __SETUP_FIXTURE_HPP__

#include "gtest/gtest.h"
#include "iot.hpp"

struct iot_setup : public ::testing::Test {

  iot_setup(){
    iot::reset_building(8,16);
  }
  
};

static int repeats = 10;

#endif
