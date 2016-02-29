#pragma once
#include <vector>


namespace iot {

  static int n_levels_ = 64;
  static int n_rooms_ = 1 << 20;

  
  static int n_rooms(){
    return n_rooms_;
  }

  static int n_levels(){
    return n_levels_;
  }

  static int size(){
    return n_levels_*n_rooms_;
  }
  
  static std::vector<int> fill_devices(){
    std::vector<int> value(size());
    for (int & i : value)
      i = 42;//TODO insert RNG here
    return value;
  }
  
  static std::vector<int> devices_ = fill_devices();

  static int active_devices(int level_number, int room_number){
    return devices_[(level_number*n_rooms())+room_number];
  }
  
}
