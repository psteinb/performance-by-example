#pragma once
#include <vector>
#include <random>

namespace iot {

  static int n_levels_ = 64;
  static int n_rooms_ = 1 << 18;

  
  static int n_rooms(){
    return n_rooms_;
  }

  static int n_levels(){
    return n_levels_;
  }

  static void reset_building(int level_number, int room_number){
    n_levels_ = level_number;
    n_rooms_ = room_number;
  }
  
  static int size(){
    return n_levels_*n_rooms_;
  }
  
  static std::vector<int> fill_devices(){
    std::srand(size());
    
    std::vector<int> value(size());
    for (int & i : value)
      i = (std::rand()/float(RAND_MAX)) > .5 ? 1 : 0;
    return value;
  }
  
  static std::vector<int> devices_ = fill_devices();

  static int active_devices(int level_number, int room_number){
    return devices_[(level_number*n_rooms())+room_number];
  }
  
}
