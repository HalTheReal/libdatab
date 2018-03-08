#ifndef TIME_H
#define TIME_H

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>    // Eccezioni
#include <iostream>     // std::cout
#include <math.h>       // fmod
#include <tools.h>

namespace Epoch {

  class Time {
    public:
      Time();
      Time(int secs);
      Time(int hs, int ms, int ss);

      int hour() const;
      int min() const;
      int sec() const;
      
      Time& addSec(int secs);

    private:
      int hours;
      int minutes;
      int seconds;

      bool isValid() const;
  };

  bool operator == (const Time &tm1, const Time &tm2);
  bool operator != (const Time &tm1, const Time &tm2);
  bool operator < (const Time &tm1, const Time &tm2);
  bool operator <= (const Time &tm1, const Time &tm2);
  bool operator > (const Time &tm1, const Time &tm2);
  bool operator >= (const Time &tm1, const Time &tm2);

  std::ostream& operator << (std::ostream &stream, const Time &tm);
  std::istream& operator >> (std::istream &stream, Time &tm);

  int toInt(const Time &tm);
  std::string toString(const Time &tm, char sep = ':');
  Time strToTime(const std::string &str);
}

#endif
