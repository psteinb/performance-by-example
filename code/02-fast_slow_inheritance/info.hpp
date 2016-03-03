#pragma once
#include <vector>
#include <random>

#include "iot.hpp"

namespace iot {

  struct device_t {

    int temperature_kelvin;
    
    int network_protocol;
    int network_address;
    int mac_address;

    float power_consumption;
  };
  
  
  static std::vector<device_t> fill_device_info(){
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

  static void update_device_info(std::vector<device_t>& _vector){

    std::srand(size());

    for (device_t & item : _vector){
      
      item.temperature_kelvin += ((std::rand()/float(RAND_MAX)) - 1)*10;
      
      item.power_consumption += (std::rand()/RAND_MAX)*20;
    }

  }

  
  static std::vector<device_t> device_info = fill_device_info();
  
}
