#ifndef __RENDER_ENTITY_HPP__
#define __RENDER_ENTITY_HPP__

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include "info.hpp"

namespace iot {

  struct temperature_view {

    int value;
    std::string html_start;
    std::string val;
    std::string html_end;

    temperature_view(int _data):
      value(_data),
      html_start("<em>"),
      val(std::to_string(_data)),
      html_end(" K</em>"){}

    int prepare() const {

      return val.size();
    }
  };

  struct power_view {

    float value;
    std::string html_start;
    std::string val;
    std::string html_end;

    power_view(float _data):
      value(_data),
      html_start("<em>"),
      val(std::to_string(_data)),
      html_end(" kWh</em>"){}

    int prepare() const {
      
      // std::this_thread::sleep_for(dur);
      return val.size();
    }
    
  };

  
  struct render_entity {

    temperature_view temp;
    power_view pow;
    
    render_entity():
      temp(0),
      pow(0)
    {
      
    }
    
    render_entity(const device_t& _dev):
      temp(_dev.temperature_kelvin),
      pow(_dev.power_consumption)
    {
      
    }

    temperature_view const * temperature() const {
      return &temp;
    }

    power_view const * power() const {
      return &pow;
    }

  };
  
}

#endif
