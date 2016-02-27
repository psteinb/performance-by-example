#pragma once
#include <vector>

#ifndef N_LEVELS
#define N_LEVELS 32
#endif

#ifndef N_ROOMS
#define N_ROOMS (1<<10)
#endif

namespace iot {

  static std::vector<int> fill_devices(){
    std::vector<int> value(N_LEVELS*N_ROOMS);
    for (int & i : value)
      i = 42;//TODO insert RNG here
    return value;
  }
  
  static std::vector<int> devices_ = fill_devices();

  static int devices_active(int level_number, int room_number){
    return devices_[(level_number*N_ROOMS)+room_number];
  }

  static int n_rooms(){
    return N_ROOMS;
  }

  static int n_levels(){
    return N_LEVELS;
  }
  
}
