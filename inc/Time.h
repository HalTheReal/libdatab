#ifndef TIME_H
#define TIME_H

#include <string>
#include <vector>
#include <stdexcept>    // Eccezioni
#include <iostream>     // std::cout
#include <math.h>       // fmod

namespace Chrono {

  class Time {
    public:
      Time();
      Time(int secs);
      Time(int hs, int ms, int ss);

      int hour() const;
      int min() const;
      int sec() const;
      
      Time& addSeconds(int secs);

    private:
      int hours;
      int minutes;
      int seconds;
  };

  int toInt(const Time &tm);
  std::string toString(const Time &tm);
  Time strToTime(const std::string &str);

}

#endif
