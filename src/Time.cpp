#include "Time.h"

namespace Chrono {

  Time::Time()
    : hours(0)
    , minutes(0)
    , seconds(0)
  {}

  Time::Time(int hs, int ms, int ss)
    : hours(hs)
    , minutes(ms)
    , seconds(ss)
  {
    if (!isValid()) {
      throw std::runtime_error("Invalid Time");
    }
  }

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

  bool Time::isValid() const {
    if(hours < 0 || minutes < 0 || seconds < 0) {
      return false;
    }
    if(hours > 23 || minutes > 59 || seconds > 59) {
      return false;
    }
    return true;
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

  Time& Time::addSec(int secs) {
    int base = toInt(*this);
    Time add(base + secs);
    hours = add.hour();
    minutes = add.min();
    seconds = add.sec();
    return *this;
  }

  bool operator == (const Time &tm1, const Time &tm2) {
    if (tm1.hour() == tm2.hour() && tm1.min() == tm2.min() && tm1.sec() == tm2.sec()) {
      return true;
    }
    return false;
  }

  bool operator != (const Time &tm1, const Time &tm2) {
    return !(tm1 == tm2);
  }

  bool operator < (const Time &tm1, const Time &tm2) {
    if (tm1.hour() < tm2.hour()) {
      return true;
    }
    if (tm1.hour() > tm2.hour()) {
      return false;
    }
    if (tm1.min() < tm2.min()) {
      return true;
    }
    if (tm1.min() > tm2.min()) {
      return false;
    }
    if (tm1.sec() < tm2.sec()) {
      return true;
    }
    if (tm1.sec() > tm2.sec()) {
      return false;
    }
    return false;
  }

  bool operator <= (const Time &tm1, const Time &tm2) {
    return (tm1 < tm2 || tm1 == tm2);
  }

  bool operator > (const Time &tm1, const Time &tm2) {
    return !(tm1 <= tm2);
  }

  bool operator >= (const Time &tm1, const Time &tm2) {
    return !(tm1 < tm2);
  }

  int toInt(const Time &tm) {
    int ret = 0;
    ret += tm.sec();
    ret += tm.min() * 60;
    ret += tm.hour() * 60 * 60;
    return ret;
  }

  std::ostream& operator << (std::ostream &stream, const Time &tm) {
    if (tm.hour() < 10) {
      stream << '0';
    }
    stream << tm.hour() << ':';
    if (tm.min() < 10) {
      stream << '0';
    }
    stream << tm.min() << ':';
    if (tm.sec() < 10) {
      stream << '0';
    }
    stream << tm.sec();
    return stream;
  }

  std::istream& operator >> (std::istream &stream, Time &tm) {
    if (!stream.good()) {
      return stream;
    }
    std::string timeStr;
    std::vector <std::string> toks;
    int hour, min, sec;
    if (stream >> timeStr) {
      toks = tls::split(timeStr, ':');
    }
    try {
      hour = std::stoi(toks[0]);
      min = std::stoi(toks[1]);
      sec = std::stoi(toks[2]);
      tm = Time(hour, min, sec);
    }
    catch (const std::exception &e) {
      std::cout << "Presa!\n";
      stream.setstate(std::ios_base::failbit);
    }
    return stream;
  }

  std::string toString(const Time &tm) {
    std::stringstream ss;
    ss << tm;
    return ss.str();
  }

}
