#include "htr_datetime.h"

namespace Epoch {

  DateTime::DateTime()
    : dateNow()
    , timeNow()
  {}

  DateTime::DateTime(int dy, int mn, int yr, int hr, int mi, int sc)
    : dateNow(dy, mn, yr)
    , timeNow(hr, mi, sc)
  {}

  DateTime::DateTime(const Date &dt, const Time &tm)
    : dateNow(dt)
    , timeNow(tm)
  {}

  int DateTime::day() const {
    return dateNow.day();
  }

  int DateTime::month() const {
    return dateNow.month();
  }

  int DateTime::year() const {
    return dateNow.year();
  }

  int DateTime::hour() const {
    return timeNow.hour();
  }

  int DateTime::min() const {
    return timeNow.min();
  }

  int DateTime::sec() const {
    return timeNow.sec();
  }

  DateTime& DateTime::addDay(int toAdd) {
    dateNow.addDay(toAdd);
    return *this;
  }

  DateTime& DateTime::addSec(int toAdd) {
    int daysToAdd = toAdd / 86400;
    toAdd -= daysToAdd * 86400;
    Time before = timeNow;
    timeNow.addSec(toAdd);
    if (toAdd > 0 && timeNow < before) {      // Ho aggiunto ma before è >
      ++daysToAdd;
    }
    else if (toAdd < 0 && timeNow > before) { // Ho tolto ma before è <
      --daysToAdd;
    }
    dateNow.addDay(daysToAdd);
    return *this;
  }

  int toUnix(DateTime dtt) {
    DateTime uxZero(1, 1, 1970, 0, 0, 0);
    int midnightOffset = 86400 - toInt(toTime(dtt));
    dtt.addSec(midnightOffset);
    int dayToZero = 0;
    while (dtt > uxZero) {
      dtt.addDay(-1);
      ++dayToZero;
    }
    while (dtt < uxZero) {
      dtt.addDay(1);
      --dayToZero;
    }
    return (dayToZero * 86400) - midnightOffset;
  }

  Date toDate(const DateTime &dtt) {
    return (Date(dtt.day(), dtt.month(), dtt.year()));
  }

  Time toTime(const DateTime &dtt) {
    return (Time(dtt.hour(), dtt.min(), dtt.sec()));
  }

  std::string to_string(const DateTime &dtt, char dtSep, char tmSep) {
    std::string dtStr = to_string(toDate(dtt), dtSep);
    std::string tmStr = to_string(toTime(dtt), tmSep);
    std::stringstream ss;
    ss << dtStr << ' ' << tmStr;
    return ss.str();
  }

  DateTime strToDateTime(const std::string &str) {
    std::stringstream ss;
    ss << str;
    Date dt;
    Time tm;
    if (ss >> dt >> tm) {
      return DateTime(dt, tm);
    }
    throw std::invalid_argument("Invalid format");
  }

  bool operator == (const DateTime &dtt1, const DateTime &dtt2) {
    if (toTime(dtt1) == toTime(dtt2) && toDate(dtt1) == toDate(dtt2)) {
      return true;
    }
    return false;
  }

  bool operator != (const DateTime &dtt1, const DateTime &dtt2) {
    return !(dtt1 == dtt2);
  }

  bool operator < (const DateTime &dtt1, const DateTime &dtt2) {
    if (toDate(dtt1) < toDate(dtt2)) {
      return true;
    }
    if (toDate(dtt1) > toDate(dtt2)) {
      return false;
    }
    if (toTime(dtt1) < toTime(dtt2)) {
      return true;
    }
    // Se arrivo qui le date sono == ma l'orario
    // non è minore! quindi l'orario è >=
    return false;
  }

  bool operator <= (const DateTime &dtt1, const DateTime &dtt2) {
    return (dtt1 < dtt2 || dtt1 == dtt2);
  }

  bool operator > (const DateTime &dtt1, const DateTime &dtt2) {
    return !(dtt1 <= dtt2);
  }

  bool operator >= (const DateTime &dtt1, const DateTime &dtt2) {
    return !(dtt1 < dtt2);
  }

  std::ostream& operator << (std::ostream &stream, const DateTime &dtt) {
    if(!stream.good()) {
      return stream;
    }
    stream << to_string(dtt);
    return stream;
  }

  std::istream& operator >> (std::istream &stream, DateTime &dtt) {
    if(!stream) {
      return stream;
    }
    Time tm;
    Date dt;
    if (stream >> dt >> tm) {
      dtt = DateTime(dt, tm);
    }
    return stream;
  }

  DateTime to_DateTime(const std::tm &tm) {
    int month = tm.tm_mon + 1;    // tm_mon = months from Jan
    int year = tm.tm_year + 1900; // tm_year = years from 1900
    return DateTime(tm.tm_mday, month, year, tm.tm_hour, tm.tm_min, tm.tm_sec);
  }

  DateTime local() {
    using std::chrono::system_clock;
    system_clock::time_point now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);
#ifdef _WIN32
    std::tm local;
    localtime_s(&local, &tt);
#endif
#ifdef __linux__
    std::tm local = *localtime_r(&tt, &local);
#endif
    return to_DateTime(local);
  }

  DateTime gmt() {
    using std::chrono::system_clock;
    system_clock::time_point now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);
#ifdef _WIN32
    std::tm gmt;
    gmtime_s(&gmt, &tt);
#endif
#ifdef __linux__
    std::tm gmt = *gmtime_r(&tt, &gmt);
#endif
    return to_DateTime(gmt);
  }

}

int width(const Range<Epoch::DateTime> &rng) {
  Range<Epoch::Date> dRange(Epoch::toDate(rng.lower()), Epoch::toDate(rng.upper()));
  Range<Epoch::Time> tRange(Epoch::toTime(rng.lower()), Epoch::toTime(rng.upper()));
  int days = width(dRange);
  int secs = width(tRange);
  secs += days * 24 * 60 * 60;
  return secs;
}

template <>
Range<Epoch::DateTime>& shift<Epoch::DateTime>(Range<Epoch::DateTime> &rng, int sec) {
  rng.setUpper(rng.upper().addSec(sec));
  rng.setLower(rng.lower().addSec(sec));
  return rng;
}
