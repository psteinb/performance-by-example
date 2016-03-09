#pragma once
#include <vector>
#include <random>

#include "iot.hpp"

namespace iot {

  struct device_t {

    int temperature_kelvin;
    
    int network_protocol;
    int network_address ;
    int mac_address     ;

    float power_consumption;

    
    device_t operator-(const device_t& _rhs) const {

      device_t value;
      value.temperature_kelvin = temperature_kelvin - _rhs.temperature_kelvin;
      
      value.network_protocol = network_protocol != _rhs.network_protocol ? 0 : _rhs.network_protocol;
      value.network_address  = network_address  != _rhs.network_address  ? 0 : _rhs.network_address;
      value.mac_address      = mac_address      != _rhs.mac_address      ? 0 : _rhs.mac_address    ;
      
      value.power_consumption = power_consumption - _rhs.power_consumption;

      return value;
    }

  };
    
  static std::vector<device_t> fill_device_info(){
    std::srand(size());
    
    std::vector<device_t> value(size());

    double rand_max_dbl = RAND_MAX;
    
    for (device_t & item : value){
      item.temperature_kelvin = (std::rand()/rand_max_dbl)*10 + 273.5;
      item.network_protocol = (std::rand()/rand_max_dbl) > .5 ? 1 : 0;
      item.network_address = (std::rand()/rand_max_dbl);
      item.mac_address = item.network_address/42;

      item.power_consumption = (std::rand()/rand_max_dbl)*10;
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
