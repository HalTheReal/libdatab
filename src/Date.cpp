#include "Date.h"

namespace Chrono {

  Date::Date()
    : dDay(1)
    , dMonth(1)
    , dYear(1970)
  {}

  Date::Date(int dy, int mt, int yr)
    : dDay(dy)
    , dMonth(mt)
    , dYear(yr)
  {
    if (!isValid()) {
      dDay = 1;
      dMonth = 1;
      dYear = 1970;
      throw std::runtime_error("Invalid Date");
    }
  }

  bool Date::isValid() const {
    if (dYear < 0 || dMonth < 1 || dDay < 1) {
      return false;
    }
    if (dMonth > 12 || dDay > daysInMonth(*this)) {
      return false;
    }
    return true;
  }

  int Date::day() const {
    return dDay;
  }

  int Date::month() const {
    return dMonth;
  }

  int Date::year() const {
    return dYear;
  }

  Date& Date::addDay(int toAdd) {
    while (toAdd < 0) {
      ++toAdd;
      --dDay;
      if (dDay == 0) {
        addMonth(-1);
        dDay += daysInMonth(*this);
      }
    }
    while (toAdd > 0) {
      --toAdd;
      ++dDay;
      if (dDay > daysInMonth(*this)) {
        addMonth(1);
        dDay = 1;
      }
    }
    return *this;
  }

  Date& Date::addMonth(int toAdd) {
    while (toAdd < 0) {
      ++toAdd;
      --dMonth;
      if (dMonth == 0) {
        dMonth = 12;
        addYear(-1);
      }
    }
    while (toAdd > 0) {
      --toAdd;
      ++dMonth;
      if (dMonth == 13) {
        dMonth = 1;
        addYear(1);
      }
    }
    return *this; 
  }

  Date& Date::addYear(int toAdd) {
    dYear += toAdd;
    // Succede solo se aggiungo 1 anno al 29/2
    if (dMonth == 2 && dDay > daysInMonth(*this)) {
      addDay(1);
    }
    // Eccezione se year < 0 ?
    return *this;
  }

  bool isLeap(const Date &dt) {
    if (dt.year() % 4 != 0) {
      return false;
    }
    else if (dt.year() % 100 != 0) {
      return true;
    }
    else if (dt.year() % 400 != 0) {
      return false;
    }
    else {
      return true;
    }
  }

  int daysInMonth(const Date &dt) {
    int days = 0;
    if (dt.month() == 2) {
      return isLeap(dt) ? 29 : 28;
    }
    if (dt.month() <= 7) {
      days = dt.month() % 2 == 0 ? 30 : 31;
    }
    else if (dt.month() > 7) {
      days = dt.month() % 2 == 0 ? 31 : 30;
    }
    return days;
  }

  int daysInYear(const Date &dt) {
    return (isLeap(dt) ? 366 : 365);
  }

  std::string toString(const Date &dt, char sep) {
    std::stringstream ss;
    ss << dt.day() << sep;
    ss << dt.month() << sep;
    ss << dt.year();
    return ss.str();
  }
}
