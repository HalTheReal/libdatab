#include "Time.h"

namespace Chrono {

  Time::Time()
    : hours(0)
    , minutes(0)
    , seconds(0)
  {}

  // TODO Eccezione se l'orario non è valido
  Time::Time(int hs, int ms, int ss)
    : hours(hs)
    , minutes(ms)
    , seconds(ss)
  {}

  Time::Time(int secs)
    : Time()
  {
    int usable = secs - ((secs / 86400) * 86400);
    if (usable < 0) {
      usable += 86400;
    }
    hours = usable / 3600;
    usable -= hours * 3600;
    minutes = usable / 60;
    usable -= minutes * 60;
    seconds = usable;
  }

  int Time::hour() const {
    return hours;
  }

  int Time::min() const {
    return minutes;
  }

  int Time::sec() const {
    return seconds;
  }

  Time& Time::addSeconds(int secs) {
    int base = toInt(*this);
    Time add(base + secs);
    hours = add.hour();
    minutes = add.min();
    seconds = add.sec();
    return *this;
  }

  int toInt(const Time &tm) {
    int ret = 0;
    ret += tm.sec();
    ret += tm.min() * 60;
    ret += tm.hour() * 60 * 60;
    return ret;
  }

  std::string toString(const Time &tm) {
    std::stringstream ss;
    if (tm.hour() < 10) {
      ss << '0';
    }
    ss << tm.hour() << ':';
    if (tm.min() < 10) {
      ss << '0';
    }
    ss << tm.min() << ':';
    if (tm.sec() < 10) {
      ss << '0';
    }
    ss << tm.sec();
    return ss.str();
  }

}
