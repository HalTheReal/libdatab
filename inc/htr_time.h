#ifndef TIME_H
#define TIME_H

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iostream>     // std::cout
#include <iomanip>
#include <math.h>       // fmod
#include <htr_tools.h>
#include <htr_range.h>

namespace Epoch {

  class TimeZone {
    public:
      TimeZone();
      TimeZone(int hr, unsigned mnt = 0);

      int hour() const;
      int mnt() const;
      bool isAfterUTC() const;
    private:
      int hours;
      int minutes;
      bool isValid() const;
  };

  bool operator == (const TimeZone &tz1, const TimeZone &tz2);
  bool operator != (const TimeZone &tz1, const TimeZone &tz2);
  bool operator < (const TimeZone &tz1, const TimeZone &tz2);
  bool operator <= (const TimeZone &tz1, const TimeZone &tz2);
  bool operator > (const TimeZone &tz1, const TimeZone &tz2);
  bool operator >= (const TimeZone &tz1, const TimeZone &tz2);

  std::ostream& operator << (std::ostream &stream, const TimeZone &tm);

  int toInt(const TimeZone& tz);
  std::string to_string(const TimeZone &tz);
  
  const TimeZone UTC;
  const TimeZone CET(1);
  const TimeZone CEST(2);

  class Time {
    public:
      Time();
      Time(int secs, TimeZone tz = UTC);
      Time(int hs, int ms, int ss, TimeZone tz = UTC);

      int hour() const;
      int mnt() const;
      int sec() const;
      TimeZone zone() const;
      
      Time& addSec(int secs);

    private:
      int hours;
      int minutes;
      int seconds;
      TimeZone timezone;

      bool isValid() const;
  };

  Time toTimeZone(Time tm, const TimeZone& tz);
  Time toUTC(Time tm);
  int toInt(const Time &tm);

  bool operator == (const Time &tm1, const Time &tm2);
  bool operator != (const Time &tm1, const Time &tm2);
  bool operator < (const Time &tm1, const Time &tm2);
  bool operator <= (const Time &tm1, const Time &tm2);
  bool operator > (const Time &tm1, const Time &tm2);
  bool operator >= (const Time &tm1, const Time &tm2);

  std::ostream& operator << (std::ostream &stream, const Time &tm);
  std::istream& operator >> (std::istream &stream, Time &tm);

  std::string to_string(const Time &tm);
  Time strToTime(const std::string &str);

}

int width(const Range<Epoch::Time> &rng);
template <>
Range<Epoch::Time>& shift<Epoch::Time>(Range<Epoch::Time> &rng, int sec);

#endif
