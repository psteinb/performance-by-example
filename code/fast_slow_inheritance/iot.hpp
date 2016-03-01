#pragma once
#include <vector>
#include <random>

namespace iot {

  struct device_t {

    int temperature_kelvin;
    
    int network_protocol;
    int network_address;
    int mac_address;

    float power_consumption;
  };

  
  static int n_levels_ = 64;
  static int n_rooms_ = 1 << 18;
  
  static int n_rooms(){
    return n_rooms_;
  }

  static int n_levels(){
    return n_levels_;
  }

  static int size(){
    return n_levels_*n_rooms_;
  }
  
  static std::vector<device_t> fill_devices(){
    std::srand(size());
    
    std::vector<device_t> value(size());
    for (device_t & item : value){
      item.temperature_kelvin = (std::rand()/RAND_MAX)*273.5;
      item.network_protocol = (std::rand()/RAND_MAX) > .5 ? 1 : 0;
      item.network_address = (std::rand()/RAND_MAX);
      item.mac_address = item.network_address/42;

      item.power_consumption = (std::rand()/RAND_MAX)*10;
    }
    return value;
  }
  
  static std::vector<device_t> devices_ = fill_devices();
  

  static int active_devices(int level_number, int room_number){
    return devices_[(level_number*n_rooms())+room_number].power_consumption;
  }
  
}
