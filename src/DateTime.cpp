#include "DateTime.h"

namespace Chrono {

  DateTime::DateTime()
    : dateNow()
    , timeNow()
  {}

  DateTime::DateTime(int dy, int mn, int yr, int hr, int mi, int sc)
    : dateNow(dy, mn, yr)
    , timeNow(hr, mi, sc)
  {}

  DateTime::DateTime(const Time &tm, const Date &dt)
    : DateTime()
  {
    dateNow = dt;
    timeNow = tm;
  }

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

  std::string toString(const DateTime &dtt, char dateSep) {
    std::stringstream ss;
    ss << dtt.day() << dateSep;
    ss << dtt.month() << dateSep;
    ss << dtt.year() << ' ';
    if (dtt.hour() < 10) {
      ss << '0';
    }
    ss << dtt.hour() << ':';
    if (dtt.min() < 10) {
      ss << '0';
    }
    ss << dtt.min() << ':';
    if (dtt.sec() < 10) {
      ss << '0';
    }
    ss << dtt.sec();
    return ss.str();
  }

}
