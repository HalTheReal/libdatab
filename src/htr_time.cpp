#include "htr_time.h"

namespace Epoch {

  TimeZone::TimeZone()
    : hours(0)
    , minutes(0)
  {}
  
  TimeZone::TimeZone(int hr, unsigned mnt)
    : hours(hr)
    , minutes(mnt)
  {
    if (!isValid()) {
      throw std::runtime_error("Invalid TimeZone");
    }
  }

  bool TimeZone::isValid() const {
    if(minutes > 59) {
      return false;
    }
    return true;
  }

  int TimeZone::hour() const {
    return hours;
  }

  int TimeZone::mnt() const {
    return minutes;
  }

  bool TimeZone::isAfterUTC() const {
    return hours >= 0;
  }

  bool operator == (const TimeZone &tz1, const TimeZone &tz2) {
    return toInt(tz1) == toInt(tz2);
  }

  bool operator != (const TimeZone &tz1, const TimeZone &tz2) {
    return toInt(tz1) != toInt(tz2);
  }

  bool operator < (const TimeZone &tz1, const TimeZone &tz2) {
    return toInt(tz1) < toInt(tz2);
  }

  bool operator <= (const TimeZone &tz1, const TimeZone &tz2) {
    return toInt(tz1) <= toInt(tz2);
  }

  bool operator > (const TimeZone &tz1, const TimeZone &tz2) {
    return toInt(tz1) > toInt(tz2);
  }

  bool operator >= (const TimeZone &tz1, const TimeZone &tz2) {
    return toInt(tz1) >= toInt(tz2);
  }
 
  int toInt(const TimeZone& tz) {
    int sec;
    if (tz.isAfterUTC()) {
      sec = tz.hour() * 3600 + tz.mnt() * 60;
    }
    else {
      sec = tz.hour() * 3600 - tz.mnt() * 60;
    }
    return sec;
  }

  std::string to_string(const TimeZone &tz) {
    std::stringstream ss;
    if (tz.isAfterUTC()) {
      ss << '+';
      ss << std::setfill('0') << std::setw(2) << tz.hour();
    }
    else {
      ss << '-';
      ss << std::setfill('0') << std::setw(2) << - tz.hour();
    }
    ss << ':';
    ss << std::setfill('0') << std::setw(2) << tz.mnt();
    return ss.str();
  }

  std::ostream& operator << (std::ostream &stream, const TimeZone &tz) {
    if(!stream.good()) {
      return stream;
    }
    stream << to_string(tz);
    return stream;
  }

  Time::Time()
    : hours(0)
    , minutes(0)
    , seconds(0)
    , timezone()
  {}

  Time::Time(int hs, int ms, int ss, TimeZone tz)
    : hours(hs)
    , minutes(ms)
    , seconds(ss)
    , timezone(tz)
  {
    if (!isValid()) {
      throw std::runtime_error("Invalid Time");
    }
  }

  Time::Time(int secs, TimeZone tz)
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
    timezone = tz;
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

  int Time::mnt() const {
    return minutes;
  }

  int Time::sec() const {
    return seconds;
  }

  TimeZone Time::zone() const {
   return timezone;
  }

  Time& Time::addSec(int secs) {
    int base = 0;
    base += seconds;
    base += minutes * 60;
    base += hours * 60 * 60;
    *this = Time(base + secs, timezone);
    return *this;
  }

  Time toUTC(Time tm) {
    int offset = toInt(tm.zone());
    int base = toInt(tm);
    return Time(offset + base);
  }

  int toInt(const Time &tm) {
    int ret = 0;
    ret += tm.sec();
    ret += tm.mnt() * 60;
    ret += tm.hour() * 60 * 60;
    return ret;
  }

  bool operator == (const Time &tm1, const Time &tm2) {
    return toInt(toUTC(tm1)) == toInt(toUTC(tm2));
  }

  bool operator != (const Time &tm1, const Time &tm2) {
    return !(tm1 == tm2);
  }

  bool operator < (const Time &tm1, const Time &tm2) {
    return toInt(toUTC(tm1)) < toInt(toUTC(tm2));
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

 std::ostream& operator << (std::ostream &stream, const Time &tm) {
    stream << to_string(tm);
    return stream;
  }

  std::istream& operator >> (std::istream &stream, Time &tm) {
    char s1, s2, s3;
    int hour, min, sec, htz, mtz;
    stream >> hour >> s1 >> min >> s2 >> sec >> htz >> s3 >> mtz;
    if (!stream) {
      return stream;
    }
    if (s1 != s2 || s1 != s3) {
      stream.setstate(std::ios_base::failbit);
      return stream;
    }
    try {
      tm = Time(hour, min, sec, TimeZone(htz, mtz));
    }
    catch (const std::exception &e) {
      stream.setstate(std::ios_base::failbit);
    }
    return stream;
  }

  std::string to_string(const Time &tm, char sep) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << tm.hour();
    ss << sep;
    ss << std::setfill('0') << std::setw(2) << tm.mnt();
    ss << sep;
    ss << std::setfill('0') << std::setw(2) << tm.sec();
    ss << to_string(tm.zone());
    return ss.str();
  }

  Time strToTime(const std::string &str) {
    std::stringstream ss(str);
    Time tm;
    if (ss >> tm) {
      return tm;
    }
    throw std::invalid_argument("Invalid format");
  }

}

int width(const Range<Epoch::Time> &rng) {
  int upper = toInt(rng.upper());
  int lower = toInt(rng.lower());
  return upper - lower;
}

template <>
Range<Epoch::Time>& shift<Epoch::Time>(Range<Epoch::Time> &rng, int sec) {
  rng.setUpper(rng.upper().addSec(sec));
  rng.setLower(rng.lower().addSec(sec));
  return rng;
}

